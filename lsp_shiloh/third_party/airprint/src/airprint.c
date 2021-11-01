/*
 * ============================================================================
 * (C) Copyright 2014   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** @file airprint.c 
 * 
 * NOTE: Release of this source file to SDK customers is restricted.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "ATypes.h"
#include "logger.h"
#include "net_logger.h"
#include "debug.h"
#include "lassert.h"
#include "error_types.h"
#include "ipp_api.h"
#include "net_mdns_api.h"
#include "oid_airprint_api.h"
#include "airprint_api.h"

#include "net_api.h"
#include "net_iface_vars.h"
#include "map_api.h"
#include "db_api.h"
#define DEFAULT_VENDOR "SINDOH" // Map with previous BI/CI by mspyo, 2021-03-15
//#include "trsocket.h"


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "AIR: "
#define LOGGER_MODULE_MASK  (DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IPP)

#define DBG_ERR(...)        DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...)        DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
//#define DBG_ENTER(fmt,...)  dbg_printf(DBG_PRFX "==>%s "fmt"\n", __func__ ,##__VA_ARGS__ )
//#define DBG_RETURN(fmt,...) dbg_printf(DBG_PRFX "<==%s "fmt"\n", __func__ ,##__VA_ARGS__ )

//#ifdef HAVE_CMD
//#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
//#endif // HAVE_CMD

#ifndef DBG_ERR 
    #define DBG_ERR(...)
#endif
#ifndef DBG_MSG
    #define DBG_MSG(...)
#endif
#ifndef DBG_ENTER
    #define DBG_ENTER(...)
#endif
#ifndef DBG_RETURN
    #define DBG_RETURN(...)
#endif




#define AIRPRINT_IPP_PORT            631

static ipp_inst_hndl_t g_ipp_instance = IPP_INVALID_HANDLE;


// local translation function callbacks
static int32_t ipp_printer_more_info_func(ipp_req_hndl_t request_hndl, oid_t oid, uint32_t index, char *buf, uint32_t *len);

static ipp_attribute_t g_airprint_ipp_attrs[] =
{
    {"marker-colors", OID_AIRPRINT_MARKER_COLORS, IPP_TAG_NAME_WITHOUT_LANGUAGE, NULL, IPP_GRP_DESCRIPTION},
    {"marker-high-levels", OID_AIRPRINT_MARKER_HIGH_LEVELS, IPP_TAG_INTEGER, NULL, IPP_GRP_DESCRIPTION},
    {"marker-levels", OID_AIRPRINT_MARKER_LEVELS, IPP_TAG_INTEGER, NULL, IPP_GRP_DESCRIPTION},
    {"marker-low-levels", OID_AIRPRINT_MARKER_LOW_LEVELS, IPP_TAG_INTEGER, NULL, IPP_GRP_DESCRIPTION},
    {"marker-names", OID_AIRPRINT_MARKER_NAMES, IPP_TAG_NAME_WITHOUT_LANGUAGE, NULL, IPP_GRP_DESCRIPTION},
    {"marker-types", OID_AIRPRINT_MARKER_TYPES, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
    {"pdf-versions-supported", OID_AIRPRINT_PDF_VERSIONS_SUPPORTED, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
    {"print-scaling-default", OID_AIRPRINT_PRINT_SCALING_DEFAULT, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
    {"print-scaling-supported", OID_AIRPRINT_PRINT_SCALING_SUPPORTED, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
    {"printer-kind", OID_AIRPRINT_PRINTER_KIND, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
    {"urf-supported", OID_AIRPRINT_URF_SUPPORTED, IPP_TAG_KEYWORD, NULL, IPP_GRP_DESCRIPTION},
};
static uint32_t g_airprint_ipp_attr_cnt = sizeof(g_airprint_ipp_attrs)/sizeof(ipp_attribute_t);

// these definitions override any existing default attribute definitions in the IPP module
static ipp_attribute_t g_ipp_attr_overrides[] =
{
    {"printer-more-info", OID_INVALID, IPP_TAG_URI, ipp_printer_more_info_func, IPP_GRP_DESCRIPTION},
};
static uint32_t g_ipp_attr_override_cnt = sizeof(g_ipp_attr_overrides)/sizeof(ipp_attribute_t);


void airprint_init(void)
{
    DBG_ENTER();

    XASSERT(g_ipp_instance == IPP_INVALID_HANDLE, (int)g_ipp_instance);
    g_ipp_instance = ipp_open("airprint", NULL, AIRPRINT_IPP_PORT);
    XASSERT(g_ipp_instance != IPP_INVALID_HANDLE, (int)g_ipp_instance);

    // override standard IPP attributes
    int32_t ipp_rcode;
    ipp_rcode = ipp_replace_attributes(g_ipp_instance, g_ipp_attr_overrides, g_ipp_attr_override_cnt);
    if(ipp_rcode != SYS_OK)
    {
        DBG_ERR("add attrs failed (ipp_rcode=%d)!\n", ipp_rcode);
        goto done;
    }

    // add AirPrint-specific IPP attributes 
    ipp_rcode = ipp_add_attributes(g_ipp_instance, g_airprint_ipp_attrs, g_airprint_ipp_attr_cnt);
    if(ipp_rcode != SYS_OK)
    {
        DBG_ERR("add attrs failed (ipp_rcode=%d)!\n", ipp_rcode);
        goto done;
    }

    // feature names are advertized via the ipp-features-supported attributed
    ipp_rcode = ipp_add_feature(g_ipp_instance, "airprint-1.3"); // TODO get this from a config file

done:
    DBG_RETURN();
    return;
}


static int32_t mdns_txt_if_airprint_enabled_func(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);
static int32_t mdns_txt_admin_url_func(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);

static mdns_txt_entry_t g_airprint_mdns_txt_entries[] =
{
    {"kind",        NULL,               OID_AIRPRINT_PRINTER_KIND,      mdns_txt_if_airprint_enabled_func},
    {"adminurl",    NULL,               OID_INVALID,                    mdns_txt_admin_url_func},
    {"URF",         NULL,               OID_AIRPRINT_URF_SUPPORTED,     mdns_txt_if_airprint_enabled_func},
};
static uint32_t g_airprint_mdns_txt_entry_cnt = sizeof(g_airprint_mdns_txt_entries)/sizeof(mdns_txt_entry_t);

// mdns callback 
// TODO register this as a formal callback to be called from mdns to acquire airprint-specific txt name/value pairs
void airprint_mdns_txt_add_entries(map_handle_t mdns_txt_map)
{
    mdns_txt_entry_t *entry_list = g_airprint_mdns_txt_entries;
    uint32_t entry_cnt = g_airprint_mdns_txt_entry_cnt;

    int32_t map_rcode;
    int i;
    for(i = 0; i < entry_cnt; i++) 
    {
        if(!entry_list[i].separator)
        {
            entry_list[i].separator = ","; // default separator
        }
        entry_list[i].flags = 0;
        entry_list[i].next = NULL;

        ASSERT(mdns_txt_map != MAP_INVALID_HANDLE);
        // first remove any pre-existing IPP attribute of same name
        map_remove(mdns_txt_map, entry_list[i].name, strlen(entry_list[i].name));
        map_rcode = map_insert(mdns_txt_map, entry_list[i].name, strlen(entry_list[i].name), (void *)&entry_list[i]);
        XASSERT(map_rcode == SYS_OK || map_rcode == MAP_DUP_KEY, map_rcode); 
        if(map_rcode == MAP_DUP_KEY)
        {
            // note this is not necessarily a fatal error
//            ipp_rcode = IPP_DUP_ATTR;
        }
    }

    return;
}


// translation function callback
// buf [out]
// len [in/out]
//Replace this with valid Airprint Admin URL, whenever available
// const char *g_airprint_str = "http://www.sindoh.com";
static int32_t ipp_printer_more_info_func(ipp_req_hndl_t request_hndl, oid_t oid, uint32_t index, char *buf, uint32_t *len)
{
    int32_t ret = SYS_OK;
    uint32_t out_len = 0; 
    //char hostname[IPP_MAX_HOSTNAME_SIZE];

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    if(index > 0)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }
#if 0
    ret = ipp_get_response_hostname(request_hndl, hostname, IPP_MAX_HOSTNAME_SIZE);
    if(ret != SYS_OK)
    {
        ret = SYS_FAIL;
        goto done;
    }

    if(*len < strlen(g_airprint_str) + strlen(hostname) - 2 + 1) // -2 for '%s', +1 for NULL terminator
    {
        ret = OID_ERROR_BUFFER_TOO_SMALL;
        goto done;
    }
#endif
    char *vendorName=NULL;
    error_type_t db_retval = FAIL;
    vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
    char *support_url= db_get_var("CONF_SYSTEM","WebURLSupport",&db_retval);
    // if(0!=strcmp(vendorName,DEFAULT_VENDOR))
   	if(0!=strcasecmp(vendorName, DEFAULT_VENDOR)) // by mspyo, 2021-03-15
    {

    //do nothing
    }
    else
    {
    sprintf(buf, support_url);
    }
    MEM_FREE_AND_NULL(vendorName);
    MEM_FREE_AND_NULL(support_url);
    // sprintf(buf, g_airprint_str);
    out_len = strlen(buf);

done:
    *len = out_len;
    return ret;
}

static int32_t mdns_txt_if_airprint_enabled_func(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);
    ASSERT(oid != OID_INVALID);

    int32_t ret = SYS_OK;

    error_type_t error;
    uint32_t     airprint_enabled = 0;
    error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
    UNUSED_VAR(error);
    XASSERT(SYS_OK == error, (int)error);

    if(!airprint_enabled)
    {
        *len = 0;
        buf[0] = '\0';
        ret = OID_ERROR_NOT_AVAILABLE;
        goto done;
    }

    ASSERT(oid != OID_INVALID);

    uint8_t oid_type;
    oid_type = oid_type_id_get(oid); // -1 if attr->oid == OID_INVALID

    switch(oid_type) 
    {
        case OID_TYPE_STRING:
        {
            ret = oid_get_string(oid, index, buf, *len); 
            if(ret == SYS_OK)
            {
                *len = strlen(buf);
            }
            break; 
        }
        case OID_TYPE_UINT32:
        case OID_TYPE_BYTE_ARRAY:
            // fall through -- not currently supported
        default:
            // unsupported oid_type
            XASSERT(0, oid_type);
            ret = SYS_FAIL;
            break;
    }

done:
    if(ret != SYS_OK)
    {
        *len = 0;
        buf[0] = '\0';
    }

    return ret;
}

#define AIRPRINT_MDNS_LABEL_LOCAL  ".local."
static int32_t mdns_txt_admin_url_func(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);
    XASSERT(oid == OID_INVALID, (int)oid);

    int32_t ret = SYS_OK;

    if(index > 0)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    error_type_t error;
    uint32_t     airprint_enabled = 0;
    error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
    UNUSED_VAR(error);
    XASSERT(SYS_OK == error, (int)error);

    if(!airprint_enabled)
    {
        *len = 0;
        buf[0] = '\0';
        ret = OID_ERROR_NOT_AVAILABLE;
        goto done;
    }



	#if 0
    char hostname[NET_IFACE_HOSTNAME_SIZE];
    uint32_t tmp_len;

    // TODO must account for multiple simultaneous interfaces
    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    error = net_iface_get_var_hostname(if_index, 
                                       hostname, 
                                       NET_IFACE_HOSTNAME_SIZE);
    UNUSED_VAR(error);
    XASSERT(NET_IFACE_OK == error, (int)error);
	#endif
    char *vendorName=NULL;
    error_type_t db_retval = FAIL;
    vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
    char *support_url= db_get_var("CONF_SYSTEM","WebURLSupport",&db_retval);
    // if(0!=strcmp(vendorName,DEFAULT_VENDOR))
    if(0!=strcasecmp(vendorName,DEFAULT_VENDOR)) // by mspyo, 2021-03-15
    {
    	//do nothing
    }
    else
    {
    	sprintf(buf, support_url);
    }
    MEM_FREE_AND_NULL(vendorName);
    MEM_FREE_AND_NULL(support_url);
	*len = strlen(buf);
	#if 0
	strlen(g_airprint_str)-4 + strlen(hostname) + strlen(AIRPRINT_MDNS_LABEL_LOCAL);
    if(*len > tmp_len)
    {
        sprintf(buf, g_airprint_str, hostname, AIRPRINT_MDNS_LABEL_LOCAL);
        *len = tmp_len;
    }
    else
    {
        ret = SYS_FAIL;
        goto done;
    }
	#endif

done:
    if(ret != SYS_OK)
    {
        *len = 0;
        buf[0] = '\0';
    }

    return ret;
}



//REMOVED FROM: void ipp_add_req_attr_to_job_ticket() in ipp_config_attr.c
// rdj 1/15/14 TODO: move to AirPrint extension layer in third_party
//      ASSERT(0);
//      if (ipp_job_attr_req[PRINT_SCALING].config_val_set)
//      {
//         ASSERT(ipp_job_attr_req[PRINT_SCALING].config_data_len < PRINT_SCALING_STR_LEN);
//
//         memcpy(&job_ticket->doc_processing.type.print.print_scaling,
//                ipp_job_attr_req[PRINT_SCALING].data_ptr,
//                ipp_job_attr_req[PRINT_SCALING].config_data_len);
//      }





// TODO: removed from common -- needs integration into third_party/airprint and oem config
/* 
 * 1/16/2014 proposed design for smjob ticket extensions:
 * 
 * AirPrint job attributes must be included in smjob ticket in such a way that they can be
 * applied to the job by the internal print subsystem.
 * 
 * Propose leveraging existing vnd_extensions passthrough mechanism in sm_job_api.h.
 * Define an 'smjob extensions' struct that includes callbacks for acquiring extension
 * values, e.g. for default ticket, and then for mapping or applying the settings when
 * completing the associated print job. For example, for airprint IP cleaning had to
 * remove printScaling from set_job_ticket_info() in vpi_parser.c, and this needs to
 * accounted for at some point e.g. via the callback mechanism, both for VPI as well as
 * physical printer.
 * 
 * Add an api for reqistering extensions to smjob. Use pointer to linked list of
 * extensions in the smjob ticket to support multiple extensions, since some customers
 * will need vendor extensions in addition to airprint.
 * 
 * The callbacks will simplify acquiring the ticket, e.g. via smjob_get_default_ticket(),
 * without directly exposing user to extensions.  We use a struct to encapsulate/abstract
 * the extension 'object' so we can modify it without affecting rest of smjob ticket.
 */
#if 0

smjob_rcode_t smjob_get_default_ticket(smjob_type_t job_type, smjob_ticket_t *job_ticket) {
    smjob_print_scaling_t *print_scaling; 
    rcode = smjob_get_default_print_scaling(SMJOB_TYPE_PRINT, &print_scaling);
    ASSERT(rcode == SMJOB_OK);
    ASSERT(strlen(print_scaling) < PRINT_SCALING_STR_LEN);
    strncpy(print_proc->print_scaling, print_scaling, strlen(print_scaling));
    






smjob_rcode_t smjob_get_supported_print_scaling(smjob_type_t job_type, uint32_t index, 
                                                smjob_print_scaling_t **value);


//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_supported_print_scaling(smjob_type_t          job_type, 
                                                uint32_t              index, 
                                                smjob_print_scaling_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_print_scaling(index, value);

    return sm_res;
}


//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_default_print_scaling(smjob_type_t          job_type, 
                                              smjob_print_scaling_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_print_scaling(value);

    return sm_res;
}





smjob_rcode_t smjob_oem_get_supported_print_scaling(uint32_t index, 
                                                    smjob_print_scaling_t **value);



//---------------------------------------------------------------------------------------
// Print only
smjob_rcode_t SY_ATTR_WEAK smjob_oem_get_supported_print_scaling(uint32_t              index, 
                                                    smjob_print_scaling_t **value)
{
   smjob_rcode_t sm_res = SMJOB_OK;

   if (index < g_smjob_print_scaling_table_size)
   {
      *value = g_smjob_print_scaling_table[index];
   }
   else
   {
      sm_res = SM_BAD_INDEX;
   }

   return (sm_res);
}




//---------------------------------------------------------------------------------------
// Print only
smjob_rcode_t smjob_oem_get_default_print_scaling(smjob_print_scaling_t **value)
{
   *value = g_smjob_print_scaling_table[0];

   return(SMJOB_OK);
}

#endif // 0
