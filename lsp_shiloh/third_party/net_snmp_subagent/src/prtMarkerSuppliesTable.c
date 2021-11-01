/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerSuppliesTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */
#define DBG_PRFX "snmp: "
#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtMarkerSuppliesTable.h"

#include "db_api.h"


static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtMarkerSuppliesTable_oid[] = { prtMarkerSuppliesTable_TABLE_OID };
size_t prtMarkerSuppliesTable_oid_len = OID_LENGTH(prtMarkerSuppliesTable_oid);

static int prtMarkerSuppliesTable_row_copy(
    prtMarkerSuppliesTable_context *dst, prtMarkerSuppliesTable_context *src);

static int prtMarkerSuppliesTable_module_initialized = 0;

#if 0
/************************************************************
 * get data from a row
 */
int get_prtMarkerSuppliesTable_data(
                prtMarkerSuppliesTable_context *out, unsigned long colFlags)
{
    prtMarkerSuppliesTable_context *row;
    int getAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerSuppliesTable_module_initialized) {
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
    row = (prtMarkerSuppliesTable_context *) 
                                    prtMarkerSuppliesTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtMarkerSuppliesTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERSUPPLIESINDEX:
            out->prtMarkerSuppliesIndex = row->prtMarkerSuppliesIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
            out->prtMarkerSuppliesMarkerIndex = 
                                        row->prtMarkerSuppliesMarkerIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
            out->prtMarkerSuppliesColorantIndex = 
                                        row->prtMarkerSuppliesColorantIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCLASS:
            out->prtMarkerSuppliesClass = row->prtMarkerSuppliesClass;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESTYPE:
            out->prtMarkerSuppliesType = row->prtMarkerSuppliesType;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
            memcpy((void*) out->prtMarkerSuppliesDescription,
                    (const void *) row->prtMarkerSuppliesDescription,
                    row->prtMarkerSuppliesDescription_len);
            out->prtMarkerSuppliesDescription_len = 
                    row->prtMarkerSuppliesDescription_len;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
            out->prtMarkerSuppliesSupplyUnit =
                                        row->prtMarkerSuppliesSupplyUnit;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            out->prtMarkerSuppliesMaxCapacity = 
                                        row->prtMarkerSuppliesMaxCapacity;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            out->prtMarkerSuppliesLevel = row->prtMarkerSuppliesLevel;
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
int set_prtMarkerSuppliesTable_data(
                prtMarkerSuppliesTable_context *data, unsigned long colFlags)
{
    prtMarkerSuppliesTable_context *row;
    int setAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerSuppliesTable_module_initialized) {
        snmp_log(LOG_ERR, 
                "%s: table module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerSuppliesTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtMarkerSuppliesTable_context *) 
                                    prtMarkerSuppliesTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtMarkerSuppliesTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERSUPPLIESINDEX:
            /** TODO: revisit
              * also a row index, must be consistent with index.oids[1]
              * do not allow changing this value for now
              * to change this value, another row must be created
              */
#if 0
            row->prtMarkerSuppliesIndex = data->prtMarkerSuppliesIndex;
#else
            snmp_log(LOG_ERR, 
                    "%s: cannot change marker supplies index\n", __func__);
#endif
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
            row->prtMarkerSuppliesMarkerIndex = 
                                        data->prtMarkerSuppliesMarkerIndex;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
            row->prtMarkerSuppliesColorantIndex = 
                                        data->prtMarkerSuppliesColorantIndex;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCLASS:
            row->prtMarkerSuppliesClass = data->prtMarkerSuppliesClass;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESTYPE:
            row->prtMarkerSuppliesType = data->prtMarkerSuppliesType;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
            memcpy((void*) row->prtMarkerSuppliesDescription,
                    (const void *) data->prtMarkerSuppliesDescription,
                    data->prtMarkerSuppliesDescription_len);
            row->prtMarkerSuppliesDescription_len = 
                    data->prtMarkerSuppliesDescription_len;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
            row->prtMarkerSuppliesSupplyUnit = 
                                        data->prtMarkerSuppliesSupplyUnit;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            row->prtMarkerSuppliesMaxCapacity = 
                                        data->prtMarkerSuppliesMaxCapacity;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            row->prtMarkerSuppliesLevel = data->prtMarkerSuppliesLevel;
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
int del_prtMarkerSuppliesTable_entry(long devNdx, long mrkSupNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtMarkerSuppliesTable_context *row;

    /** make sure the module is initialized */
    if (!prtMarkerSuppliesTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: table module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = mrkSupNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtMarkerSuppliesTable_context *) 
                        prtMarkerSuppliesTable_get_by_idx(&hdr);
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
int add_prtMarkerSuppliesTable_entry(prtMarkerSuppliesTable_context *info)
{
    netsnmp_index *hdr;
    prtMarkerSuppliesTable_context *row;

    /** make sure the module is initialized */
    if (!prtMarkerSuppliesTable_module_initialized)
        init_prtMarkerSuppliesTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerSuppliesTable_data(info, 0))
        return -1;

    /** make sure desired index tuple are non-existent */
    hdr = &info->index;
    row = (prtMarkerSuppliesTable_context *) 
                                    prtMarkerSuppliesTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                    __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtMarkerSuppliesTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy column data */
    if (prtMarkerSuppliesTable_row_copy(row, info)) {
        snmp_log(LOG_ERR, "%s: row content copying failed\n", __func__);
        return -1;
    }

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtMarkerSuppliesTable_data(
                prtMarkerSuppliesTable_context *data, unsigned long colFlags)
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
        snmp_log(LOG_ERR, "invalid marker supply index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtMarkerSuppliesTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERSUPPLIESINDEX:
            if (!data->prtMarkerSuppliesIndex)
                data->prtMarkerSuppliesIndex = data->index.oids[1];
            else if (data->prtMarkerSuppliesIndex != data->index.oids[1]) {
                    snmp_log(LOG_ERR, 
                                "inconsistent marker supplies index (%d)\n",
                                (int) data->prtMarkerSuppliesIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
            if (!IS_0TOFFFF(data->prtMarkerSuppliesMarkerIndex)) {
                snmp_log(LOG_ERR, "invalid marking sub-unit index (%d)\n",
                    (int) data->prtMarkerSuppliesMarkerIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
            if (!IS_0TOFFFF(data->prtMarkerSuppliesColorantIndex)) {
                snmp_log(LOG_ERR, "invalid marking colorant index (%d)\n",
                    (int) data->prtMarkerSuppliesColorantIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESCLASS:
            if (!IS_PRTMARKERSUPPLIESCLASS_VALID(
                                        data->prtMarkerSuppliesClass)) {
                snmp_log(LOG_ERR, 
                            "%s: invalid marker supply class code (%d)\n",
                            __func__, (int) data->prtMarkerSuppliesClass);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESTYPE:
            if (!IS_PRTMARKERSUPPLIESTYPE_VALID(data->prtMarkerSuppliesType)) {
                snmp_log(LOG_ERR, "%s: invalid marker supply type code (%d)\n",
                    __func__, (int) data->prtMarkerSuppliesType);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
            if (!IS_0TO255(data->prtMarkerSuppliesDescription_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid marker supply description length (%d)\n",
                    __func__, (int) data->prtMarkerSuppliesDescription_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
            if (!IS_PRTMARKERSUPPLIESSUPPLYUNIT_VALID(
                                        data->prtMarkerSuppliesSupplyUnit)) {
                snmp_log(LOG_ERR, "%s: invalid marker supply unit code (%d)\n",
                    __func__, (int) data->prtMarkerSuppliesSupplyUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerSuppliesMaxCapacity)) {
                snmp_log(LOG_ERR, " invalid supply max capacity (%d)\n",
                    (int) data->prtMarkerSuppliesMaxCapacity);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            if (!IS_NEG3TO7FFFFFFF(data->prtMarkerSuppliesLevel)) {
                snmp_log(LOG_ERR, " invalid supply max capacity (%d)\n",
                    (int) data->prtMarkerSuppliesLevel);
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
void populate_prtMarkerSuppliesTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtMarkerSuppliesTable_context *row;
    uint32_t i, numOfMarkerSupplies;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfMarkerSupplies, NULL))
        numOfMarkerSupplies = 1;

    if (!prtMarkerSuppliesTable_module_initialized)
        initialize_table_prtMarkerSuppliesTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfMarkerSupplies; i++) {

        indexes[1] = i + 1;
        row = prtMarkerSuppliesTable_create_row(&hdr);
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
void init_prtMarkerSuppliesTable(void)
{
    initialize_table_prtMarkerSuppliesTable();
    prtMarkerSuppliesTable_module_initialized = 1;
}

/************************************************************
 * copy row contents
 */
static int prtMarkerSuppliesTable_row_copy(
                                    prtMarkerSuppliesTable_context *dst, 
                                    prtMarkerSuppliesTable_context *src)
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
    

    /** copy components into the context structure */
#if 0
    dst->prtMarkerSuppliesIndex = src->prtMarkerSuppliesIndex;
    dst->prtMarkerSuppliesMarkerIndex = src->prtMarkerSuppliesMarkerIndex;
    dst->prtMarkerSuppliesColorantIndex = src->prtMarkerSuppliesColorantIndex;
    dst->prtMarkerSuppliesClass = src->prtMarkerSuppliesClass;
    dst->prtMarkerSuppliesType = src->prtMarkerSuppliesType;
    memcpy( dst->prtMarkerSuppliesDescription, 
            src->prtMarkerSuppliesDescription, 
            src->prtMarkerSuppliesDescription_len );
    dst->prtMarkerSuppliesDescription_len = 
            src->prtMarkerSuppliesDescription_len;
    dst->prtMarkerSuppliesSupplyUnit = src->prtMarkerSuppliesSupplyUnit;
    dst->prtMarkerSuppliesMaxCapacity = src->prtMarkerSuppliesMaxCapacity;
    dst->prtMarkerSuppliesLevel = src->prtMarkerSuppliesLevel;
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
int prtMarkerSuppliesTable_can_delete(
                                prtMarkerSuppliesTable_context *undo_ctx,
                                prtMarkerSuppliesTable_context *row_ctx,
                                netsnmp_request_group *rg)
{
    return 0;
}


/************************************************************
 * the *_duplicate row routine
 */
prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_duplicate_row(
                                    prtMarkerSuppliesTable_context *row_ctx)
{
    prtMarkerSuppliesTable_context * dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(prtMarkerSuppliesTable_context);
    if(!dup)
        return NULL;
        
    if (prtMarkerSuppliesTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * the *_delete_row method is called to delete a row.
 */
netsnmp_index *prtMarkerSuppliesTable_delete_row(
                                        prtMarkerSuppliesTable_context *ctx )
{
    if (ctx->index.oids)
        free(ctx->index.oids);
    free( ctx );

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
void prtMarkerSuppliesTable_set_reserve1( netsnmp_request_group *rg )
{
    prtMarkerSuppliesTable_context *row_ctx =
            (prtMarkerSuppliesTable_context *)rg->existing_row;
    /*
    prtMarkerSuppliesTable_context *undo_ctx =
            (prtMarkerSuppliesTable_context *)rg->undo_info;
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

#if 0
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            rc = netsnmp_check_vb_int_range(var, -2, 0x7FFFFFFF);
            break;

        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            rc = netsnmp_check_vb_int_range(var, -3, 0x7FFFFFFF);
            break;
#else
		/* added checking for read-only */
		case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
		case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
		case COLUMN_PRTMARKERSUPPLIESCLASS:
		case COLUMN_PRTMARKERSUPPLIESTYPE:
		case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
		case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
		case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
		case COLUMN_PRTMARKERSUPPLIESLEVEL:
			printf("Read Only\n");
			rc = SNMP_ERR_READONLY; 
			break;			
#endif
				

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "prtMarkerSuppliesTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc );
        rg->status = SNMP_MAX( rg->status, current->ri->status );
    }
}

void prtMarkerSuppliesTable_set_reserve2( netsnmp_request_group *rg )
{
    prtMarkerSuppliesTable_context *row_ctx = 
            (prtMarkerSuppliesTable_context *)rg->existing_row;
    /*
    prtMarkerSuppliesTable_context *undo_ctx = 
            (prtMarkerSuppliesTable_context *)rg->undo_info;
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

        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            if (!IS_NEG2TO7FFFFFFF(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            if (!IS_NEG3TO7FFFFFFF(*var->val.integer))
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
void prtMarkerSuppliesTable_set_action( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    prtMarkerSuppliesTable_context *row_ctx = 
                (prtMarkerSuppliesTable_context *)rg->existing_row;
    /*
    prtMarkerSuppliesTable_context *undo_ctx = 
                (prtMarkerSuppliesTable_context *)rg->undo_info;
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
            case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
                /** INTEGER32 = ASN_INTEGER */
                row_ctx->prtMarkerSuppliesMaxCapacity = *var->val.integer;
                break;

            case COLUMN_PRTMARKERSUPPLIESLEVEL:
                /** INTEGER32 = ASN_INTEGER */
                row_ctx->prtMarkerSuppliesLevel = *var->val.integer;
                break;
#else
            case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            case COLUMN_PRTMARKERSUPPLIESLEVEL:
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
void prtMarkerSuppliesTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtMarkerSuppliesTable_context *row_ctx = 
                (prtMarkerSuppliesTable_context *)rg->existing_row;
    prtMarkerSuppliesTable_context *undo_ctx = 
                (prtMarkerSuppliesTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            /** INTEGER32 = ASN_INTEGER */
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
void prtMarkerSuppliesTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtMarkerSuppliesTable_context *row_ctx = 
                (prtMarkerSuppliesTable_context *)rg->existing_row;
    prtMarkerSuppliesTable_context *undo_ctx = 
                (prtMarkerSuppliesTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            /** INTEGER32 = ASN_INTEGER */
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
void prtMarkerSuppliesTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtMarkerSuppliesTable_context *row_ctx = 
                (prtMarkerSuppliesTable_context *)rg->existing_row;
    prtMarkerSuppliesTable_context *undo_ctx = 
                (prtMarkerSuppliesTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            /** INTEGER32 = ASN_INTEGER */
        break;

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }
    }
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtMarkerSuppliesTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR, 
            "initialize_table_prtMarkerSuppliesTable_handler called again\n");
        return;
    }
    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info) {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtMarkerSuppliesTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtMarkerSuppliesTable",
                                             netsnmp_table_array_helper_handler,
                                             prtMarkerSuppliesTable_oid,
                                             prtMarkerSuppliesTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtMarkerSuppliesTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
        /** index: prtMarkerSuppliesIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtMarkerSuppliesTable_COL_MIN;
    table_info->max_column = prtMarkerSuppliesTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtMarkerSuppliesTable_get_value;
    cb.container = netsnmp_container_find("prtMarkerSuppliesTable_primary:"
                                          "prtMarkerSuppliesTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod *) 
        prtMarkerSuppliesTable_duplicate_row;
    cb.delete_row = (UserRowMethod *) 
        prtMarkerSuppliesTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *) 
        prtMarkerSuppliesTable_row_copy;
    cb.can_delete = (Netsnmp_User_Row_Action *)     
        prtMarkerSuppliesTable_can_delete;

    cb.set_reserve1 = prtMarkerSuppliesTable_set_reserve1;
    cb.set_reserve2 = prtMarkerSuppliesTable_set_reserve2;
    cb.set_action = prtMarkerSuppliesTable_set_action;
    cb.set_commit = prtMarkerSuppliesTable_set_commit;
    cb.set_free = prtMarkerSuppliesTable_set_free;
    cb.set_undo = prtMarkerSuppliesTable_set_undo;
    DEBUGMSGTL(("initialize_table_prtMarkerSuppliesTable",
                "Registering table prtMarkerSuppliesTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int prtMarkerSuppliesTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    netsnmp_variable_list *var = request->requestvb;
    prtMarkerSuppliesTable_context *ctx = 
        (prtMarkerSuppliesTable_context *) item;

    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
    uint32_t ndx = map->marvell_oid_index;

    if (ndx == INDEX_VARIABLE)
        ndx = ctx->index.oids[1] - 1;

    switch(table_info->colnum) {

#if 0
        case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesMarkerIndex,
                         sizeof(context->prtMarkerSuppliesMarkerIndex) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesColorantIndex,
                         sizeof(context->prtMarkerSuppliesColorantIndex) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESCLASS:
            /** PrtMarkerSuppliesClassTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesClass,
                         sizeof(context->prtMarkerSuppliesClass) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESTYPE:
            /** PrtMarkerSuppliesTypeTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesType,
                         sizeof(context->prtMarkerSuppliesType) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
            /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtMarkerSuppliesDescription,
                         context->prtMarkerSuppliesDescription_len );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
            /** PrtMarkerSuppliesSupplyUnitTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesSupplyUnit,
                         sizeof(context->prtMarkerSuppliesSupplyUnit) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesMaxCapacity,
                         sizeof(context->prtMarkerSuppliesMaxCapacity) );
        break;
    
        case COLUMN_PRTMARKERSUPPLIESLEVEL:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSuppliesLevel,
                         sizeof(context->prtMarkerSuppliesLevel) );
        break;
#else
        case COLUMN_PRTMARKERSUPPLIESMARKERINDEX:
        case COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX:
        case COLUMN_PRTMARKERSUPPLIESCLASS:
        case COLUMN_PRTMARKERSUPPLIESTYPE:
        case COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT:
        // case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
        // case COLUMN_PRTMARKERSUPPLIESLEVEL:
        {
            uint32_t num = 0;

            if (OK != get_prtMIB_OIDValue(map, ndx, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;
        }
        case COLUMN_PRTMARKERSUPPLIESMAXCAPACITY:
		{	
			int maxCapacity = 100;
			snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &maxCapacity, sizeof(maxCapacity));
		}
        break;        
        case COLUMN_PRTMARKERSUPPLIESLEVEL:
		{
			int tonerLevel;
			error_type_t db_retval = FAIL;

			tonerLevel = db_get_var_int("INST_TONERCOUNT","CountInPercent",&db_retval);
			if(db_retval == FAIL){
				tonerLevel = -2;
			}
			snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &tonerLevel, sizeof(tonerLevel));
		}
        break;
        
        case COLUMN_PRTMARKERSUPPLIESDESCRIPTION:
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
                     "prtMarkerSuppliesTable_get_value\n");
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for a row based on a set of indexes
 */
const prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_get_by_idx(
                                                            netsnmp_index *hdr)
{
    return (const prtMarkerSuppliesTable_context *)
                                            CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_create_row(
                                                            netsnmp_index *hdr)
{
    prtMarkerSuppliesTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtMarkerSuppliesTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR, "%s: context memory allocation failed.\n", __func__);
        return NULL;
    }

    /** parse indexes */
    if (prtMarkerSuppliesTable_extract_index(ctx, hdr)) {
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
int prtMarkerSuppliesTable_extract_index(
                        prtMarkerSuppliesTable_context *ctx, netsnmp_index *hdr)
{
    netsnmp_variable_list var_hrDeviceIndex;
    netsnmp_variable_list var_prtMarkerSuppliesIndex;
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
    var_hrDeviceIndex.next_variable = &var_prtMarkerSuppliesIndex;

    memset(&var_prtMarkerSuppliesIndex, 0x00, 
                                        sizeof(var_prtMarkerSuppliesIndex));
    var_prtMarkerSuppliesIndex.type = ASN_INTEGER;
    var_prtMarkerSuppliesIndex.next_variable = NULL;

    /** parse the oid into the individual index components */
    err = parse_oid_indexes(hdr->oids, hdr->len, &var_hrDeviceIndex);
    if (err == SNMP_ERR_NOERROR) {
        /** copy index components into the context structure */
        if (snmp_clone_mem((void *) &ctx->index.oids,
                                        hdr->oids, hdr->len * sizeof(oid))) {
            return SNMP_ERR_GENERR;
        }
        ctx->index.len = hdr->len;
        //ctx->prtMarkerSuppliesIndex = *var_prtMarkerSuppliesIndex.val.integer;
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers( &var_hrDeviceIndex );

    return err;
}



