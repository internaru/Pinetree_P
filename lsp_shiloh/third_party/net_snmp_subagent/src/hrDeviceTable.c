/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * hrDeviceTable Printer MIB
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
#include "hrDeviceTable.h"

#include "db_api.h"
#include "memAPI.h"

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid hrDeviceTable_oid[] = { hrDeviceTable_TABLE_OID };
size_t hrDeviceTable_oid_len = OID_LENGTH(hrDeviceTable_oid);

static int hrDeviceTable_row_copy(
        hrDeviceTable_context *, hrDeviceTable_context *);

static int hrDeviceTable_module_initialized = 0;

/************************************************************
 * populate table with startup rows
 */
void populate_hrDeviceTable(PRTMIB_OID_MAP *map)
{
    netsnmp_index hdr;
    hrDeviceTable_context *row;
    oid hrDeviceIndex = PRTMIB_THIS_HOST_DEVICE_INDEX;
	uint32_t i, numOfDescrs = 5;

    if (NULL == map)
        return;

    if (!hrDeviceTable_module_initialized)
        initialize_table_hrDeviceTable();

    hdr.len = 1;
    hdr.oids = &hrDeviceIndex;
	//printf("\\\\\\\\\\\\\\\\\\###################1\n");

    for (i = 0; i < numOfDescrs; i++) {

		//printf("\\\\\\\\\\\\\\\\\\####################%d\n",i);
		hrDeviceIndex = i+1;
    	row = hrDeviceTable_create_row(&hdr);
	    if (!row || CONTAINER_INSERT(cb.container, row)) {
	        snmp_log(LOG_ERR, "%s: row addition failed\n", __func__);
	        return;
	    }
    	row->columnInfo = map;
	}
	//printf("\\\\\\\\\\\\\\\\\\###################1\n");
}

/************************************************************
 * initialize the module
 */
void init_hrDeviceTable(void)
{
    initialize_table_hrDeviceTable();
    hrDeviceTable_module_initialized = 1;
}

/************************************************************
 * copy the contents of a row into another
 */
static int hrDeviceTable_row_copy(
                hrDeviceTable_context *dst, hrDeviceTable_context *src)
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
int hrDeviceTable_can_delete(hrDeviceTable_context *undo_ctx,
                            hrDeviceTable_context *row_ctx,
                            netsnmp_request_group *rg)
{
    return 0;
}


/************************************************************
 * duplicate a row
 */
hrDeviceTable_context *hrDeviceTable_duplicate_row(
                                            hrDeviceTable_context *row_ctx)
{
    hrDeviceTable_context *dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(hrDeviceTable_context);
    if (!dup)
        return NULL;
        
    if (hrDeviceTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *hrDeviceTable_delete_row(hrDeviceTable_context *ctx)
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
void hrDeviceTable_set_reserve1(netsnmp_request_group *rg)
{
    hrDeviceTable_context *row_ctx =
            (hrDeviceTable_context *) rg->existing_row;
    /*
    hrDeviceTable_context *undo_ctx =
            (hrDeviceTable_context *) rg->undo_info;
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

        case COLUMN_HRDEVICEID:
        case COLUMN_HRDEVICESTATUS:
        case COLUMN_HRDEVICEERRORS:
            rc = netsnmp_check_vb_int_range(var, 1, 0xFFFF);
            break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "hrDeviceTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc);
        rg->status = SNMP_MAX(rg->status, current->ri->status);
    }
}

void hrDeviceTable_set_reserve2(netsnmp_request_group *rg)
{
    hrDeviceTable_context *row_ctx =
        (hrDeviceTable_context *) rg->existing_row;
    /*
    hrDeviceTable_context *undo_ctx =
        (hrDeviceTable_context *) rg->undo_info;
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

        case COLUMN_HRDEVICEID:
        case COLUMN_HRDEVICESTATUS:
        case COLUMN_HRDEVICEERRORS:
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
void hrDeviceTable_set_action(netsnmp_request_group *rg)
{
    netsnmp_variable_list *var;
    hrDeviceTable_context *row_ctx = 
        (hrDeviceTable_context *) rg->existing_row;
    /*
    hrDeviceTable_context *undo_ctx = 
        (hrDeviceTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;

        switch (current->tri->colnum) {

        case COLUMN_HRDEVICEID:
        case COLUMN_HRDEVICESTATUS:
            /** INTEGER32 = ASN_INTEGER */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.integer, NULL);
            break;

		case COLUMN_HRDEVICEERRORS:
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
void hrDeviceTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    hrDeviceTable_context *row_ctx = 
        (hrDeviceTable_context *) rg->existing_row;
    hrDeviceTable_context *undo_ctx = 
        (hrDeviceTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRDEVICEID:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICESTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICEERRORS:
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
void hrDeviceTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    hrDeviceTable_context *row_ctx = 
        (hrDeviceTable_context *)rg->existing_row;
    hrDeviceTable_context *undo_ctx = 
        (hrDeviceTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRDEVICEID:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICESTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICEERRORS:
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
void hrDeviceTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /* 
    hrDeviceTable_context *row_ctx = 
        (hrDeviceTable_context *)rg->existing_row;
    hrDeviceTable_context *undo_ctx = 
        (hrDeviceTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_HRDEVICEID:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICESTATUS:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_HRDEVICEERRORS:
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
void initialize_table_hrDeviceTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR,
                "initialize_table_hrDeviceTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info)
    {
        snmp_log(LOG_ERR, "malloc table_oinfo failed in "
                 "initialize_table_hrDeviceTable\n");
        return;
    }

    my_handler = netsnmp_create_handler_registration("hrDeviceTable",
                                             netsnmp_table_array_helper_handler,
                                             hrDeviceTable_oid,
                                             hrDeviceTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_hrDeviceTable_handler\n");
		SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = hrDeviceTable_COL_MIN;
    table_info->max_column = hrDeviceTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = hrDeviceTable_get_value;
    cb.container = netsnmp_container_find("hrDeviceTable_primary:"
                                          "hrDeviceTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod*)hrDeviceTable_duplicate_row;
    cb.delete_row = (UserRowMethod*)hrDeviceTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *)hrDeviceTable_row_copy;

    cb.can_delete = (Netsnmp_User_Row_Action *)hrDeviceTable_can_delete;

    cb.set_reserve1 = hrDeviceTable_set_reserve1;
    cb.set_reserve2 = hrDeviceTable_set_reserve2;
    cb.set_action = hrDeviceTable_set_action;
    cb.set_commit = hrDeviceTable_set_commit;
    cb.set_free = hrDeviceTable_set_free;
    cb.set_undo = hrDeviceTable_set_undo;
    DEBUGMSGTL(("initialize_table_hrDeviceTable",
                "Registering table hrDeviceTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int hrDeviceTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    char str[255] = {0};
//    uint32_t str_sz = 255;
    uint32_t num = 0;

    DBG_VERBOSE("%s() starts x%x col %d\n", __func__, (unsigned int)table_info, table_info->colnum);
	
    netsnmp_variable_list *var = request->requestvb;
    hrDeviceTable_context *ctx = (hrDeviceTable_context *) item;
    
    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
	uint32_t ndx = map->marvell_oid_index;


	//if (ndx == INDEX_VARIABLE)
	//ndx = ctx->index.oids[1] - 1;
	ndx = ctx->index.oids[0];
	//ndx += 1;

//	printf("##########################ctx%d, table%d, ndx%d\n",ctx->columnInfo->marvell_oid_index,table_info->colnum,ndx);//table_info->colnum.marvell_oid_index,ndx);

    switch (table_info->colnum) {

        case COLUMN_HRDEVICEINDEX:
			{
				num = ndx;
				snmp_set_var_typed_value(
						var, ASN_INTEGER, (char *) &num, sizeof(num));

			}
			break;
        case COLUMN_HRDEVICETYPE:
			{
				static oid  HRDEVICEOTHER_OID[] = {1, 3, 6, 1, 2, 1, 25, 3, 1, 1 };
				static oid  HRDEVICEPRINTER_OID[] = {1, 3, 6, 1, 2, 1, 25, 3, 1, 5};
				
				switch( ndx ){
				case 1:  
					snmp_set_var_typed_value(
							var,ASN_OBJECT_ID,(u_char*) HRDEVICEPRINTER_OID,10 * sizeof(oid));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					snmp_set_var_typed_value(
							var,ASN_OBJECT_ID,(u_char*) HRDEVICEOTHER_OID,10 * sizeof(oid));
					break;
				}
			}
			break;
        case COLUMN_HRDEVICEDESCR:
			{
				//jeseo
				static char modelName[64];
				//if (modelName == NULL)
				//{
				
				//modelName = (char *)malloc(64*sizeof(char));
				strcpy(modelName, "Printer");
				FILE *fp;
				fp = popen("/app/karas_pinetree_util.sh --get-product-string", "r");
				if (fp == NULL)
				{
					perror("erro : ");
					printf("%s[%d] use default sysdecr\n", __FILE__, __LINE__);
				}
				else
				{
					fgets(modelName, 64, fp);
					if (pclose(fp) != 0)
					{
						printf("%s[%d] use default sysdecr\n", __FILE__, __LINE__);
					}
					else
					{
						modelName[strlen(modelName) - 1] = '\0';
					}
				}
				//}

				switch( ndx ){
					case 1:
						//sprintf( str, "Print" );
						strcpy(str, modelName);
						break;
					case 2:
						sprintf( str, "System Platform" );
						break;
					case 3:
						sprintf( str, "Copier" );
						break;
					case 4:
						sprintf( str, "Fax" );
						break;
					case 5:
						sprintf( str, "Scanner" );
						break;
				}
            	snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
			}
			break;
        case COLUMN_HRDEVICEID:
			{
				sprintf( str, "DeviceID complete" );
            	snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
			}
			break;
        case COLUMN_HRDEVICESTATUS:
        	{
				// num = 1;
        		num = 2; // Edited by mspyo, 2016-09-23. for SAP LPD/SNMP issue.
				snmp_set_var_typed_value(
						var, ASN_INTEGER, (char *) &num, sizeof(num));
			}
			break;
        case COLUMN_HRDEVICEERRORS:
			{
				num = 0;
				snmp_set_var_typed_value(
						var, ASN_INTEGER, (char *) &num, sizeof(num));
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
const hrDeviceTable_context *hrDeviceTable_get_by_idx(netsnmp_index *hdr)
{
    return (const hrDeviceTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
hrDeviceTable_context *hrDeviceTable_create_row(netsnmp_index *hdr)
{
    hrDeviceTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(hrDeviceTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(hrDeviceTable_context) failed!\n");
        return NULL;
    }

    /** check indexes */
    if (hrDeviceTable_extract_index(ctx, hdr)) {
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
int hrDeviceTable_extract_index(
                            hrDeviceTable_context *ctx, netsnmp_index *hdr)
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

