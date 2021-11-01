/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerColorantTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtMarkerColorantTable.h"


static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtMarkerColorantTable_oid[] = { prtMarkerColorantTable_TABLE_OID };
size_t prtMarkerColorantTable_oid_len = OID_LENGTH(prtMarkerColorantTable_oid);

static int prtMarkerColorantTable_module_initialized = 0;


#if 0
/************************************************************
 * get data from a row
 */
int get_prtMarkerColorantTable_data(
                prtMarkerColorantTable_context *out, unsigned long colFlags)
{
    int getAll = colFlags ? 0 : 1;
    prtMarkerColorantTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerColorantTable_module_initialized) {
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
    row = (prtMarkerColorantTable_context *) 
                                    prtMarkerColorantTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtMarkerColorantTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERCOLORANTINDEX:
            out->prtMarkerColorantIndex = row->prtMarkerColorantIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERCOLORANTMARKERINDEX:
            out->prtMarkerColorantMarkerIndex = 
                row->prtMarkerColorantMarkerIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERCOLORANTROLE:
            out->prtMarkerColorantRole = row->prtMarkerColorantRole;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERCOLORANTVALUE:
            memcpy((void *) out->prtMarkerColorantValue,
                (const void *) row->prtMarkerColorantValue,
                row->prtMarkerColorantValue_len);
            out->prtMarkerColorantValue_len = row->prtMarkerColorantValue_len;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERCOLORANTTONALITY:
            out->prtMarkerColorantTonality = row->prtMarkerColorantTonality;
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
int set_prtMarkerColorantTable_data(
                prtMarkerColorantTable_context *data, unsigned long colFlags)
{
    int setAll = colFlags ? 0 : 1;
    prtMarkerColorantTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerColorantTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerColorantTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtMarkerColorantTable_context *) 
                                    prtMarkerColorantTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtMarkerColorantTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERCOLORANTINDEX:
            /** TODO: revisit
              * also a row index, must be consistent with index.oids[1]
              * do not allow changing this value for now
              * to change this value, another row must be created
              */
#if 0
            row->prtMarkerColorantIndex = data->prtMarkerColorantIndex;
#else
            snmp_log(LOG_ERR, 
                    "%s: cannot change marker colorant index\n", __func__);
#endif
            if (!setAll)
                break;
        case COLUMN_PRTMARKERCOLORANTMARKERINDEX:
            row->prtMarkerColorantMarkerIndex = 
                data->prtMarkerColorantMarkerIndex;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERCOLORANTROLE:
            row->prtMarkerColorantRole = data->prtMarkerColorantRole;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERCOLORANTVALUE:
            memcpy((void *) row->prtMarkerColorantValue,
                (const void *) data->prtMarkerColorantValue,
                data->prtMarkerColorantValue_len);
            row->prtMarkerColorantValue_len = data->prtMarkerColorantValue_len;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERCOLORANTTONALITY:
            row->prtMarkerColorantTonality = data->prtMarkerColorantTonality;
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
int del_prtMarkerColorantTable_entry(long devNdx, long mrkNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtMarkerColorantTable_context *row;

    /** make sure the module is initialized */
    if (!prtMarkerColorantTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = mrkNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtMarkerColorantTable_context *) 
                                    prtMarkerColorantTable_get_by_idx(&hdr);
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
int add_prtMarkerColorantTable_entry(prtMarkerColorantTable_context *info)
{
    netsnmp_index *hdr;
    prtMarkerColorantTable_context *row;

    /** make sure the module is initialized first */
    if (!prtMarkerColorantTable_module_initialized)
        init_prtMarkerColorantTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerColorantTable_data(info, 0))
        return -1;
    hdr = &info->index;


    /** make sure desired index tuple are non-existent */
    row = (prtMarkerColorantTable_context *) 
                                    prtMarkerColorantTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                    __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtMarkerColorantTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy input column data */
    row->prtMarkerColorantIndex =           hdr->oids[1];
    row->prtMarkerColorantMarkerIndex =     info->prtMarkerColorantMarkerIndex;
    row->prtMarkerColorantRole =            info->prtMarkerColorantRole;
    memcpy((void *) row->prtMarkerColorantValue, (const void *)
                                            info->prtMarkerColorantValue,
                                            info->prtMarkerColorantValue_len);
    row->prtMarkerColorantValue_len =       info->prtMarkerColorantValue_len;
    row->prtMarkerColorantTonality =        info->prtMarkerColorantTonality;

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtMarkerColorantTable_data(
                prtMarkerColorantTable_context *data, unsigned long colFlags)
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
        snmp_log(LOG_ERR, "invalid marker colorant index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtMarkerColorantTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERCOLORANTINDEX:
            if (!data->prtMarkerColorantIndex)
                data->prtMarkerColorantIndex = data->index.oids[1];
            else if (data->prtMarkerColorantIndex != data->index.oids[1]) {
                snmp_log(LOG_ERR, 
                        "inconsistent marker colorant index: %d and %d\n",
                            (int) data->index.oids[1], 
                            (int) data->prtMarkerColorantIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERCOLORANTMARKERINDEX:
            if (!IS_0TOFFFF(data->prtMarkerColorantMarkerIndex)) {
                snmp_log(LOG_ERR, "invalid marker colorant marker index\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERCOLORANTROLE:
            if (!IS_PRTMARKERCOLORANTROLE_VALID(data->prtMarkerColorantRole)) {
                snmp_log(LOG_ERR, "%s: invalid marker colorant role (%d)\n",
                    __func__, (int) data->prtMarkerColorantRole);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERCOLORANTVALUE:
            if (!IS_0TO255(data->prtMarkerColorantValue_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid marker colorant value length (%d)\n",
                        __func__, (int) data->prtMarkerColorantValue_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERCOLORANTTONALITY:
            if (!IS_2TO7FFFFFFF(data->prtMarkerColorantTonality)) {
                snmp_log(LOG_ERR, "%s: invalid marker colorant tonality (%d)\n",
                    __func__, (int) data->prtMarkerColorantTonality);
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
void populate_prtMarkerColorantTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtMarkerColorantTable_context *row;
    uint32_t i, numOfMarkerColorants;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfMarkerColorants, NULL))
        numOfMarkerColorants = 1;

    if (!prtMarkerColorantTable_module_initialized)
        initialize_table_prtMarkerColorantTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfMarkerColorants; i++) {

        indexes[1] = i + 1;
        row = prtMarkerColorantTable_create_row(&hdr);
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
void init_prtMarkerColorantTable(void)
{
    initialize_table_prtMarkerColorantTable();
    prtMarkerColorantTable_module_initialized = 1;
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtMarkerColorantTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR, 
            "initialize_table_prtmarkercoloranttable_handler called again\n");
        return;
    }
    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info) {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtMarkerColorantTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtMarkerColorantTable",
                                             netsnmp_table_array_helper_handler,
                                             prtMarkerColorantTable_oid,
                                             prtMarkerColorantTable_oid_len,
                                             HANDLER_CAN_RONLY
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtMarkerColorantTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
        /** index: prtMarkerColorantIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtMarkerColorantTable_COL_MIN;
    table_info->max_column = prtMarkerColorantTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtMarkerColorantTable_get_value;
    cb.container = netsnmp_container_find("prtMarkerColorantTable_primary:"
                                          "prtMarkerColorantTable:"
                                          "table_container");
    DEBUGMSGTL(("initialize_table_prtMarkerColorantTable",
                "Registering table prtMarkerColorantTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * prtMarkerColorantTable_get_value
 *
 * This routine is called for get requests to copy the data
 * from the context to the varbind for the request. If the
 * context has been properly maintained, you don't need to
 * change in code in this fuction.
 */
int prtMarkerColorantTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    netsnmp_variable_list *var = request->requestvb;
    prtMarkerColorantTable_context *ctx = 
        (prtMarkerColorantTable_context *) item;

    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
    uint32_t ndx = map->marvell_oid_index;

    if (ndx == INDEX_VARIABLE)
        ndx = ctx->index.oids[1] - 1;

    switch(table_info->colnum) {

#if 0
        case COLUMN_PRTMARKERCOLORANTMARKERINDEX:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerColorantMarkerIndex,
                         sizeof(context->prtMarkerColorantMarkerIndex) );
        break;
    
        case COLUMN_PRTMARKERCOLORANTROLE:
            /** PrtMarkerColorantRoleTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerColorantRole,
                         sizeof(context->prtMarkerColorantRole) );
        break;
    
        case COLUMN_PRTMARKERCOLORANTVALUE:
            /** OCTETSTR = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtMarkerColorantValue,
                         context->prtMarkerColorantValue_len );
        break;
    
        case COLUMN_PRTMARKERCOLORANTTONALITY:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerColorantTonality,
                         sizeof(context->prtMarkerColorantTonality) );
        break;
#else
        case COLUMN_PRTMARKERCOLORANTMARKERINDEX:
        case COLUMN_PRTMARKERCOLORANTROLE:
        case COLUMN_PRTMARKERCOLORANTTONALITY:
        {
            uint32_t num = 0;

            if (OK != get_prtMIB_OIDValue(map, ndx, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;
        } 
        case COLUMN_PRTMARKERCOLORANTVALUE:
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
                     "prtMarkerColorantTable_get_value\n");
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for row based on a set of indexes
 */
const prtMarkerColorantTable_context *prtMarkerColorantTable_get_by_idx(
                                                        netsnmp_index * hdr)
{
    return (const prtMarkerColorantTable_context *)
        CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtMarkerColorantTable_context *prtMarkerColorantTable_create_row(
                                                        netsnmp_index *hdr)
{
    prtMarkerColorantTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtMarkerColorantTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR, "cannot allocate for marker colorant table\n");
        return NULL;
    }

    /** check indexes */
    if (prtMarkerColorantTable_extract_index(ctx, hdr)) {
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
int prtMarkerColorantTable_extract_index(
                    prtMarkerColorantTable_context *ctx, netsnmp_index *hdr)
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
        //ctx->prtMarkerColorantIndex = hdr->oids[1];
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers(&var_hrDeviceIndex);

    return err;
}
