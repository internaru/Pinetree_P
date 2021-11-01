/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * hrPrinterTable Printer MIB
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
#include "hrPrinterTable.h"

#include "PrintJobInfo.h"
#include "pjl_extra_api.h"
#include "print_events_common_api.h"

#include "db_api.h"
#include "memAPI.h"

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid hrPrinterTable_oid[] = { hrPrinterTable_TABLE_OID };
size_t hrPrinterTable_oid_len = OID_LENGTH(hrPrinterTable_oid);

static int hrPrinterTable_row_copy(
        hrPrinterTable_context *, hrPrinterTable_context *);

static int hrPrinterTable_module_initialized = 0;

/************************************************************
 * populate table with startup rows
 */
void populate_hrPrinterTable(PRTMIB_OID_MAP *map)
{
    netsnmp_index hdr;
    hrPrinterTable_context *row;
    oid hrDeviceIndex = PRTMIB_THIS_HOST_DEVICE_INDEX;

    if (NULL == map)
        return;

    if (!hrPrinterTable_module_initialized)
        initialize_table_hrPrinterTable();

    hdr.len = 1;
    hdr.oids = &hrDeviceIndex;
    row = hrPrinterTable_create_row(&hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: row addition failed\n", __func__);
        return;
    }
    row->columnInfo = map;
}

/************************************************************
 * initialize the module
 */
void init_hrPrinterTable(void)
{
    initialize_table_hrPrinterTable();
    hrPrinterTable_module_initialized = 1;
}

/************************************************************
 * copy the contents of a row into another
 */
static int hrPrinterTable_row_copy(
                hrPrinterTable_context *dst, hrPrinterTable_context *src)
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
int hrPrinterTable_can_delete(hrPrinterTable_context *undo_ctx,
                            hrPrinterTable_context *row_ctx,
                            netsnmp_request_group *rg)
{
    return 0;
}


/************************************************************
 * duplicate a row
 */
hrPrinterTable_context *hrPrinterTable_duplicate_row(
                                            hrPrinterTable_context *row_ctx)
{
    hrPrinterTable_context *dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(hrPrinterTable_context);
    if (!dup)
        return NULL;
        
    if (hrPrinterTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *hrPrinterTable_delete_row(hrPrinterTable_context *ctx)
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
void hrPrinterTable_set_reserve1(netsnmp_request_group *rg)
{
    hrPrinterTable_context *row_ctx =
            (hrPrinterTable_context *) rg->existing_row;
    /*
    hrPrinterTable_context *undo_ctx =
            (hrPrinterTable_context *) rg->undo_info;
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

        case COLUMN_HRPRINTERSTATUS:
        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
            rc = netsnmp_check_vb_int_range(var, 1, 0xFFFF);
            break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "hrPrinterTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc);
        rg->status = SNMP_MAX(rg->status, current->ri->status);
    }
}

void hrPrinterTable_set_reserve2(netsnmp_request_group *rg)
{
    hrPrinterTable_context *row_ctx =
        (hrPrinterTable_context *) rg->existing_row;
    /*
    hrPrinterTable_context *undo_ctx =
        (hrPrinterTable_context *) rg->undo_info;
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

        case COLUMN_HRPRINTERSTATUS:
        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
            if(!IS_1TOFFFF(*var->val.integer))
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
void hrPrinterTable_set_action(netsnmp_request_group *rg)
{
    netsnmp_variable_list *var;
    hrPrinterTable_context *row_ctx = 
        (hrPrinterTable_context *) rg->existing_row;
    /*
    hrPrinterTable_context *undo_ctx = 
        (hrPrinterTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;

        switch (current->tri->colnum) {

        case COLUMN_HRPRINTERSTATUS:
        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
            /** INTEGER32 = ASN_INTEGER */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.integer, NULL);
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
void hrPrinterTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    hrPrinterTable_context *row_ctx = 
        (hrPrinterTable_context *) rg->existing_row;
    hrPrinterTable_context *undo_ctx = 
        (hrPrinterTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRPRINTERSTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
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
void hrPrinterTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    hrPrinterTable_context *row_ctx = 
        (hrPrinterTable_context *)rg->existing_row;
    hrPrinterTable_context *undo_ctx = 
        (hrPrinterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRPRINTERSTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
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
void hrPrinterTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /* 
    hrPrinterTable_context *row_ctx = 
        (hrPrinterTable_context *)rg->existing_row;
    hrPrinterTable_context *undo_ctx = 
        (hrPrinterTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRPRINTERSTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
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
void initialize_table_hrPrinterTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR,
                "initialize_table_hrPrinterTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);

	if (!table_info)
    {
        snmp_log(LOG_ERR, "malloc table_oinfo failed in "
                 "initialize_table_hrPrinterTable\n");
        return;
    }

    my_handler = netsnmp_create_handler_registration("hrPrinterTable",
                                             netsnmp_table_array_helper_handler,
                                             hrPrinterTable_oid,
                                             hrPrinterTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_hrPrinterTable_handler\n");
		SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = hrPrinterTable_COL_MIN;
    table_info->max_column = hrPrinterTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = hrPrinterTable_get_value;
    cb.container = netsnmp_container_find("hrPrinterTable_primary:"
                                          "hrPrinterTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod*)hrPrinterTable_duplicate_row;
    cb.delete_row = (UserRowMethod*)hrPrinterTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *)hrPrinterTable_row_copy;

    cb.can_delete = (Netsnmp_User_Row_Action *)hrPrinterTable_can_delete;

    cb.set_reserve1 = hrPrinterTable_set_reserve1;
    cb.set_reserve2 = hrPrinterTable_set_reserve2;
    cb.set_action = hrPrinterTable_set_action;
    cb.set_commit = hrPrinterTable_set_commit;
    cb.set_free = hrPrinterTable_set_free;
    cb.set_undo = hrPrinterTable_set_undo;
    DEBUGMSGTL(("initialize_table_hrPrinterTable",
                "Registering table hrPrinterTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int hrPrinterTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    //char str[255] = {0};
//    uint32_t str_sz = 255;
    uint32_t num = 0;

    DBG_VERBOSE("%s() starts x%x col %d\n", __func__, (unsigned int)table_info, table_info->colnum);
	
    netsnmp_variable_list *var = request->requestvb;
    hrPrinterTable_context *ctx = (hrPrinterTable_context *) item;
    
    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
	uint32_t ndx = map->marvell_oid_index;

	//if (ndx == INDEX_VARIABLE)
	//ndx = ctx->index.oids[1] - 1;
	ndx += 1;

    switch (table_info->colnum) {
        case COLUMN_HRPRINTERSTATUS:
			{
				int karas_status = 2;
				num = map_print_status_to_pjl(pec_get_current_printstatus());

				if(num == 10001){
					karas_status = 3;
				}
				else if(num == 10014){
					karas_status = 4;
				}
				snmp_set_var_typed_value(
						var, ASN_INTEGER, (char *) &karas_status, sizeof(karas_status));
			}
			break;
        case COLUMN_HRPRINTERDETECTEDERRORSTATE:
			{
				unsigned char karas_status[2];
				num = map_print_status_to_pjl(pec_get_current_printstatus());

				karas_status[0] = 0;
				karas_status[1] = 0;

				switch( num ){
				case 42060:  //no paper
					karas_status[0] = 64;
					break;
				case 40038:  //low toner
					karas_status[0] = 32;
					break;
				case 40039:  //no toner
					karas_status[0] = 16;
					break;
				case 42200:  //cover open
					karas_status[0] = 8;
					break;
				case 42000:  //jammed
					karas_status[0] = 4;
					break;
				case 49999:  //offline
					karas_status[0] = 2;
					break;
				case 40147:  //output full
					karas_status[1] = 8;
					break;
				}

				snmp_set_var_typed_value(
						var,ASN_OCTET_STR,(u_char*) karas_status,2);
			}
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
const hrPrinterTable_context *hrPrinterTable_get_by_idx(netsnmp_index *hdr)
{
    return (const hrPrinterTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
hrPrinterTable_context *hrPrinterTable_create_row(netsnmp_index *hdr)
{
    hrPrinterTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(hrPrinterTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(hrPrinterTable_context) failed!\n");
        return NULL;
    }

    /** check indexes */
    if (hrPrinterTable_extract_index(ctx, hdr)) {
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
int hrPrinterTable_extract_index(
                            hrPrinterTable_context *ctx, netsnmp_index *hdr)
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

