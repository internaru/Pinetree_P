/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtInterpreterTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtInterpreterTable.h"


static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtInterpreterTable_oid[] = { prtInterpreterTable_TABLE_OID };
size_t prtInterpreterTable_oid_len = OID_LENGTH(prtInterpreterTable_oid);

static int prtInterpreterTable_row_copy(
            prtInterpreterTable_context *dst, prtInterpreterTable_context *src);

static int prtInterpreterTable_module_initialized = 0;


#if 0
/************************************************************
 * get data from a row
 */
int get_prtInterpreterTable_data(
                    prtInterpreterTable_context *out, unsigned long colFlags)
{
    prtInterpreterTable_context *row;
    int getAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtInterpreterTable_module_initialized) {
        snmp_log(LOG_ERR, 
                "%s: table module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!out || out->index.len != 2 || !out->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }

    /** find desired row */
    hdr = &out->index;
    row = (prtInterpreterTable_context *) prtInterpreterTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtInterpreterTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTINTERPRETERINDEX:
            out->prtInterpreterIndex = row->prtInterpreterIndex;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERLANGFAMILY:
            out->prtInterpreterLangFamily = row->prtInterpreterLangFamily;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERLANGLEVEL:
            memcpy((void*) out->prtInterpreterLangLevel,
                     (const void *) row->prtInterpreterLangLevel,
                                    row->prtInterpreterLangLevel_len);
            out->prtInterpreterLangLevel_len = 
                                    row->prtInterpreterLangLevel_len;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERLANGVERSION:
            memcpy((void*) out->prtInterpreterLangVersion,
                     (const void *) row->prtInterpreterLangVersion,
                                    row->prtInterpreterLangVersion_len);
            out->prtInterpreterLangVersion_len = 
                                    row->prtInterpreterLangVersion_len;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERDESCRIPTION:
            memcpy((void*) out->prtInterpreterDescription,
                     (const void *) row->prtInterpreterDescription,
                                    row->prtInterpreterDescription_len);
            out->prtInterpreterDescription_len = 
                                    row->prtInterpreterDescription_len;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERVERSION:
            memcpy((void*) out->prtInterpreterVersion,
                     (const void *) row->prtInterpreterVersion,
                                    row->prtInterpreterVersion_len);
            out->prtInterpreterVersion_len = row->prtInterpreterVersion_len;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            out->prtInterpreterDefaultOrientation = 
                                    row->prtInterpreterDefaultOrientation;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERFEEDADDRESSABILITY:
            out->prtInterpreterFeedAddressability = 
                                    row->prtInterpreterFeedAddressability;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY:
            out->prtInterpreterXFeedAddressability = 
                                    row->prtInterpreterXFeedAddressability;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            out->prtInterpreterDefaultCharSetIn = 
                                    row->prtInterpreterDefaultCharSetIn;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            out->prtInterpreterDefaultCharSetOut = 
                                    row->prtInterpreterDefaultCharSetOut;
            if (!getAll)
                break;
        case COLUMN_PRTINTERPRETERTWOWAY:
            out->prtInterpreterTwoWay = row->prtInterpreterTwoWay;
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
int set_prtInterpreterTable_data(
                    prtInterpreterTable_context *data, unsigned long colFlags)
{
    prtInterpreterTable_context *row;
    int setAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtInterpreterTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: table module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtInterpreterTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtInterpreterTable_context *) prtInterpreterTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtInterpreterTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTINTERPRETERINDEX:
            /** TODO: revisit
              * also a row index, must be consistent with index.oids[1]
              * do not allow changing this value for now
              * to change this value, another row must be created
              */
#if 0
            row->prtInterpreterIndex = data->prtInterpreterIndex;
#else
            snmp_log(LOG_ERR, 
                        "%s: cannot change interpreter index\n", __func__);
#endif
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERLANGFAMILY:
            row->prtInterpreterLangFamily = data->prtInterpreterLangFamily;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERLANGLEVEL:
            memcpy((void*) row->prtInterpreterLangLevel,
                     (const void *) data->prtInterpreterLangLevel,
                                    data->prtInterpreterLangLevel_len);
            row->prtInterpreterLangLevel_len = 
                                    data->prtInterpreterLangLevel_len;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERLANGVERSION:
            memcpy((void*) row->prtInterpreterLangVersion,
                     (const void *) data->prtInterpreterLangVersion,
                                    data->prtInterpreterLangVersion_len);
            row->prtInterpreterLangVersion_len = 
                                    data->prtInterpreterLangVersion_len;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERDESCRIPTION:
            memcpy((void*) row->prtInterpreterDescription,
                     (const void *) data->prtInterpreterDescription,
                                    data->prtInterpreterDescription_len);
            row->prtInterpreterDescription_len = 
                                    data->prtInterpreterDescription_len;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERVERSION:
            memcpy((void*) row->prtInterpreterVersion,
                     (const void *) data->prtInterpreterVersion,
                                    data->prtInterpreterVersion_len);
            row->prtInterpreterVersion_len = 
                                    data->prtInterpreterVersion_len;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            row->prtInterpreterDefaultOrientation = 
                                    data->prtInterpreterDefaultOrientation;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERFEEDADDRESSABILITY:
            row->prtInterpreterFeedAddressability = 
                                    data->prtInterpreterFeedAddressability;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY:
            row->prtInterpreterXFeedAddressability = 
                                    data->prtInterpreterXFeedAddressability;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            row->prtInterpreterDefaultCharSetIn = 
                                    data->prtInterpreterDefaultCharSetIn;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            row->prtInterpreterDefaultCharSetOut = 
                                    data->prtInterpreterDefaultCharSetOut;
            if (!setAll)
                break;
        case COLUMN_PRTINTERPRETERTWOWAY:
            row->prtInterpreterTwoWay = data->prtInterpreterTwoWay;
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
int del_prtInterpreterTable_entry(long devNdx, long intNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtInterpreterTable_context *row;

    /** make sure the module is initialized */
    if (!prtInterpreterTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: table module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = intNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtInterpreterTable_context *) prtInterpreterTable_get_by_idx(&hdr);
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
int add_prtInterpreterTable_entry(prtInterpreterTable_context *info)
{
    netsnmp_index *hdr;
    prtInterpreterTable_context *row;

    /** make sure the module is initialized */
    if (!prtInterpreterTable_module_initialized)
        init_prtInterpreterTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtInterpreterTable_data(info, 0))
        return -1;

    /** make sure desired index tuple are non-existent */
    hdr = &info->index;
    row = (prtInterpreterTable_context *) prtInterpreterTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtInterpreterTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy column data */
    if (prtInterpreterTable_row_copy(row, info)) {
        snmp_log(LOG_ERR, "%s: row content copying failed\n", __func__);
        return -1;
    }

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtInterpreterTable_data(
                    prtInterpreterTable_context *data, unsigned long colFlags)
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
        snmp_log(LOG_ERR, "invalid interpreter index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtInterpreterTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;

        switch (col) {

        case COLUMN_PRTINTERPRETERINDEX:
            if (!data->prtInterpreterIndex)
                data->prtInterpreterIndex = data->index.oids[1];
            else if (data->prtInterpreterIndex != data->index.oids[1]) {
                snmp_log(LOG_ERR, 
                            "inconsistent interpreter index: %d and %d\n",
                                    (int) data->index.oids[1], 
                                    (int) data->prtInterpreterIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERLANGFAMILY:
            if (!IS_PRTINTERPRETERLANGFAMILY_VALID(
                                            data->prtInterpreterLangFamily)) {
                snmp_log(LOG_ERR, "%s: invalid PDL family code (%d)\n",
                    __func__, (int) data->prtInterpreterLangFamily);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERLANGLEVEL:
            if (!IS_0TO31(data->prtInterpreterLangLevel_len)) {
                snmp_log(LOG_ERR, "%s: invalid PDL level length (%d)\n",
                    __func__, (int) data->prtInterpreterLangLevel_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERLANGVERSION:
            if (!IS_0TO31(data->prtInterpreterLangVersion_len)) {
                snmp_log(LOG_ERR, "%s: invalid PDL version length (%d)\n",
                    __func__, (int) data->prtInterpreterLangVersion_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERDESCRIPTION:
            if (!IS_0TO255(data->prtInterpreterDescription_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid interpreter description length (%d)\n",
                        __func__, (int) data->prtInterpreterDescription_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERVERSION:
            if (!IS_0TO31(data->prtInterpreterVersion_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid interpreter version length (%d)\n",
                        __func__, (int) data->prtInterpreterVersion_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            if (!IS_PRTPRINTORIENTATION_VALID(
                                    data->prtInterpreterDefaultOrientation)) {
                snmp_log(LOG_ERR, 
                    "%s: invalid interpreter default orientation (%d)\n",
                    __func__, (int) data->prtInterpreterDefaultOrientation);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERFEEDADDRESSABILITY:
            if (!IS_NEG2TO7FFFFFFF(data->prtInterpreterFeedAddressability)) {
                snmp_log(LOG_ERR, 
                        "invalid interpreter feed addressability (%d)\n",
                        (int) data->prtInterpreterFeedAddressability);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY:
            if (!IS_NEG2TO7FFFFFFF(data->prtInterpreterXFeedAddressability)) {
                snmp_log(LOG_ERR, 
                        "invalid interpreter cross feed addressability (%d)\n",
                        (int) data->prtInterpreterXFeedAddressability);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            if (!IS_IANACHARSET_VALID(data->prtInterpreterDefaultCharSetIn)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid interpreter default input charset (%d)\n",
                        __func__, (int) data->prtInterpreterDefaultCharSetIn);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            if (!IS_IANACHARSET_VALID(data->prtInterpreterDefaultCharSetOut)) {
                snmp_log(LOG_ERR, 
                    "%s: invalid interpreter default output charset (%d)\n",
                    __func__, (int) data->prtInterpreterDefaultCharSetOut);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINTERPRETERTWOWAY:
            if (!IS_PRTINTERPRETERTWOWAY_VALID(data->prtInterpreterTwoWay)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid interpreter two-way flag code (%d)\n",
                        __func__, (int) data->prtInterpreterTwoWay);
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
void populate_prtInterpreterTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtInterpreterTable_context *row;
    uint32_t i, numOfInterpreters;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfInterpreters, NULL))
        numOfInterpreters = 1;

    if (!prtInterpreterTable_module_initialized)
        initialize_table_prtInterpreterTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfInterpreters; i++) {

        indexes[1] = i + 1;
        row = prtInterpreterTable_create_row(&hdr);
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
void init_prtInterpreterTable(void)
{
    initialize_table_prtInterpreterTable();
    prtInterpreterTable_module_initialized = 1;
}

/************************************************************
 * copy the contents of a row
 */
static int prtInterpreterTable_row_copy(
            prtInterpreterTable_context *dst, prtInterpreterTable_context *src)
{
    if (!dst||!src)
        return 1;
        
    /** copy index, if provided */
    if (dst->index.oids)
        free(dst->index.oids);
    if (snmp_clone_mem( (void*)&dst->index.oids, src->index.oids,
                           src->index.len * sizeof(oid) )) {
        dst->index.oids = NULL;
        return 1;
    }
    dst->index.len = src->index.len;

#if 0
    /** copy components into the context structure */
    dst->prtInterpreterIndex = 
                src->prtInterpreterIndex;
    dst->prtInterpreterLangFamily = 
                src->prtInterpreterLangFamily;
    memcpy( dst->prtInterpreterLangLevel, 
                src->prtInterpreterLangLevel, 
                src->prtInterpreterLangLevel_len );
    dst->prtInterpreterLangLevel_len = 
                src->prtInterpreterLangLevel_len;
    memcpy( dst->prtInterpreterLangVersion, 
                src->prtInterpreterLangVersion, 
                src->prtInterpreterLangVersion_len );
    dst->prtInterpreterLangVersion_len = 
                src->prtInterpreterLangVersion_len;
    memcpy( dst->prtInterpreterDescription, 
                src->prtInterpreterDescription, 
                src->prtInterpreterDescription_len );
    dst->prtInterpreterDescription_len = 
                src->prtInterpreterDescription_len;
    memcpy( dst->prtInterpreterVersion, 
                src->prtInterpreterVersion, 
                src->prtInterpreterVersion_len );
    dst->prtInterpreterVersion_len = 
                src->prtInterpreterVersion_len;
    dst->prtInterpreterDefaultOrientation = 
                src->prtInterpreterDefaultOrientation;
    dst->prtInterpreterFeedAddressability = 
                src->prtInterpreterFeedAddressability;
    dst->prtInterpreterXFeedAddressability = 
                src->prtInterpreterXFeedAddressability;
    dst->prtInterpreterDefaultCharSetIn = 
                src->prtInterpreterDefaultCharSetIn;
    dst->prtInterpreterDefaultCharSetOut = 
                src->prtInterpreterDefaultCharSetOut;
    dst->prtInterpreterTwoWay = 
                src->prtInterpreterTwoWay;
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
int prtInterpreterTable_can_delete( prtInterpreterTable_context *undo_ctx,
                                    prtInterpreterTable_context *row_ctx,
                                    netsnmp_request_group *rg)
{
    return 0;
}

/************************************************************
 * duplicate a row
 */
prtInterpreterTable_context *prtInterpreterTable_duplicate_row(
                                    prtInterpreterTable_context *row_ctx)
{
    prtInterpreterTable_context *dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(prtInterpreterTable_context);
    if (!dup)
        return NULL;
        
    if (prtInterpreterTable_row_copy(dup, row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *prtInterpreterTable_delete_row(prtInterpreterTable_context *ctx)
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
void prtInterpreterTable_set_reserve1( netsnmp_request_group *rg )
{
    prtInterpreterTable_context *row_ctx =
            (prtInterpreterTable_context *)rg->existing_row;
    /*
    prtInterpreterTable_context *undo_ctx =
            (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_variable_list *var;
    netsnmp_request_group_item *current;
    int rc;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;
        rc = SNMP_ERR_NOERROR;

        if (!IS_COLUMN_IMPLEMENTED(row_ctx->columnInfo, current->tri->colnum))
            continue; /** no checking needed for unimplemented columns */

        switch(current->tri->colnum) {

        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            if (!IS_PRTPRINTORIENTATION_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            if (!IS_IANACHARSET_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "prtInterpreterTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc );
        rg->status = SNMP_MAX( rg->status, current->ri->status );
    }
}

void prtInterpreterTable_set_reserve2( netsnmp_request_group *rg )
{
    prtInterpreterTable_context *row_ctx = 
                    (prtInterpreterTable_context *)rg->existing_row;
    /*
    prtInterpreterTable_context *undo_ctx = 
                    (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;
    netsnmp_variable_list *var;
    int rc;

    rg->rg_void = rg->list->ri;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;
        rc = SNMP_ERR_NOERROR;

        if (!IS_COLUMN_IMPLEMENTED(row_ctx->columnInfo, current->tri->colnum))
            continue; /** no checking needed for unimplemented columns */

        switch(current->tri->colnum) {

        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            if (!IS_PRTPRINTORIENTATION_VALID(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            if (!IS_IANACHARSET_VALID(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;

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
void prtInterpreterTable_set_action( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    prtInterpreterTable_context *row_ctx = 
                    (prtInterpreterTable_context *)rg->existing_row;
    /*
    prtInterpreterTable_context *undo_ctx = 
                    (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    PRTMIB_OID_MAP *map;
    uint32_t ndx;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        map = row_ctx->columnInfo + current->tri->colnum;
        ndx = map->marvell_oid_index;
        if (ndx == INDEX_VARIABLE)
            ndx = row_ctx->index.oids[1] - 1;

        switch(current->tri->colnum) {

#if 0
            case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
                /** PrtPrintOrientationTC = ASN_INTEGER */
                row_ctx->prtInterpreterDefaultOrientation = *var->val.integer;
                break;

            case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
                /** IANACharset = ASN_INTEGER */
                row_ctx->prtInterpreterDefaultCharSetIn = *var->val.integer;
                break;

            case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
                /** IANACharset = ASN_INTEGER */
                row_ctx->prtInterpreterDefaultCharSetOut = *var->val.integer;
                break;
#else
            case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
                set_prtMIB_OIDValue(map, ndx, (void *) var->val.integer, NULL);
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
void prtInterpreterTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtInterpreterTable_context *row_ctx = 
                    (prtInterpreterTable_context *)rg->existing_row;
    prtInterpreterTable_context *undo_ctx = 
                    (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            /** PrtPrintOrientationTC = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            /** IANACharset = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            /** IANACharset = ASN_INTEGER */
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
void prtInterpreterTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtInterpreterTable_context *row_ctx = 
                    (prtInterpreterTable_context *)rg->existing_row;
    prtInterpreterTable_context *undo_ctx = 
                    (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            /** PrtPrintOrientationTC = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            /** IANACharset = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            /** IANACharset = ASN_INTEGER */
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
void prtInterpreterTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtInterpreterTable_context *row_ctx = 
                    (prtInterpreterTable_context *)rg->existing_row;
    prtInterpreterTable_context *undo_ctx = 
                    (prtInterpreterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            /** PrtPrintOrientationTC = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            /** IANACharset = ASN_INTEGER */
        break;

        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            /** IANACharset = ASN_INTEGER */
        break;

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }
    }
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtInterpreterTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR, 
                "initialize_table_prtInterpreterTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info)
    {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtInterpreterTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtInterpreterTable",
                                             netsnmp_table_array_helper_handler,
                                             prtInterpreterTable_oid,
                                             prtInterpreterTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtInterpreterTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
        /** index: prtInterpreterIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtInterpreterTable_COL_MIN;
    table_info->max_column = prtInterpreterTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtInterpreterTable_get_value;
    cb.container = netsnmp_container_find("prtInterpreterTable_primary:"
                                          "prtInterpreterTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod*)prtInterpreterTable_duplicate_row;
    cb.delete_row = (UserRowMethod*)prtInterpreterTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *)prtInterpreterTable_row_copy;

    cb.can_delete = (Netsnmp_User_Row_Action *)prtInterpreterTable_can_delete;

    cb.set_reserve1 = prtInterpreterTable_set_reserve1;
    cb.set_reserve2 = prtInterpreterTable_set_reserve2;
    cb.set_action = prtInterpreterTable_set_action;
    cb.set_commit = prtInterpreterTable_set_commit;
    cb.set_free = prtInterpreterTable_set_free;
    cb.set_undo = prtInterpreterTable_set_undo;
    DEBUGMSGTL(("initialize_table_prtInterpreterTable",
                "Registering table prtInterpreterTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * prtInterpreterTable_get_value
 *
 * This routine is called for get requests to copy the data
 * from the context to the varbind for the request. If the
 * context has been properly maintained, you don't need to
 * change in code in this fuction.
 */
int prtInterpreterTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    netsnmp_variable_list *var = request->requestvb;
    prtInterpreterTable_context *ctx = (prtInterpreterTable_context *)item;

    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
    uint32_t ndx = map->marvell_oid_index;

    if (ndx == INDEX_VARIABLE)
        ndx = ctx->index.oids[1] - 1;

    switch(table_info->colnum) {

#if 0
        case COLUMN_PRTINTERPRETERLANGFAMILY:
            /** PrtInterpreterLangFamilyTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterLangFamily,
                         sizeof(context->prtInterpreterLangFamily) );
        break;
    
        case COLUMN_PRTINTERPRETERLANGLEVEL:
            /** OCTETSTR = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtInterpreterLangLevel,
                         context->prtInterpreterLangLevel_len );
        break;
    
        case COLUMN_PRTINTERPRETERLANGVERSION:
            /** OCTETSTR = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtInterpreterLangVersion,
                         context->prtInterpreterLangVersion_len );
        break;
    
        case COLUMN_PRTINTERPRETERDESCRIPTION:
            /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtInterpreterDescription,
                         context->prtInterpreterDescription_len );
        break;
    
        case COLUMN_PRTINTERPRETERVERSION:
            /** OCTETSTR = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtInterpreterVersion,
                         context->prtInterpreterVersion_len );
        break;
    
        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
            /** PrtPrintOrientationTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterDefaultOrientation,
                         sizeof(context->prtInterpreterDefaultOrientation) );
        break;
    
        case COLUMN_PRTINTERPRETERFEEDADDRESSABILITY:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterFeedAddressability,
                         sizeof(context->prtInterpreterFeedAddressability) );
        break;
    
        case COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterXFeedAddressability,
                         sizeof(context->prtInterpreterXFeedAddressability) );
        break;
    
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
            /** IANACharset = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterDefaultCharSetIn,
                         sizeof(context->prtInterpreterDefaultCharSetIn) );
        break;
    
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
            /** IANACharset = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterDefaultCharSetOut,
                         sizeof(context->prtInterpreterDefaultCharSetOut) );
        break;
    
        case COLUMN_PRTINTERPRETERTWOWAY:
            /** PrtInterpreterTwoWayTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtInterpreterTwoWay,
                         sizeof(context->prtInterpreterTwoWay) );
        break;
#else
        case COLUMN_PRTINTERPRETERLANGFAMILY:
        case COLUMN_PRTINTERPRETERDEFAULTORIENTATION:
        case COLUMN_PRTINTERPRETERFEEDADDRESSABILITY:
        case COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY:
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETIN:
        case COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT:
        case COLUMN_PRTINTERPRETERTWOWAY:
        {
            uint32_t num = 0;

            if (OK != get_prtMIB_OIDValue(map, ndx, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;
        } 
        case COLUMN_PRTINTERPRETERLANGLEVEL:
        case COLUMN_PRTINTERPRETERLANGVERSION:
        case COLUMN_PRTINTERPRETERDESCRIPTION:
        case COLUMN_PRTINTERPRETERVERSION:
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
                     "prtInterpreterTable_get_value\n");
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * find a row with the given set of indexes
 */
const prtInterpreterTable_context *prtInterpreterTable_get_by_idx(
                                                            netsnmp_index *hdr)
{
    return (const prtInterpreterTable_context *)
        CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtInterpreterTable_context *prtInterpreterTable_create_row(netsnmp_index *hdr)
{
    prtInterpreterTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtInterpreterTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR, "%s: context memory allocation failed.\n", __func__);
        return NULL;
    }

    /** parse indexes */
    if (prtInterpreterTable_extract_index(ctx, hdr)) {
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
int prtInterpreterTable_extract_index(
                        prtInterpreterTable_context *ctx, netsnmp_index *hdr)
{
    netsnmp_variable_list var_hrDeviceIndex;
    netsnmp_variable_list var_prtInterpreterIndex;
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
    var_hrDeviceIndex.next_variable = &var_prtInterpreterIndex;

    memset(&var_prtInterpreterIndex, 0x00, 
                                        sizeof(var_prtInterpreterIndex));
    var_prtInterpreterIndex.type = ASN_INTEGER;
    var_prtInterpreterIndex.next_variable = NULL;

    /** parse the oid into the individual index components */
    err = parse_oid_indexes(hdr->oids, hdr->len, &var_hrDeviceIndex);
    if (err == SNMP_ERR_NOERROR) {
        /** copy index components into the context structure */
        if (snmp_clone_mem((void *) &ctx->index.oids,
                                        hdr->oids, hdr->len * sizeof(oid))) {
            return SNMP_ERR_GENERR;
        }
        ctx->index.len = hdr->len;
        //ctx->prtInterpreterIndex = *var_prtInterpreterIndex.val.integer;
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers(&var_hrDeviceIndex);

    return err;
}


