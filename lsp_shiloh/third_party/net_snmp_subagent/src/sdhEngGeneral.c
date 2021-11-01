/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * sdhEngGeneralTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

// debug headers
//#define DBG_ERR
//#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON
#define DBG_PRFX "snmp: "
#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "sdhEngGeneral.h"

#include "PrintJobInfo.h"
#include "pjl_extra_api.h"
#include "print_events_common_api.h"

#include "db_api.h"
#include "memAPI.h"

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid sdhEngGeneralTable_oid[] = { SDHENGGENERALTABLE_TABLE_OID };
size_t sdhEngGeneralTable_oid_len = OID_LENGTH(sdhEngGeneralTable_oid);

static int sdhEngGeneralTable_row_copy(
        sdhEngGeneralTable_context *, sdhEngGeneralTable_context *);

static int sdhEngGeneralTable_module_initialized = 0;

/************************************************************
 * populate table with startup rows
 */
void populate_sdhEngGeneralTable(PRTMIB_OID_MAP *map)
{
    netsnmp_index hdr;
    sdhEngGeneralTable_context *row;
	oid hrDeviceIndex = PRIVATE_SINDOH_LEAF_NODE;

    if (NULL == map)
        return;

    if (!sdhEngGeneralTable_module_initialized)
        initialize_table_sdhEngGeneralTable();

    hdr.len = 1;
    hdr.oids = &hrDeviceIndex;
    row = sdhEngGeneralTable_create_row(&hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: row addition failed\n", __func__);
        return;
    }
    row->columnInfo = map;
}

/************************************************************
 * initialize the module
 */
void init_sdhEngGeneralTable(void)
{
    initialize_table_sdhEngGeneralTable();
    sdhEngGeneralTable_module_initialized = 1;
}

/************************************************************
 * copy the contents of a row into another
 */
static int sdhEngGeneralTable_row_copy(
                sdhEngGeneralTable_context *dst, sdhEngGeneralTable_context *src)
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
    dst->columnInfo = src->columnInfo;
    return 0;
}

/************************************************************
 * the *_can_delete routine is called to determine if a row
 * can be deleted.
 *
 * return 1 if the row can be deleted
 * return 0 if the row cannot be deleted
 */
int sdhEngGeneralTable_can_delete(sdhEngGeneralTable_context *undo_ctx,
                            sdhEngGeneralTable_context *row_ctx,
                            netsnmp_request_group *rg)
{
    return 0;
}


/************************************************************
 * duplicate a row
 */
sdhEngGeneralTable_context *sdhEngGeneralTable_duplicate_row(
                                            sdhEngGeneralTable_context *row_ctx)
{
    sdhEngGeneralTable_context *dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(sdhEngGeneralTable_context);
    if (!dup)
        return NULL;
        
    if (sdhEngGeneralTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *sdhEngGeneralTable_delete_row(sdhEngGeneralTable_context *ctx)
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
void sdhEngGeneralTable_set_reserve1(netsnmp_request_group *rg)
{
    sdhEngGeneralTable_context *row_ctx =
            (sdhEngGeneralTable_context *) rg->existing_row;
    /*
    sdhEngGeneralTable_context *undo_ctx =
            (sdhEngGeneralTable_context *) rg->undo_info;
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
			case COLUMN_SDHENGSEARCHFUNCTION:
			case COLUMN_SDHENGHDDUSED:
				/*case COLUMN_ENGDRUMLIFE:*/
				rc = netsnmp_check_vb_int_range(var, 1, 0xFFFF);
				break;

			case COLUMN_SDHENGCOUNTER:
				rc = SNMP_ERR_WRONGVALUE;
				break;

			case COLUMN_SDHENGTONERLEVEL:
			case COLUMN_SDHENGHDDMAX:
				rc = netsnmp_check_vb_type_and_max_size(var, ASN_OCTET_STR, 127);
				break;

			default: /** We shouldn't get here */
				rc = SNMP_ERR_GENERR;
				snmp_log(LOG_ERR, "unknown column in "
						"sdhEngGeneralTable_set_reserve1\n");
		}

		if (rc)
			netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc);
		rg->status = SNMP_MAX(rg->status, current->ri->status);
	}
}

void sdhEngGeneralTable_set_reserve2(netsnmp_request_group *rg)
{
    sdhEngGeneralTable_context *row_ctx =
        (sdhEngGeneralTable_context *) rg->existing_row;
    /*
    sdhEngGeneralTable_context *undo_ctx =
        (sdhEngGeneralTable_context *) rg->undo_info;
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

        case COLUMN_SDHENGSEARCHFUNCTION:
        case COLUMN_SDHENGHDDUSED:
            if(!IS_1TOFFFF(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_SDHENGCOUNTER:
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_SDHENGTONERLEVEL:
        case COLUMN_SDHENGHDDMAX:
        
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
void sdhEngGeneralTable_set_action(netsnmp_request_group *rg)
{
    netsnmp_variable_list *var;
    sdhEngGeneralTable_context *row_ctx = 
        (sdhEngGeneralTable_context *) rg->existing_row;
    /*
    sdhEngGeneralTable_context *undo_ctx = 
        (sdhEngGeneralTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;

        switch (current->tri->colnum) {

	
        case COLUMN_SDHENGSEARCHFUNCTION:
        case COLUMN_SDHENGCOUNTER:
        case COLUMN_SDHENGHDDUSED:
			/** INTEGER32 = ASN_INTEGER */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.integer, NULL);
            break;

        case COLUMN_SDHENGTONERLEVEL:
        case COLUMN_SDHENGHDDMAX:
            /** OCTETSTR = ASN_OCTET_STR */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.string, NULL);
            break;

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
void sdhEngGeneralTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    sdhEngGeneralTable_context *row_ctx = 
        (sdhEngGeneralTable_context *) rg->existing_row;
    sdhEngGeneralTable_context *undo_ctx = 
        (sdhEngGeneralTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_SDHENGSEARCHFUNCTION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_SDHENGCOUNTER:
            /** SDHENGCOUNTERTC = ASN_INTEGER */
        break;

        case COLUMN_SDHENGTONERLEVEL:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDMAX:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDUSED:
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
void sdhEngGeneralTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    sdhEngGeneralTable_context *row_ctx = 
        (sdhEngGeneralTable_context *)rg->existing_row;
    sdhEngGeneralTable_context *undo_ctx = 
        (sdhEngGeneralTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_SDHENGSEARCHFUNCTION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_SDHENGCOUNTER:
            /** SDHENGCOUNTERTC = ASN_INTEGER */
        break;

        case COLUMN_SDHENGTONERLEVEL:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDMAX:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDUSED:
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
void sdhEngGeneralTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /* 
    sdhEngGeneralTable_context *row_ctx = 
        (sdhEngGeneralTable_context *)rg->existing_row;
    sdhEngGeneralTable_context *undo_ctx = 
        (sdhEngGeneralTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_SDHENGSEARCHFUNCTION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_SDHENGCOUNTER:
            /** SDHENGCOUNTERTC = ASN_INTEGER */
        break;

        case COLUMN_SDHENGTONERLEVEL:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDMAX:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_SDHENGHDDUSED:
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
void initialize_table_sdhEngGeneralTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR,
                "initialize_table_sdhEngGeneralTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
	if (!table_info)
    {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_sdhEngGeneralTable\n");
        return;
    }

    my_handler = netsnmp_create_handler_registration("sdhEngGeneralTable",
                                             netsnmp_table_array_helper_handler,
                                             sdhEngGeneralTable_oid,
                                             sdhEngGeneralTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_sdhEngGeneralTable_handler\n");
		SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
    */
    /** index: hrDeviceIndex */
    netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = SDHENGGENERALTABLE_COL_MIN;
    table_info->max_column = SDHENGGENERALTABLE_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = sdhEngGeneralTable_get_value;
    cb.container = netsnmp_container_find("sdhEngGeneralTable_primary:"
                                          "sdhEngGeneralTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod*)sdhEngGeneralTable_duplicate_row;
    cb.delete_row = (UserRowMethod*)sdhEngGeneralTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *)sdhEngGeneralTable_row_copy;

    cb.can_delete = (Netsnmp_User_Row_Action *)sdhEngGeneralTable_can_delete;

    cb.set_reserve1 = sdhEngGeneralTable_set_reserve1;
    cb.set_reserve2 = sdhEngGeneralTable_set_reserve2;
    cb.set_action = sdhEngGeneralTable_set_action;
    cb.set_commit = sdhEngGeneralTable_set_commit;
    cb.set_free = sdhEngGeneralTable_set_free;
    cb.set_undo = sdhEngGeneralTable_set_undo;
    DEBUGMSGTL(("initialize_table_sdhEngGeneralTable",
                "Registering table sdhEngGeneralTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int sdhEngGeneralTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    char str[255] = {0};
    uint32_t str_sz = 255;
    uint32_t num = 0;

    DBG_VERBOSE("%s() starts x%x col %d\n", __func__, (unsigned int)table_info, table_info->colnum);
	
    netsnmp_variable_list *var = request->requestvb;
    sdhEngGeneralTable_context *ctx = (sdhEngGeneralTable_context *) item;

    switch (table_info->colnum) {

        case COLUMN_SDHENGSERIALNUMBER:
		case COLUMN_SDHENGSEARCHFUNCTION:
        case COLUMN_SDHENGHDDUSED:
            if (OK != get_prtMIB_OIDValue(&ctx->columnInfo[table_info->colnum],
                                            0, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_COUNTER, (char *) &num, sizeof(num));
            break;
    
        			break;
		case COLUMN_SDHENGCOUNTER:
			{
				num = map_print_status_to_pjl(pec_get_current_printstatus());
	            snmp_set_var_typed_value(
            	           var, ASN_INTEGER, (char *) &num, sizeof(num));

			}
			break;
		case COLUMN_SDHENGTONERLEVEL:
			{
				int tonerLevel;
				error_type_t db_retval = FAIL;

				tonerLevel = db_get_var_int("INST_TONERCOUNT","CountInPercent",&db_retval);
				if(db_retval == FAIL){
					tonerLevel = -1;
				}
				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &tonerLevel, sizeof(tonerLevel));
			}
            break;

        case COLUMN_SDHENGHDDMAX:
			if (OK != get_prtMIB_OIDValue(&ctx->columnInfo[table_info->colnum],
						0, (void *) str, &str_sz))
				strcpy(str, PRTMIB_STR_UNIMPLEMENTED);
			snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
            break;
        default:
            snmp_log(LOG_ERR, "%s: unknown column\n", __func__);
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for a row based on a set of indexes
 */
const sdhEngGeneralTable_context *sdhEngGeneralTable_get_by_idx(netsnmp_index *hdr)
{
    return (const sdhEngGeneralTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
sdhEngGeneralTable_context *sdhEngGeneralTable_create_row(netsnmp_index *hdr)
{
    sdhEngGeneralTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(sdhEngGeneralTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(sdhEngGeneralTable_context) failed!\n");
        return NULL;
    }

    /** check indexes */
    if (sdhEngGeneralTable_extract_index(ctx, hdr)) {
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
int sdhEngGeneralTable_extract_index(
                            sdhEngGeneralTable_context *ctx, netsnmp_index *hdr)
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

