/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// system includes
#include <stdio.h> 
#include <string.h>

// common headers
#include "error_types.h" // OK
#include "os_system.h" // os_system()
#include "net_iface_config.h" 
#include "memAPI.h"
#include "uuid.h" // get_device_uuid_string()
#include "map_api.h"
#include "net_api.h" // common net api
#include "oid_api.h" // oid_t
#include "oid_sm_api.h"
#include "net_mdns_api.h" // format_mdns_conf()
#include "net_conf_api.h" // Bonjour_e
#include "net_intf_api.h" // get_intf_mac_addr()
#include "db_api.h"	//for serialnumber TEMPTEMP
#include "spec.h"

// Debug configuration
#define DBG_PRFX "net.mdns.conf: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

//=========================================================================================
// Defines
//=========================================================================================

// The base string for the Airprint local address.
//Replace this with valid Airprint admin URL string,whenever available
#define AIRPRINT_BASE_URL_STR "http://www.sindoh.com"
#define AIRPRINT_BASE_URL_STR_LEN (sizeof(AIRPRINT_BASE_URL_STR) - 2)

#define FRIENDLY_NAME_MAX_SIZE      64
#define MFG_NAME_MAX_SIZE           64
#define MODEL_NAME_MAX_SIZE         64
#define IPV4_ADDRESS_MAX_SIZE       16
#define MAC_ADDRESS_LENGTH          17
#define UUID_FRIENDLY_LENGTH        37
#define DEVICE_ID_LOCATION_LENGTH       256 // copied from device_identification_vars.h

#define DEFAULT_VENDOR "SINDOH" // Map with previous BI/CI by mspyp, 2021-03-15

#define USB_MFG_KEY     "usb_MFG"
#define USB_MDL_KEY     "usb_MDL"
#define MFG_KEY         "mfg"
#define MDL_KEY         "mdl"
#define ADMINURL_KEY    "adminurl="
#define TY_KEY          "ty"
#define PRODUCT_KEY     "product"
#define MAC_KEY         "mac"
#define UUID_KEY        "UUID"

#define MDNS_PRINTER_SERVICE_NAME "ipp/print" /* see also net_mdns_oem_config.h */

#define MDNS_TXT_RP              MDNS_PRINTER_SERVICE_NAME       /* Max 40 chars */
#define MDNS_TXT_QTOTAL          "1"                             /* Max 2 chars */
#define MDNS_TXT_PRIORITY        "40"                            /* Max 2 chars */
#define MDNS_TXT_USB_CMD         "ZJ/URF"                        /* Max 12 chars */
#define MDNS_TXT_USB_CMD_NO_AP   "ZJ"                            /* Max 12 chars */
#define MDNS_TXT_TRANSPARENT     "T"                             /* Max 1 char */
#define MDNS_TXT_BINARY          "T"                             /* Max 1 char */
#define MDNS_TXT_PAPERCUSTOM     "F"                             /* Max 1 char */


#define FEATURE_SUPPORTED     "T"
#define FEATURE_NOT_SUPPORTED "F"

// How long the feature setting string length will be, T or F plus null.
#define FEATURE_SETTING_STR_LEN 2

#define MDNS_TXT_BIND_STR "Bind="
#define MDNS_TXT_BIND_STR_LEN (sizeof(MDNS_TXT_BIND_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_COLLATE_STR "Collate="
#define MDNS_TXT_COLLATE_STR_LEN (sizeof(MDNS_TXT_COLLATE_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_COLOR_STR "Color="
#define MDNS_TXT_COLOR_STR_LEN (sizeof(MDNS_TXT_COLOR_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_COPIES_STR "Copies="
#define MDNS_TXT_COPIES_STR_LEN (sizeof(MDNS_TXT_COPIES_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_DUPLEX_STR "Duplex="
#define MDNS_TXT_DUPLEX_STR_LEN (sizeof(MDNS_TXT_DUPLEX_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_PUNCH_STR "Punch="
#define MDNS_TXT_PUNCH_STR_LEN (sizeof(MDNS_TXT_PUNCH_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_SCAN_STR "Scan="
#define MDNS_TXT_SCAN_STR_LEN (sizeof(MDNS_TXT_SCAN_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_FAX_STR "Fax="
#define MDNS_TXT_FAX_STR_LEN (sizeof(MDNS_TXT_FAX_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_SORT_STR "Sort="
#define MDNS_TXT_SORT_STR_LEN (sizeof(MDNS_TXT_SORT_STR) + FEATURE_SETTING_STR_LEN)

#define MDNS_TXT_STAPLE_STR "Staple="
#define MDNS_TXT_STAPLE_STR_LEN (sizeof(MDNS_TXT_STAPLE_STR) + FEATURE_SETTING_STR_LEN)

#define ADMIN_URL_MAX_SIZE (IPV4_ADDRESS_MAX_SIZE + AIRPRINT_BASE_URL_STR_LEN + \
                            sizeof(ADMINURL_KEY))

// The maximum recommended TXT record is 512 bytes long.
#define MAX_REC_TXT_RECORD_LEN (512)

// Set the maximum string allowed here.
#define MAX_PDL_STR_LEN (1 * MAX_REC_TXT_RECORD_LEN)

// Define this to be 128 bytes. That should cover it.
#define MAX_URF_STR_LEN 128

// Define this to be 64 bytes. That should cover all the kinds
#define MAX_PRINTER_KIND_STR_LEN 64

#define USB_CMD_STR "usb_CMD="
#define MAX_USB_CMD_VALUE_STR_LEN 12
#define USB_CMD_STR_LEN (sizeof(USB_CMD_STR) + MAX_USB_CMD_VALUE_STR_LEN)

static void update_vendor_Name();
char ipp_oid_vendor_name[55];
//#define MDNS_DEBUG_HEX_DUMP

//=========================================================================================
// Typedefs
//=========================================================================================
typedef error_type_t (*TXT_RECORD_HANDLER)(int index, char** txt_record);
typedef error_type_t (*SUB_TYPE_HANDLER)(int index, char** txt_record);

typedef struct
{
    const char* service;
    TXT_RECORD_HANDLER txt_handler;
    uint32_t (*func_get_num_sub_types)(void);
    SUB_TYPE_HANDLER   sub_type_handler;

} net_mdns_config_handler_map_t;


/*
 * rdj added 2/2014
 */

// public API
void update_vendor_Name()
{
	char *vendorName=NULL;
	char *ModelName=NULL;
	error_type_t db_retval = FAIL;
	vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
	ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
	// if(0==strcmp(vendorName,DEFAULT_VENDOR))
	if(0==strcasecmp(vendorName, DEFAULT_VENDOR)) // by mspyo, 2021-03-15
	{
	 int i=1;
	 for(;i<=5;i++)
	 {
	 	vendorName[i]-=32;
	 }
	}
	sprintf(ipp_oid_vendor_name,"%s %s",vendorName,ModelName);
	MEM_FREE_AND_NULL(vendorName);
	MEM_FREE_AND_NULL(ModelName);
}

int32_t mdns_txt_add_entries(mdns_txt_entry_t *entry_list, uint32_t entry_cnt);

// private

#define MAX_INT_STR_LEN 12 // maximum string length of a signed int, 10 digits + 1 sign + 1 NULL terminator

// private routines
static int32_t mdns_txt_insert_entries(mdns_txt_entry_t *entry_list, uint32_t entry_cnt, uint32_t flags);

// Translate boolean integer to string 'T' or 'F'
static int32_t mdns_txt_int_to_bool_str(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);

static int32_t mdns_ipp_txt_pdl_list(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);
static int32_t mdns_ipp_txt_product(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);
static int32_t mdns_ipp_txt_mac(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);
static int32_t mdns_ipp_txt_uuid(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len);

static mdns_txt_entry_t g_mdns_txt_entries_ipp[] =
{
    {"txtvers",     "1",                OID_INVALID,                    NULL},
    {"qtotal",      MDNS_TXT_QTOTAL,    OID_INVALID,                    NULL},
    {"pdl",         NULL,               OID_INVALID,                    mdns_ipp_txt_pdl_list},
    {"rp",          MDNS_TXT_RP,        OID_INVALID,                    NULL},
    {"ty",          ipp_oid_vendor_name,      OID_INVALID, 				NULL},
    {"product",     NULL,               OID_INVALID,                    mdns_ipp_txt_product},
    {"usb_MFG",     ipp_oid_vendor_name,      OID_INVALID,    			NULL},
    {"usb_MDL",     ipp_oid_vendor_name,      OID_INVALID, 				NULL},
    {"priority",    MDNS_TXT_PRIORITY,  OID_INVALID,                    NULL},
    {"mac",         NULL,               OID_INVALID,                    mdns_ipp_txt_mac},
    {"adminurl",    NULL,               OID_SM_SYSTEM_MORE_INFO,        NULL},
    {"UUID",        NULL,               OID_INVALID,                    mdns_ipp_txt_uuid},
    {"Transparent", MDNS_TXT_TRANSPARENT, OID_INVALID,                  NULL},
    {"Binary",      MDNS_TXT_BINARY,    OID_INVALID,                    NULL},
    {"Bind",        NULL,               OID_PRINT_BIND_SUPPORT,         mdns_txt_int_to_bool_str},
    {"Collate",     NULL,               OID_PRINT_COLLATE_SUPPORT,      mdns_txt_int_to_bool_str},
    {"Color",       NULL,               OID_PRINT_HAVE_COLOR,           mdns_txt_int_to_bool_str},
    {"Copies",      NULL,               OID_SCAN_SUPPORT,               mdns_txt_int_to_bool_str},
    {"Duplex",      NULL,               OID_DUPLEXER_SUPPORT,           mdns_txt_int_to_bool_str},
    {"PaperCustom", MDNS_TXT_PAPERCUSTOM, OID_INVALID,                  NULL},
    {"PaperMax",    "legal-A4",         OID_INVALID,                    NULL},
    {"Punch",       "F",                OID_INVALID,                    NULL},
    {"Scan",        NULL,               OID_SCAN_SUPPORT,               mdns_txt_int_to_bool_str},
    {"Fax",         NULL,               OID_FAX_SUPPORT,                mdns_txt_int_to_bool_str},
    {"Sort",        NULL,               OID_PRINT_SORT_SUPPORT,         mdns_txt_int_to_bool_str},
    {"Staple",      NULL,               OID_PRINT_STAPLE_SUPPORT,       mdns_txt_int_to_bool_str},
    {"note",        NULL,               OID_SM_SYSTEM_LOCATION,         NULL},
};
static uint32_t g_mdns_txt_entry_cnt_ipp = sizeof(g_mdns_txt_entries_ipp)/sizeof(mdns_txt_entry_t);

static map_handle_t g_mdns_txt_map = MAP_INVALID_HANDLE;


//=========================================================================================
// Local function declarations
//=========================================================================================
static error_type_t _pdl_datastream_tcp_txt_record(int index, char** txt_record);
//static error_type_t mdns_get_txt_rdata(char* buf, uint32_t *len); // rdj added feb-2014
static int32_t mdns_get_txt_entry_str(mdns_txt_entry_t *txt_entry, char *buf, uint32_t *len);// rdj added feb-2014
static error_type_t _ipp_tcp_txt_record(int index, char** txt_record);
static error_type_t _ipp_sub_types( int index, char **sub_type );
static error_type_t _printer_tcp_txt_record(int index, char** txt_record);
static error_type_t _scanner_tcp_txt_record(int index, char** txt_record);
static uint32_t     _get_dynamic_num_ipp_subtypes();

#ifdef MDNS_DEBUG_HEX_DUMP
static void rdata_hex_dump(unsigned char *ptr, int size);
#endif // MDNS_DEBUG_HEX_DUMP

static void populate_pdl_str();
static void populate_adminurl_str();
static void populate_usb_CMD_str();

static char* get_feature_support(uint32_t OID);
static char* get_MDNS_TXT_STAPLE();
static char* get_MDNS_TXT_SORT();
static char* get_MDNS_TXT_PUNCH();
static char* get_MDNS_TXT_BIND();
static char* get_MDNS_TXT_COLLATE();
static char* get_MDNS_TXT_COPIES();
static char* get_MDNS_TXT_COLOR();
static char* get_MDNS_TXT_DUPLEX();
static char* get_MDNS_TXT_SCAN();
static char* get_MDNS_TXT_FAX();

//=========================================================================================
// Local variables
//=========================================================================================
static char mdns_txt_bind[MDNS_TXT_BIND_STR_LEN]       = MDNS_TXT_BIND_STR;
static char mdns_txt_collate[MDNS_TXT_COLLATE_STR_LEN] = MDNS_TXT_COLLATE_STR;
static char mdns_txt_color[MDNS_TXT_COLOR_STR_LEN]     = MDNS_TXT_COLOR_STR;
static char mdns_txt_copies[MDNS_TXT_COPIES_STR_LEN]   = MDNS_TXT_COPIES_STR;
static char mdns_txt_duplex[MDNS_TXT_DUPLEX_STR_LEN]   = MDNS_TXT_DUPLEX_STR;
static char mdns_txt_punch[MDNS_TXT_PUNCH_STR_LEN]     = MDNS_TXT_PUNCH_STR;
static char mdns_txt_scan[MDNS_TXT_SCAN_STR_LEN]       = MDNS_TXT_SCAN_STR;
static char mdns_txt_fax[MDNS_TXT_FAX_STR_LEN]         = MDNS_TXT_FAX_STR;
static char mdns_txt_sort[MDNS_TXT_SORT_STR_LEN]       = MDNS_TXT_SORT_STR;
static char mdns_txt_staple[MDNS_TXT_STAPLE_STR_LEN]   = MDNS_TXT_STAPLE_STR;
static char pdl_str[MAX_PDL_STR_LEN]                   = {0};
static char usb_CMD[USB_CMD_STR_LEN]                   = {0};

// The +# account for the txt record name= string length
static char usb_mfg[MFG_NAME_MAX_SIZE + 8]                          = "";
static char usb_mdl[MODEL_NAME_MAX_SIZE + 8]                        = "";
static char mfg[MFG_NAME_MAX_SIZE + 4]                              = "";
static char mdl[MODEL_NAME_MAX_SIZE + 4]                            = "";
static char adminurl[ADMIN_URL_MAX_SIZE]                            = "";
static char ty_printer[FRIENDLY_NAME_MAX_SIZE  + 3]                 = "";
static char product[MFG_NAME_MAX_SIZE + MODEL_NAME_MAX_SIZE + 11  ] = "";
static char mac[MAC_ADDRESS_LENGTH + 4 ]                            = "";
static char uuid[UUID_FRIENDLY_LENGTH + 5 ]                         = "";
static char location_note[DEVICE_ID_LOCATION_LENGTH + 5 ]           = "";

static char *ipp_sub_types[] =
{
    "_ePCL",
};

static const uint32_t num_fixed_IPP_sub_types = (sizeof(ipp_sub_types) / 
                                                 sizeof(ipp_sub_types[0]));

net_mdns_config_handler_map_t _handlers[] =
{
    {"_pdl-datastream._tcp", _pdl_datastream_tcp_txt_record,    NULL,                           NULL           },
    {"_ipp._tcp",            _ipp_tcp_txt_record,               _get_dynamic_num_ipp_subtypes,  _ipp_sub_types },
    {"_printer._tcp",        _printer_tcp_txt_record,           NULL,                           NULL           },
    {"_scanner._tcp",        _scanner_tcp_txt_record,           NULL,                           NULL           },
};

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

pthread_mutex_t g_oem_conf_mutex;

#define OEM_CONF_LOCK() \
        { \
            unsigned os_rcode; \
            os_rcode = pthread_mutex_lock(&g_oem_conf_mutex); \
            XASSERT(os_rcode == 0, os_rcode); \
        }

#define OEM_CONF_UNLOCK() \
        { \
            unsigned os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_oem_conf_mutex); \
            XASSERT(os_rcode == 0, os_rcode); \
        } 


static const char* start_mdns_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " mdns_start_sh,
"",
"# expected mDNSPosixResponder location",
"export PATH=$PATH:/usr/local/sbin",
"",
"config_file=" INITD "/service.mdns.conf",
"",
"# exit if already running ..",
"mdns=$(ps -w | grep -v grep | grep -e mDNSResponder | awk '{print $1}')",
"test ! -z \"$mdns\" && (cat " VAR_RUN "/mDNSResponder.pid && exit 0)",
"",
"# start new service/daemon",
"mDNSResponderPosix -b -f ${config_file} && cat " VAR_RUN "/mDNSResponder.pid",
"",
"exit 0",
"",
"#eof " mdns_start_sh,
NULL
};

/** 
 *  run-time on-demand
 * 
 *  This function is called anytime the script is required but unavailable
 * 
 *  rebuild the start-daemon script
 **/ 
error_type_t rebuild_mdns_start_daemon_sh(void)
{
    return write_sh_file((char*)mdns_start_sh, (char**)&start_mdns_script);
}

static const char* stop_mdns_script[] = {
"#!/bin/sh",
"#/*",
"# * ============================================================================",
"# * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved",
"# *",
"# * ============================================================================",
"# */",
"#",
"# " stop_mdns_sh,
"",
"pidfile=" VAR_RUN "/mDNSResponder.pid",
"test -f $pidfile && (kill `cat $pidfile` 2>/dev/null) || true",
"",
"# cleanup any orphaned mDNSResponders..",
"pid=$(ps -w |grep -v grep |grep mDNSResponder |awk '{print $1}')",
"test -z \"$pid\" || (echo \"mdns: stop $pid\" && kill -9 $pid)",
"",
"exit 0",
"",
"#eof " stop_mdns_sh,
NULL
};

void remove_spaces(char* s);

/** 
 *  run-time on-demand
 * 
 *  This function is called anytime the script is required but unavailable
 * 
 *  rebuild the stop-daemon script
 **/ 
error_type_t rebuild_stop_mdns_daemon_sh(void)
{
    return write_sh_file((char*)stop_mdns_sh, (char**)&stop_mdns_script);
}

// public access
error_type_t rebuild_mdns_sh_files(void)
{
    error_type_t res;
    res  = rebuild_mdns_start_daemon_sh();
    res |= rebuild_stop_mdns_daemon_sh();
    return res;
}

// leaf
error_type_t stop_mdns_service(void)
{
    error_type_t rcode = ~OK;

    // current stop_daemon script defined?
    if (!file_exists(stop_mdns_sh))
    {
        DBG_ERR("%s() no '%s'\n", __func__, stop_mdns_sh);
        rcode = rebuild_stop_mdns_daemon_sh(); // time to rebuild
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    return rcode = os_system((char*)stop_mdns_sh);
}

// true if file exists AND empty
// false otherwise
bool empty_conf(char* filename)
{
    if (!filename) return false;
    if (file_exists(filename))
    {
        int fd = open(filename, FD_CLOEXEC | O_RDONLY);
        if (fd >= 0)
        {
            char tbuf[64];
            int bytes_read = read(fd, tbuf, sizeof(tbuf));
            close(fd); // close file
            if (bytes_read == 0)
            {
                return true;
            }
            return false;
        }
        else
        {
            DBG_ERR("%s(%s) open %s (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
        }
    }
    return true;
}

bool identical_serial(){
	
	error_type_t db_retval = FAIL;
	char *MachineSerial;
	char *AirprintSerial;
	bool ret = false;

	MachineSerial = db_get_var("INST_machine","MachineSerialNo",&db_retval);
	if((MachineSerial == NULL) || (db_retval == FAIL)){
    	DBG_ERR("%d(%s) \n", __LINE__, __func__);
	}
	AirprintSerial = db_get_var("INST_machine","AirprintSerialNo",&db_retval);
	if((AirprintSerial == NULL) || (db_retval == FAIL)){
    	DBG_ERR("%d(%s) \n", __LINE__, __func__);
	}

	if(strcmp(MachineSerial, AirprintSerial) == 0){
		ret = true;
	}
	else{
		ret = false;
	}

	MEM_FREE_AND_NULL(MachineSerial);
	MEM_FREE_AND_NULL(AirprintSerial);

	return ret;
}

void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

// start_mdns_service() -- verify conf/scripts exist and startup
error_type_t start_mdns_service(void)
{
    error_type_t rcode = OK;

    // current start_daemon script defined?
    if (!file_exists(mdns_start_sh))
    {
        DBG_ERR("%s() no '%s'\n", __func__, mdns_start_sh);
        rcode = rebuild_mdns_start_daemon_sh(); // time to rebuild
        if (rcode != OK) // can't run it if it doesn't exist ..
        {
            DBG_ASSERT(0); // fix it now
            return rcode;
        }
    }

    // mdns initialized?
    if (g_mdns_txt_map == MAP_INVALID_HANDLE)
    {
        DBG_ERR("%s() not ready\n", __func__);
        return FAIL; // map not ready yet, try again later..
    }

    // current service name
    char mdns_service[NET_MDNS_SERVICE_NAME_SIZE];
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, mdns_service, NET_MDNS_SERVICE_NAME_SIZE);

    // current config-file defined?
    char conf_file[MAX_CONF_NAME_SIZE];
    lookup_fs_name(Bonjour_e, conf_file, MAX_CONF_NAME_SIZE);
    if (empty_conf(conf_file) || !identical_serial())
    {
		error_type_t db_retval = FAIL;
		char *MachineSerial;
		char *MachineTLI;
		char *vendorName=NULL;
		char *ModelName=NULL;
        DBG_VERBOSE("%s() no '%s'\n", __func__, conf_file);

        // generate a unique service name if not currently defined
        if (!mdns_service[0])
        {
            generate_uniq_service_name(mdns_service); // generate '<default> [<mac>[3:5]]'
            DBG_VERBOSE("%s() default service '%s'\n", __func__, mdns_service);
        }
		MachineSerial = db_get_var("INST_machine","MachineSerialNo",&db_retval);
		MachineTLI = db_get_var("INST_machine","MachineTLI",&db_retval);
		vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
		ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
		remove_spaces(ModelName);

		if((MachineSerial == NULL) || (db_retval == FAIL)){
			sprintf(mdns_service,"SINDOH61x_Series");
		}    
		else{
			// if(0==strcmp(vendorName,DEFAULT_VENDOR))
			if(0==strcasecmp(vendorName, DEFAULT_VENDOR))
				{
					sprintf(mdns_service,"%s_%s",MachineTLI,MachineSerial);
				}
				else
				{
					// sprintf(mdns_service,"%s_%s",ModelName,MachineSerial);
					sprintf(mdns_service,"%s %s", vendorName, ModelName);
				}
		}   

		db_retval = db_set_var("INST_MACHINE","AirprintSerialNo",MachineSerial);
		if(db_retval == FAIL){
			DBG_ERR("DB FILE WRITE FAILED!!!!!!!!!!!!!!!!!!![%d:%s]\n",__LINE__,__func__);
			DBG_ERR("DB FILE WRITE FAILED!!!!!!!!!!!!!!!!!!![%d:%s]\n",__LINE__,__func__);
		}
		MEM_FREE_AND_NULL(MachineSerial);
		MEM_FREE_AND_NULL(MachineTLI);
		MEM_FREE_AND_NULL(vendorName);
		MEM_FREE_AND_NULL(ModelName);
		//printf("@#@#@#@#@#@# $$%s$$\n",mdns_service);
		rcode = mdns_write_conf(mdns_service);
		if (rcode != OK) // can't run it if it doesn't exist ..
		{
			DBG_ERR("%s() err %d '%s'\n", __func__, rcode, conf_file);
			DBG_ASSERT(0); // fix it now
			return rcode;
		}
	}
	// initiate service/daemon if enabled
    if (mdns_enabled() && !mdns_running())
    {
        DBG_MSG("%s bonjour '%s' (@ %d)\n", __func__, mdns_service, posix_gettime_ticks());
		error_type_t db_retval = FAIL;
		int Is_Authmode;

		Is_Authmode= db_get_var_int("CONF_SOLUTIONSP","AuthType",&db_retval);
		if(db_retval == FAIL){
			Is_Authmode = 0;
		}

		if(Is_Authmode == 0){
	        rcode = os_system((char*)mdns_start_sh);
			sleep(2);
		}
		else{
			printf("[SOLUTION MODE:%d] MDNS DO NOT RUNNING[%d:%s]\n",Is_Authmode,__LINE__,__func__);
		}
    }

    return rcode;
}

// restart bonjour deamon (post-conf file update)
error_type_t propogate_mdns_conf_change(void)
{
    error_type_t rcode = ~OK;

    rcode = stop_mdns_service();
    if (rcode == OK)
    {
        posix_sleep_ms(1); // nominal settle-time -- once around
        start_mdns_service();
    }
    else
    {
        DBG_ERR("%s() stop (err %d)\n", __func__, rcode);
    }
    return rcode;
}

// replace configuration file (with backup)
error_type_t update_mdns_conf(char* with_service)
{
    error_type_t rcode = ~OK;
    char* conf_file_image = (char*) MEM_MALLOC(MAX_CONF_FILE_SIZE);

    DBG_VERBOSE("%s() hostname '%s'\n", __func__, with_service);

    if (conf_file_image != NULL)
    {
        memset(conf_file_image, 0, MAX_CONF_FILE_SIZE);

        // generate updated file content
        rcode = ~OK;
        if (format_mdns_conf(conf_file_image, MAX_CONF_FILE_SIZE) != NULL)
        {
            // update fs:
            rcode = write_conf_file(Bonjour_e, conf_file_image);
        }
        MEM_FREE_AND_NULL(conf_file_image);
    }

    if (rcode != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(rcode), rcode);
    }
    return rcode;
}

#define newline '\n'

// return next writable string location (e.g, add a newline after tok)
static char* add_token_plus_newline(char* next_loc, char* tok)
{
    int chars_added = 0;
    if (tok != NULL)
    {
        int token_len = strlen(tok);
        chars_added = sprintf(next_loc, tok);
        // single byte characters assumed
        if (chars_added != token_len) // error fixup?
        {
            DBG_ERR("%s(): bad write length %d/%d?\n", __func__, chars_added, token_len);
        }
        next_loc += chars_added;
        *next_loc++ = newline;
    }
    return next_loc;
}
// add_token_plus_newline() helper macro - add_strnl()
#define add_strnl(token) add_token_plus_newline(n, token)

// static mdns conf file settings and capabilities
const char* TXT_general[] = {
    "txtvers=1",
    "qtotal=1",
    "ty=printer",
    "URF=none",
	"rp=auto",
//    "priority=5",
    "Binary=T",
    "Transparent=T",
    "Color=F",
    "Copies=T",
    "Duplex=T",
    "PaperCustom=F",
    "Punch=F",
    "Scan=F",
    "Fax=F",
    "Sort=F",
    "Staple=F",
    NULL // required end-of-TXT-strings
};

// Scanner SRVs advertise an abbreviated TXT
const char* scanner_TXT_records[] = {
	"txtvers=1",
	"qtotal=1",
	"Scan=T",
	"Duplex=T",
	"scannerAvailable=1",
    "URF=none",
    NULL // required end-of-TXT-strings
};

// http SRVs advertise an abbreviated TXT
const char* http_TXT_records[] = {
    "txtvers=1",
    "URF=none",
    NULL // required end-of-TXT-strings
};

// supported interpreters (added to static TXT records)
const char* PDL[] = {
    "image/urf",
	"application/pdf",
    "application/postscript",
    "application/zjs",
    NULL // required end-of-PDL-strings
};

// add supported comma-delimited interpretes
static char* add_pdl_record(char* starting_loc, int service)
{
    int pdl = 0;
    char* next = starting_loc;
    strcat(next,"pdl=");
    int chars_added = 4; // strlen("pdl=");
    strcat(next, PDL[0]); // assumes at least one pdl
    chars_added += strlen(PDL[0]);
    pdl++; // next pdl
    while (PDL[pdl] != NULL)
    {
        strcat(next, ",");
        strcat(next, PDL[pdl]);
        chars_added += (strlen(PDL[pdl]) + 1 /*comma*/);
        pdl++; // next pdl
    }
    strcat(next, "\n\0"); // newline finishes pdl record
    chars_added++;
    
    //noisy DBG_VERBOSE("%s\n", starting_loc);
    return starting_loc + chars_added;
}

// mdns service/domain advertisements for "rp=" txt record
// write inplace, return str start
#if 0
static char* format_rp_str(int service_type, char* buf)
{
    const char* rp_fmt_string = "rp=%s";
    if (!buf) return NULL; // buffer required
    const char* rp_str[] = // see SRV[] for relative association and ordering
    {
        "auto",     //  { "_pdl-datastream._tcp", "9100" },
        "ipp",      //  { "_printer._tcp",         "515" },
        "airprint", //  { "_http._tcp",             "80" },
        "airprint", //  { "_http-alt._tcp",       "8080" },
        "ipp/print",//  { "_ipp._tcp",             "631" }, // &_universal._sub
    };
    #define sizeof_rp_strings (sizeof(rp_str)/sizeof(char*))
    if (service_type >= sizeof_rp_strings) service_type = 0; // wrap around -> default auto
    sprintf(buf, rp_fmt_string, rp_str[service_type]);
    return buf;
}

// add supported comma-delimited interpretes
static char* add_rp_record(char* starting_loc, int service)
{
    char* next = starting_loc;
    DBG_ASSERT(starting_loc);
    if (!starting_loc) return NULL;
    char* rp_str = format_rp_str(service, starting_loc); // copied here
    int chars_added = strlen(rp_str);
    strcat(next, "\n"); // newline finishes rp record
    chars_added++;
    return starting_loc + chars_added;
}
#endif

// formats for dynamic string replacements
// NOTE:  embedded spaces within product name require literal \032 (revisit)
static char* format_product_str(char* product, char* buf)
{
    const char* prod_fmt_string = "product=(%s)";
    DBG_ASSERT(buf);
    DBG_ASSERT(product);
    if (!buf || !product) return NULL; // both buffers required
    sprintf(buf, prod_fmt_string, product);
    return buf;
}


// format mac addr string for "mac=..." advertisement
static char* format_mac_str(uint8_t* ma, char* buf)
{
    const char* mac_fmt_string = "mac=%02x:%02x:%02x:%02x:%02x:%02x\0";
    if (!buf || !ma) return NULL; // both buffers required

    sprintf(buf, mac_fmt_string, ma[0],ma[1],ma[2],ma[3],ma[4],ma[5]);
    return buf;
}

// associate service/domain w/port
struct SRV_record {
    char* service;
    char* socket_port;
    const char** TXT_records_override;
} SRV[] =
{
    { "_pdl-datastream._tcp", "9100", NULL },
    { "_printer._tcp",         "515", NULL },
    { "_scanner._tcp",         "9101", scanner_TXT_records },
//    { "_http._tcp",             "80", http_TXT_records },
//    { "_http-alt._tcp",       "8080", http_TXT_records },
    { "_ipp._tcp",             "631", NULL }, // implies "_universal._sub"
};
#define last_service (sizeof(SRV)/sizeof(struct SRV_record))


// retrieve a current os-known intf mac address -- returns NULL if unsuccessful
//   1st choice - wired aka ethernet
//   2nd choice - wireless/sta aka mlan/wlan
//   3rd choice - wireless uAP
static uint8_t* live_get_mac(uint8_t* mac_addr_buf, int buf_len)
{
    if (!mac_addr_buf) return NULL; // need a buf

    uint8_t* mac_addr = get_intf_mac_addr(NET_IF_NAME_ETH, mac_addr_buf); // 1st-choice wired
    if (!mac_addr || null_mac(mac_addr, buf_len)) 
        mac_addr = get_intf_mac_addr(NET_IF_NAME_STA, mac_addr_buf); // 2nd-choice wifi/sta
    if (!mac_addr || null_mac(mac_addr, buf_len)) 
        mac_addr = get_intf_mac_addr(NET_IF_NAME_UAP, mac_addr_buf); // 3rd-choice uAP

    if (mac_addr)
    {
        memcpy(mac_addr_buf, mac_addr, buf_len);
        DBG_VERBOSE("%s() mac [%02x:%02x:%02x:%02x:%02x:%02x]\n", __func__,
            mac_addr_buf[0], mac_addr_buf[1], mac_addr_buf[2],
            mac_addr_buf[3], mac_addr_buf[4], mac_addr_buf[5]
        );
        return mac_addr_buf;
    }
return NULL;
}

// generate_uniq_service_name() -- extend DEFAULT_PLATFORM_HOSTNAME w/mac[3:5] suffix
char* generate_uniq_service_name(char* uniq_name_buf)
{
    if (!uniq_name_buf) return NULL; // nothing to be done
    uint8_t  oid_mac[MAC_ADDR_BYTE_SIZE] = {0};
    uint8_t* mac_addr = live_get_mac(oid_mac, sizeof(oid_mac));

    sprintf(uniq_name_buf, "%s", DEFAULT_PLATFORM_HOSTNAME); // default

    // create 'unique' devname as 'service_name[mac[3,5]]'
	if (mac_addr != NULL) {
		const char* fmt = "%s [%02x%02x%02x]\0";
		sprintf(uniq_name_buf, fmt, DEFAULT_PLATFORM_HOSTNAME, mac_addr[3], mac_addr[4], mac_addr[5]);
	}

	//Airprint Name : A61x_SerialNumber (ex.A61x_123412341234)  #TEMPTEMP
	{
		error_type_t db_retval = FAIL;
		char *MachineSerial;
		char *MachineTLI;
		char *vendorName=NULL;
		char *ModelName=NULL;
		MachineSerial = db_get_var("INST_machine","MachineSerialNo",&db_retval);
		MachineTLI = db_get_var("INST_machine","MachineTLI",&db_retval);
		vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
		ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
		remove_spaces(ModelName);
		if((MachineSerial == NULL) || (db_retval == FAIL)){
			sprintf(uniq_name_buf,"SINDOH61x_Series");
		}    
		else{
			// if(0==strcmp(vendorName,DEFAULT_VENDOR))
			if(0==strcasecmp(vendorName, DEFAULT_VENDOR)) // by mspyo, 2021-03-15
			{
				sprintf(uniq_name_buf,"%s_%s",MachineTLI,MachineSerial);
			}
			else
			{
				// sprintf(uniq_name_buf,"%s_%s",ModelName,MachineSerial);
				sprintf(uniq_name_buf,"%s %s", vendorName, ModelName);
			}
			MEM_FREE_AND_NULL(MachineSerial);
			MEM_FREE_AND_NULL(MachineTLI);
			MEM_FREE_AND_NULL(vendorName);
			MEM_FREE_AND_NULL(ModelName);
		}    
	}

    DBG_VERBOSE("%s() uniq service name -> '%s'\n", __func__, uniq_name_buf);
    return uniq_name_buf;
}

// format_mdns_conf() generates the entire mdns conf file image suitable for
// the mDNSResponderPosix bonjour implementation
//
// returns 'given_buffer' (filled as appropriate)
//
char* format_mdns_conf(char* given_buffer, int buf_len)
{
    char*    next = given_buffer; // string buffer starts here
    int      service = 0;
    char     name_buf[NET_MDNS_SERVICE_NAME_SIZE], *service_name = name_buf;
    uint8_t  oid_mac[MAC_ADDR_BYTE_SIZE] = {0};
    int      chars_added = 0;
    char     uniq_devname[NET_MDNS_SERVICE_NAME_SIZE] = {};
    char     str_mac[32], *mac_ptr = NULL;
    char     product_str_record[2*NET_MDNS_SERVICE_NAME_SIZE], *product_ptr = NULL;
    int      intf = 0;
    update_vendor_Name();
    // retrieve app's stored service_name
    if (OK != oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, intf, service_name, NET_MDNS_SERVICE_NAME_SIZE))
    {
        service_name[0] = '\0'; // nullify current service name
        oid_set_string(OID_NETWORK_BONJOUR_SERVICE_NAME, intf, "", strlen("")); 
    }

    // retrieve mac addr
    memset(str_mac, 0, sizeof(str_mac));
    live_get_mac(oid_mac, sizeof(oid_mac));
    mac_ptr = format_mac_str(oid_mac,str_mac); // dynamic mac addr
    if (mac_ptr == NULL) *str_mac = '\0';

    // generate default service NOT defined (or undefined)
	/*
    if (!service_name || !service_name[0] || !strcmp(service_name, DEFAULT_PLATFORM_HOSTNAME))
    {
        generate_uniq_service_name(uniq_devname); // generate '<default> [<mac>[3:5]]'
    }
    else
    {
        DBG_VERBOSE("%s() service_name -> '%s'\n", __func__, service_name);
        snprintf(uniq_devname, NET_MDNS_SERVICE_NAME_SIZE, "%s", service_name);
    }
	*/
    generate_uniq_service_name(uniq_devname); // generate '<default> [<mac>[3:5]]'

    // create product record "product=(<product name>)"
    char product_str[NET_IFACE_HOSTNAME_SIZE] = { 0 };
    {
        if (OK == oid_get_string(OID_SM_SYSTEM_NAME, 0, product_str, sizeof(product_str)))
        {
            product_ptr = format_product_str(product_str, product_str_record);
        }
        if (product_ptr == NULL) *product_str_record = '\0';
    }

    // output conf file header/banner
    char* n = next;
    n = add_strnl("# /etc/init.d/service.mdns.conf");
    n = add_strnl("# usage: each grouping defines;\n#");
//    n = add_strnl("# < hostname (unique) >");
//    n = add_strnl("# < service [domain]  >");
//    n = add_strnl("# < port              >");
//    n = add_strnl("# < TXT records       >");
//    n = add_strnl("# < newline delimited >");
    n = add_strnl("#\n");

    // for each service pdl, printer, ipp, http...
    int last_service_except_ipp_record = last_service-2; // assumes 'scanner' second last SRV[] record and assumes 'ipp' last SRV[] record
    while (service < last_service_except_ipp_record)
    {
        n = add_strnl(uniq_devname);
        n = add_strnl(SRV[service].service);       // see SRV[]
        n = add_strnl(SRV[service].socket_port);   // see SRV[]

	// general TXT records possibly overriden
        n = add_txt_records(n,
              (char**)(SRV[service].TXT_records_override != NULL
                ? SRV[service].TXT_records_override
                : TXT_general)
            );
		if(!service){
			n = add_strnl("priority=5");
		}
//        if (*str_mac != '\0')
//        {
//            n = add_strnl(str_mac);                // dynamic mac addr
//        }
        n = add_pdl_record(n,service);             // see PDL[]
//        n = add_rp_record(n,service);              // see rp_str[]
//        n = add_strnl(product_str_record);         // 
        n = add_newline(n);                        // end-of SRV record
        service++;
        next = n;
        chars_added = (int)(next - given_buffer); // watch buffer growth

        if (chars_added >= buf_len) // revisit overflow condition
        {
            DBG_VERBOSE("%s(): buffer size %d > %d '\n%s\n'\n", __func__, chars_added, buf_len, given_buffer);
            given_buffer[buf_len-1] = '\0';
            return given_buffer;
        }
    }

    ASSERT( ! strcmp(SRV[service].service, "_scanner._tcp") ); // service @ 'scanner' SRV[] record?
	{
        //DBG_ERR("%s(): _scanner._tcp config \n", __func__);
        if(IS_SCANNER_ATTACHED)
        {
            char     mfg_name_str[MFG_NAME_MAX_SIZE+7];
            char     usb_mfg_name[MFG_NAME_MAX_SIZE+7];
            char     mdl_name_str[MFG_NAME_MAX_SIZE+7];
            char     ty_printer_str[FRIENDLY_NAME_MAX_SIZE+7];
            char     bonjour_prod_str[sizeof(product)];
            char     product_name[FRIENDLY_NAME_MAX_SIZE];
            char *vendorName=NULL;
            char *ModelName=NULL;
            error_type_t db_retval = FAIL;
            uint32_t oid_error;
            
            vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
            ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
            // remove_spaces(ModelName);

            // if(0==strcmp(vendorName,DEFAULT_VENDOR))
            if(0==strcasecmp(vendorName, DEFAULT_VENDOR)) // by mspyo, 2021-03-15
            {  oid_error = oid_get_string(OID_PLATFORM_USB_MFG_STRING,
					   0,
					   usb_mfg_name,
					   sizeof(usb_mfg_name));
                ASSERT(oid_error == OK);
            	getPrinterName(product_name);
            }
            else
            {
            	strncpy(usb_mfg_name,vendorName,strlen(vendorName));
            	sprintf(product_name,"%s %s", vendorName, ModelName);
            }

    		MEM_FREE_AND_NULL(vendorName);
    		MEM_FREE_AND_NULL(ModelName);
            sprintf(mdl_name_str, "%s=%s", USB_MDL_KEY, product_name);
            sprintf(ty_printer_str, "%s=%s", TY_KEY, product_name);
            sprintf(mfg_name_str, "%s=%s", USB_MFG_KEY, usb_mfg_name);
            sprintf(bonjour_prod_str, "%s=(%s)", PRODUCT_KEY, product_name);

            n = add_strnl(uniq_devname); //n = add_strnl(product_name);
            n = add_strnl(SRV[service].service);       // see SRV[]
            n = add_strnl(SRV[service].socket_port);   // see SRV[]

            // general TXT records possibly overriden
            n = add_txt_records(n,
            	  (char**)(SRV[service].TXT_records_override != NULL
            		? SRV[service].TXT_records_override
            		: TXT_general)
                );

            n = add_pdl_record(n,service);             // see PDL[]
            //n = add_rp_record(n,service);              // see rp_str[]
            n = add_strnl(bonjour_prod_str);         //
            n = add_strnl(ty_printer_str);         //
            n = add_strnl(mdl_name_str);         //
            n = add_strnl(mfg_name_str);         //

            n = add_newline(n);                        // end-of SRV record
    	}

        service++;
        next = n;
        chars_added = (int)(next - given_buffer); // watch buffer growth

        if (chars_added >= buf_len) // revisit overflow condition
        {
            DBG_ERR("%s(): buffer size %d > %d '\n%s\n'\n", __func__, chars_added, buf_len, given_buffer);
            given_buffer[buf_len-1] = '\0';
            return given_buffer;
        }
    }

    // tbd: don't rely on SRV[] table order -- lookup 'ipp' service record ..

    // fallthrough with service -- finally add 'ipp' SRV[] record
    ASSERT( ! strcmp(SRV[service].service, "_ipp._tcp") ); // service @ 'ipp' SRV[] record?
    {
        n = add_strnl(uniq_devname);
        n = add_strnl(SRV[service].service);       // see SRV[]
        n = add_strnl(SRV[service].socket_port);   // see SRV[]

        int32_t error = SYS_OK;
        uint32_t tmp_len; 
        mdns_txt_entry_t *txt_entry = NULL;
        map_iterator_t iter_hndl;

        // mdns txt map initialized?
        if (g_mdns_txt_map == MAP_INVALID_HANDLE)
        {
            return NULL; // map not ready yet, try again later..
        }

        iter_hndl = map_iterate_begin(g_mdns_txt_map);
        do
        {
            txt_entry = (mdns_txt_entry_t *)map_iterate_next(iter_hndl);
            if(txt_entry)
            {
                tmp_len = MAX_CONF_FILE_SIZE - (n - given_buffer);
                error = mdns_get_txt_entry_str(txt_entry, n, &tmp_len);
                if(error == SYS_OK)
                {
                    n += tmp_len;
                    *n++ = '\n';
                }
                else
                {
                    if(error == OID_ERROR_NOT_AVAILABLE)
                    {
                        // entry not available at this time, e.g. because a subsystem has been temporarily disabled
                        // skip to next entry
                        error = SYS_OK;
                    }
                }
            }
        } while(txt_entry && error == SYS_OK);
        map_iterate_end(iter_hndl);

        XASSERT(!error, error);

        n = add_newline(n);                        // end-of SRV record
        service++;
        next = n;
        chars_added = (int)(next - given_buffer); // watch buffer growth

        if (chars_added >= buf_len) // revisit overflow condition
        {
            DBG_VERBOSE("%s(): buffer size %d > %d '\n%s\n'\n", __func__, chars_added, buf_len, given_buffer);
            given_buffer[buf_len-1] = '\0';
            return given_buffer;
        }
    }

    DBG_VERBOSE("%s():'\n%s\n'\n", __func__, given_buffer);
    return given_buffer;
}

// rewrite mdns config file with 'current service name'
error_type_t mdns_write_conf(char* requested_service)
{
    error_type_t rcode = ~OK;

    // update (if changed)
    if (requested_service)
    {
        rcode = update_mdns_conf(requested_service); // update mdns fs image
        if (OK == rcode) // update mdns fs
        {
            rcode = propogate_mdns_conf_change(); // restart mdns_daemon
        }
    }
    return rcode;
}

// unity additions
/******************************************************************************
*  LOCAL FUNCTIONS
******************************************************************************/
static uint32_t _get_dynamic_num_ipp_subtypes()
{
#ifdef HAVE_AIRPRINT
   // Determine whether Airprint is enabled.
   error_type_t error;
   uint32_t     airprint_enabled = 0;
   error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
   ASSERT(error == OK);

    if (airprint_enabled)
    {
        return num_fixed_IPP_sub_types + 1;
    }
#endif // HAVE_AIRPRINT

    return num_fixed_IPP_sub_types;
}

static error_type_t _pdl_datastream_tcp_txt_record(int index, char** txt_record)
{
   // Populate all the strings as appropriate.
   sprintf(mdns_txt_duplex, "%s%s", MDNS_TXT_DUPLEX_STR, get_MDNS_TXT_DUPLEX());
   sprintf(mdns_txt_color,  "%s%s", MDNS_TXT_COLOR_STR,  get_MDNS_TXT_COLOR());

   populate_pdl_str();
   populate_adminurl_str();

   static char* _records[] = 
   {
      "txtvers=1",
      "qtotal=1",
      pdl_str,
      ty_printer,
      product,
      "priority"MDNS_TXT_PRIORITY,
      adminurl,
      usb_mfg,
      usb_mdl,
      "transparent=T",
      "binary=T",
      mdns_txt_duplex,
      mdns_txt_color
    };

    if ((index >= 0) && (index < sizeof(_records)/sizeof(_records[0])))
    {
        *txt_record = _records[index];
        return OK;
    }

    return FAIL;
}

error_type_t _printer_tcp_txt_record(int index, char** txt_record)
{
   populate_pdl_str();
   populate_adminurl_str();

    static char* _records[] = 
    {
       "txtvers=1",
       "qtotal=1",
       "rp=auto",
       pdl_str,
       ty_printer,
       product,
       "priority="MDNS_TXT_PRIORITY,
       adminurl,
       usb_mfg,
       usb_mdl,
       "transparent=T",    // Should this be lower or upper case (was upper in Sid)
       "binary=T",
    };

    if ((index >= 0) && (index < sizeof(_records)/sizeof(_records[0])))
    {
        *txt_record = _records[index];
        return OK;
    }

    return FAIL;
}

static error_type_t _scanner_tcp_txt_record(int index, char** txt_record)
{
   populate_adminurl_str();

    static char* _records[] = 
    {
       "txtvers=1",
       ty_printer,
       adminurl,
       mfg,
       mdl,
       "button=T",
       "feeder=T",
       "flatbed=T",
    };

    if ((index >= 0) && (index < sizeof(_records)/sizeof(_records[0])))
    {
        *txt_record = _records[index];
        return OK;
    }

    return FAIL;
}

static void populate_pdl_str()
{
   error_type_t error;
   char         tmp_doc_format[MAX_REC_TXT_RECORD_LEN] = {0};
   uint32_t     current_offset                         = 0;
   uint32_t     initial_bytes_for_pdl_str              = sizeof(pdl_str);
   uint32_t     bytes_remaining                        = initial_bytes_for_pdl_str;
   uint32_t     cur_str_len                            = 0;
   uint32_t     index                                  = 0;
   char         *pdl_identifier                        = "pdl=";
#ifdef HAVE_AIRPRINT
   uint32_t     airprint_enabled                       = 0;
   char         *urf_str                               = NULL;
#endif // HAVE_AIRPRINT

   // Zero out the string and put the PDL string indentifier in place.
   memset(&pdl_str[0], 0, initial_bytes_for_pdl_str);

   cur_str_len = strlen(pdl_identifier);

   strncpy(&pdl_str[0], pdl_identifier, cur_str_len);
   current_offset  += (cur_str_len);
   bytes_remaining -= (cur_str_len);

#ifdef HAVE_AIRPRINT
   // Determine whether Airprint is enabled.
   error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
   ASSERT(error == OK);
#endif // HAVE_AIRPRINT

   while (true)
   {
      error = oid_get_string(OID_SM_PRINT_DOC_FORMAT_SUPPORTED, 
                             index,
                             &tmp_doc_format[0],
                             sizeof(tmp_doc_format));

      if (error == OK)
      {
#ifdef HAVE_AIRPRINT
         // If Airprint is not enabled then make sure that any URF strings are
         // not included.
         if (!airprint_enabled)
         {
            urf_str = strstr(&tmp_doc_format[0], "/urf");

            // If we find a string with URF in it, reset for the next time
            // around.
            if (urf_str != NULL)
            {
               urf_str = NULL;

               memset(&tmp_doc_format[0], 0, sizeof(tmp_doc_format));

               ++index;

               continue;
            }
         }
#endif // HAVE_AIRPRINT

         cur_str_len = strlen(&tmp_doc_format[0]);
         if (cur_str_len < bytes_remaining)
         {
            sprintf(&pdl_str[current_offset],"%s%s", &tmp_doc_format[0], ",");
            current_offset  += (cur_str_len + 1);
            bytes_remaining -= (cur_str_len + 1);

            memset(&tmp_doc_format[0], 0, sizeof(tmp_doc_format));

            ++index;
         }
         else
         {
            break;
         }
      }
      else
      {
         break;
      }
   }

   pdl_str[current_offset - 1] = 0;
}

void populate_adminurl_str()
{
   uint32_t     str_offset;

   sprintf(&adminurl[0], "%s", ADMINURL_KEY);
   str_offset = strlen(ADMINURL_KEY);

#ifdef HAVE_AIRPRINT
   // Determine whether Airprint is enabled.
   error_type_t error;
   uint32_t     airprint_enabled = 0;
   error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
   ASSERT(error == OK);

   if (airprint_enabled)
   {
      error = oid_get_string(OID_NETWORK_AIRPRINT_CONFIG_URL,
                             0, 
                             &adminurl[str_offset], 
                             sizeof(adminurl) - str_offset);

      ASSERT(error == OK);
   }
#endif // HAVE_AIRPRINT
}

void populate_usb_CMD_str()
{
#ifdef HAVE_AIRPRINT
   error_type_t error;
   uint32_t     airprint_enabled = 0;

   // Determine whether Airprint is enabled.
   error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
   ASSERT(error == OK);

   if (airprint_enabled)
   {
      ASSERT(USB_CMD_STR_LEN > 
             (sizeof(USB_CMD_STR) + sizeof(MDNS_TXT_USB_CMD)));

      sprintf(&usb_CMD[0], "%s%s", USB_CMD_STR, MDNS_TXT_USB_CMD);
   }
   else
#endif // HAVE_AIRPRINT
   {
      ASSERT(USB_CMD_STR_LEN > 
             (sizeof(USB_CMD_STR) + sizeof(MDNS_TXT_USB_CMD_NO_AP)));

      sprintf(&usb_CMD[0], "%s%s", USB_CMD_STR, MDNS_TXT_USB_CMD_NO_AP);
   }
}


/*
 * rdj added feb-2014
 */

int32_t mdns_txt_add_entries(mdns_txt_entry_t *entry_list, uint32_t entry_cnt)
{
//    ipp_instance_t *ipp_instance = (ipp_instance_t *)instance_hndl;
//#ifdef DEBUG
//    XASSERT(ipp_instance->signature == IPP_INST_CTXT_SIGNATURE, (int)ipp_instance);
//#endif

    int32_t map_rcode;
    map_rcode = mdns_txt_insert_entries(entry_list, entry_cnt, 0);
    return map_rcode;
}

static int32_t mdns_txt_insert_entries(mdns_txt_entry_t *entry_list, uint32_t entry_cnt, uint32_t flags)
{
    int32_t ret = SYS_OK;
    int32_t map_rcode;
    int i;
    for(i = 0; i < entry_cnt; i++) 
    {
//        // only public group flags (defined in ipp_api.h) may be set directly from initializer list
//        XASSERT(!(attr_list[i].flags & ~IPP_GRP_MASK), (int)attr_list[i].flags);
//        attr_list[i].flags &= IPP_GRP_MASK;
//
//        // apply any internal flags
//        attr_list[i].flags |= flags; 
//
//        // verify IPP_CAT_ATTR entries are assigned to one and only one group and that all entries
//        // have one and only one category -- note that at most one bit is set if (x&(x-1))==0
//        XASSERT((attr_list[i].flags & IPP_GRP_MASK) && (!(attr_list[i].flags & IPP_CAT_ATTR) ||
//               !((attr_list[i].flags & IPP_GRP_MASK) & ((attr_list[i].flags & IPP_GRP_MASK)-1))), i);
//        XASSERT((attr_list[i].flags & IPP_CAT_MASK) &&
//               !((attr_list[i].flags & IPP_CAT_MASK) & ((attr_list[i].flags & IPP_CAT_MASK)-1)), i);
//
//        if(flags & IPP_MAP_OVERRIDE)
//        {
//            // first remove any pre-existing IPP attribute of same name
//            void *val;
//            val = map_remove(ipp_inst->attr_map, attr_list[i].name, strlen(attr_list[i].name));
//            if(!val)
//            {
//                // there was no pre-existing IPP attribute with this name so clear the flag
//                attr_list[i].flags &= ~IPP_MAP_OVERRIDE;
//            }
//        }

        if(!entry_list[i].separator)
        {
            entry_list[i].separator = ","; // default separator
        }
        entry_list[i].flags = 0;
        entry_list[i].next = NULL;

        ASSERT(g_mdns_txt_map != MAP_INVALID_HANDLE);
        // TODO: convert map keys to lowercase since TXT names are case insensitive (but preserve case in 'name' field)
        map_rcode = map_insert(g_mdns_txt_map, entry_list[i].name, strlen(entry_list[i].name), (void *)&entry_list[i]);
        XASSERT(map_rcode == SYS_OK || map_rcode == MAP_DUP_KEY, map_rcode); 
        if(map_rcode == MAP_DUP_KEY)
        {
            // note this is not necessarily a fatal error
//            ipp_rcode = IPP_DUP_ATTR;
        }
    }

    return ret;
}

static int32_t mdns_txt_int_to_bool_str(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    uint32_t oid_val = 0;
    int32_t ret = SYS_OK;

    ASSERT(oid != OID_INVALID);
    ASSERT(buf);
    ASSERT(len);
    XASSERT(*len > 1, *len);

    if(index > 0)
    {
        buf[0] = '\0';
        *len = 0;
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    buf[1] = '\0';
    *len = 1;

    ret = oid_get_uint32(oid, 0, &oid_val);                                         
    if(ret != SYS_OK)
    {
        buf[0] = 'U'; // 'unknown'
        ret = SYS_OK;
        goto done;
    }
    buf[0] = 'F';
    if(oid_val > 0)
    {
        buf[0] = 'T';
    }

done:
    return ret;
}

static int32_t mdns_ipp_txt_pdl_list(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    int32_t ret;
    ret = oid_get_string(OID_SM_PRINT_DOC_FORMAT_SUPPORTED, index, buf, *len);
    *len = strlen(buf); 

#ifdef HAVE_AIRPRINT
    // If we find a string with URF in it, include only if airprint is enabled.
    char *urf_str;
    urf_str = strstr(buf, "/urf");
    if(urf_str != NULL)
    {

        error_type_t error;
        uint32_t airprint_enabled = 0;
        // Determine whether Airprint is enabled.
        error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
        ASSERT(error == OK);
        if(!airprint_enabled)
        {
            // skip urf
            *len = 0;
            buf[0] = '\0';
            ret = OID_ERROR_INDEX_UNAVAILABLE;
        }
    }
#endif // HAVE_AIRPRINT

    return ret;
}

static int32_t mdns_ipp_txt_product(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    error_type_t error;
    int32_t ret = SYS_OK;
    int32_t pos = 0;

    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    if(index > 0)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    if(*len > 3) // at least "(x)"
    {
        buf[pos] = '(';
        pos++;
    }
    else
    {
        ret = SYS_FAIL; 
        goto done;
    }


    char *vendorName=NULL;
    char *ModelName=NULL;
    error_type_t db_retval = FAIL;
    vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
    ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
    // remove_spaces(ModelName);
    // if(0!=strcmp(vendorName,DEFAULT_VENDOR))
    if(0!=strcasecmp(vendorName,DEFAULT_VENDOR)) // by mspyo, 2021-03-15
    {
    	sprintf( buf + pos,"%s %s", vendorName, ModelName);
    }
    else
    {
        error = oid_get_string(OID_SM_SYSTEM_MAKE_AND_MODEL, 0, buf + pos, *len - pos);
        if(error != SYS_OK)
        {
            ret = SYS_FAIL;
            pos = 0;
            goto done;
        }
    }

	MEM_FREE_AND_NULL(vendorName);
	MEM_FREE_AND_NULL(ModelName);
    pos += strlen(buf + pos);
    XASSERT(*len > pos, pos); // note that oid_get_string() should include a NULL terminator

    if(*len - pos > 1)
    {
        buf[pos] = ')';
        pos++;
    }
    else
    {
        ret = SYS_FAIL; 
        goto done;
    }

done:

    if(ret != SYS_OK)
    {
        pos = 0;
    }

    *len = pos;
    buf[pos] = '\0'; // NULL terminate
    return ret;
}

#define MDNS_MAC_ADDR_STR_LEN  17 // at least "xx:xx:xx:xx:xx:xx"
static int32_t mdns_ipp_txt_mac(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    int32_t ret = SYS_OK;
    int32_t pos = 0;
    uint8_t addr[NET_IFACE_PHYS_ADDR_SIZE];

    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    if(index > 0)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    if(*len-pos > MDNS_MAC_ADDR_STR_LEN && // strlen plus NULL-terminator
       NET_IFACE_OK == net_iface_get_var_phys_addr((uint8_t *)addr, NET_IFACE_PHYS_ADDR_SIZE) )
    {
        sprintf(buf + pos,
                "%02x:%02x:%02x:%02x:%02x:%02x",
                addr[0],
                addr[1],
                addr[2],
                addr[3],
                addr[4],
                addr[5]);
        pos += strlen(buf + pos);
        XASSERT(pos == MDNS_MAC_ADDR_STR_LEN, pos);
    }
    else
    {
        ret = SYS_FAIL; 
        goto done;
    }

done:

    if(ret != SYS_OK)
    {
        pos = 0;
    }

    *len = pos;
    buf[pos] = '\0'; // NULL terminate
    return ret;
}

static int32_t mdns_ipp_txt_uuid(oid_t oid, uint32_t index, char *separator, char *buf, uint32_t *len)
{
    int32_t ret = SYS_OK;
    int32_t pos = 0;

    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    if(index > 0)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    ret = get_device_uuid_string( buf + pos, *len - pos );
    if(ret == SYS_OK)
    {
        pos += strlen(buf + pos);
    }
    else
    {
        ret = SYS_FAIL; 
        goto done;
    }

done:

    if(ret != SYS_OK)
    {
        pos = 0;
    }

    *len = pos;
    buf[pos] = '\0'; // NULL terminate
    return ret;
}

//static error_type_t mdns_get_txt_rdata(char *buf, uint32_t *len)
//{
//    ASSERT(buf);
//    ASSERT(len);
//    ASSERT(*len);
//
//    int32_t error = SYS_OK;
//    uint32_t tmp_len; 
//    uint32_t pos = 0;
//    uint32_t len_pos = 0;
//    mdns_txt_entry_t *txt_entry = NULL;
//    map_iterator_t iter_hndl;
//    ASSERT(g_mdns_txt_map != MAP_INVALID_HANDLE);
//    iter_hndl = map_iterate_begin(g_mdns_txt_map);
//    do
//    {
//        txt_entry = (mdns_txt_entry_t *)map_iterate_next(iter_hndl);
//        if(txt_entry)
//        {
//            // skip len byte
//            len_pos = pos; 
//            pos++;
//
//            tmp_len = *len - pos;
//            error = mdns_get_txt_entry_str(txt_entry, buf+pos, &tmp_len);
//            if(error == SYS_OK)
//            {
//                XASSERT(tmp_len < 255, tmp_len); 
//                *(buf+len_pos) = tmp_len & 0xFF;
//                pos += tmp_len;
//            }
//            else
//            {
//                pos = len_pos;
//                if(error == OID_ERROR_NOT_AVAILABLE)
//                {
//                    // entry not available at this time, e.g. because a subsystem has been temporarily disabled
//                    // skip to next entry
//                    error = SYS_OK;
//                }
//            }
//        }
//    } while(txt_entry && error == SYS_OK);
//    map_iterate_end(iter_hndl);
//
//    if(error)
//    {
//        pos = 0;
//    }
//
//#ifdef MDNS_DEBUG_HEX_DUMP
//    rdata_hex_dump((unsigned char *)buf, (int)pos);
//#endif // MDNS_DEBUG_HEX_DUMP
//
//    XASSERT(pos < *len, pos);
//    *len = pos;
//    buf[pos] = '\0';
//
//    return error;
//}

// inserts an entire NULL-terminated entry string "name=value" 
// buf [out] - caller-provided storage to be filled with null-terminated name/value pair string (i.e. "name=value")
// len [in/out] - total buf length is passed in and must be re-assigned to strlen, not including NULL-terminator
static int32_t mdns_get_txt_entry_str(mdns_txt_entry_t *txt_entry, char *buf, uint32_t *len)
{
    ASSERT(txt_entry);
    ASSERT(buf);
    ASSERT(len);
    ASSERT(*len);

    int32_t ret = SYS_OK;
    uint32_t pos = 0;
    uint32_t name_len;

    ASSERT(txt_entry->name);
    name_len = strlen(txt_entry->name) + 1; // +1 for '='
    if(*len - pos > name_len)
    {
        sprintf(buf + pos, "%s=", txt_entry->name);
        pos += name_len;
    }
    else
    {
        ret = SYS_FAIL;
        goto done; 
    }

    uint32_t val_len;
    if(txt_entry->value)
    {
        // just use the hardcoded value
        ASSERT(txt_entry->oid == OID_INVALID);
        ASSERT(txt_entry->trans_func == NULL);
        val_len = strlen(txt_entry->value);
        if(*len - pos > val_len)
        {
            strcpy(buf + pos, txt_entry->value);
            pos += val_len; 
        }
        else
        {
            ret = SYS_FAIL;
        }
    }
    else
    {
        uint8_t oid_type;
        oid_type = oid_type_id_get(txt_entry->oid); // -1 if attr->oid == OID_INVALID
        uint32_t oid_index = 0;
        uint32_t oid_val = 0;
        uint32_t val_cnt = 0;
        ASSERT(txt_entry->separator);
        uint32_t separator_len = strlen(txt_entry->separator);
        char int_str[MAX_INT_STR_LEN];
        do
        {
            if(txt_entry->trans_func)
            {
                // use the translation func
                val_len = *len - pos;
                ret = txt_entry->trans_func(txt_entry->oid, oid_index, txt_entry->separator, buf+pos, &val_len);
                if(ret == SYS_OK)
                {
                    pos += val_len;
                    val_cnt++;
                }
            }
            else
            {
                // call the OID directly
                switch(oid_type) 
                {
                    case OID_TYPE_STRING:
                    {
                        ret = oid_get_string(txt_entry->oid, oid_index, buf+pos, *len-pos); 
                        if(ret == SYS_OK)
                        {
                            pos += strlen(buf + pos);
                            val_cnt++;
                        }
                        break; 
                    }
                    case OID_TYPE_UINT32:
                    {
                        ret = oid_get_uint32(txt_entry->oid, oid_index, &oid_val);
                        if(ret == SYS_OK)
                        {
                            // convert integer to a string
                            sprintf(int_str, "%u", (unsigned int)oid_val);
                            uint32_t int_str_len = strlen(int_str);
                            XASSERT(MAX_INT_STR_LEN > int_str_len, int_str_len);
                            if(*len - pos > int_str_len)
                            {
                                strcpy(buf + pos, int_str);
                                pos += int_str_len;
                                val_cnt++;
                            }
                            else
                            {
                                ret = SYS_FAIL;
                            }
                        }
                        break;
                    }
                    case OID_TYPE_BYTE_ARRAY:
                        // fall through -- OIDs of type byte array require translation func to convert to string
                    default:
                        // unsupported oid_type
                        XASSERT(0, oid_type);
                        ret = SYS_FAIL;
                        break;
                }
            }

            if(ret == SYS_OK)
            {
                // we got a value so concatenate the separator
                if(*len - pos > separator_len)
                {
                    strcpy(buf + pos, txt_entry->separator);
                    pos += separator_len;
                }
                else
                {
                    ret = SYS_FAIL;
                }
            }
            else if(ret == OID_ERROR_INDEX_UNAVAILABLE)
            {
                // skip this index
            }
            else 
            {
                // we are done adding values -- if we've added at least one value then remove prev separator
                if(val_cnt && pos >= separator_len)
                {
                    pos -= separator_len;
                }
            }

            oid_index++;

        } while(ret == SYS_OK || ret == OID_ERROR_INDEX_UNAVAILABLE); 

        if(ret == OID_ERROR_INDEX_INVALID)
        {
            ret = SYS_OK;
        }
    }

done:

    if(ret != SYS_OK)
    {
        pos = 0;
    }

    buf[pos] = '\0'; // NULL terminate the value string
    *len = pos;

    return ret;
}

/*----------------------------------------------------------------------------*/
static error_type_t _ipp_tcp_txt_record(int index, char** txt_record)
{
   uint32_t oid_error;

   // Populate all the strings as appropriate.
   sprintf(mdns_txt_bind,    "%s%s", MDNS_TXT_BIND_STR,    get_MDNS_TXT_BIND());
   sprintf(mdns_txt_collate, "%s%s", MDNS_TXT_COLLATE_STR, get_MDNS_TXT_COLLATE());
   sprintf(mdns_txt_color,   "%s%s", MDNS_TXT_COLOR_STR,   get_MDNS_TXT_COLOR());
   sprintf(mdns_txt_copies,  "%s%s", MDNS_TXT_COPIES_STR,  get_MDNS_TXT_COPIES());
   sprintf(mdns_txt_duplex,  "%s%s", MDNS_TXT_DUPLEX_STR,  get_MDNS_TXT_DUPLEX());
   sprintf(mdns_txt_punch,   "%s%s", MDNS_TXT_PUNCH_STR,   get_MDNS_TXT_PUNCH());
   sprintf(mdns_txt_scan,    "%s%s", MDNS_TXT_SCAN_STR,    get_MDNS_TXT_SCAN());
   sprintf(mdns_txt_fax,     "%s%s", MDNS_TXT_FAX_STR,     get_MDNS_TXT_FAX());
   sprintf(mdns_txt_sort,    "%s%s", MDNS_TXT_SORT_STR,    get_MDNS_TXT_SORT());
   sprintf(mdns_txt_staple,  "%s%s", MDNS_TXT_STAPLE_STR,  get_MDNS_TXT_STAPLE());

   populate_pdl_str();
   populate_adminurl_str();
   populate_usb_CMD_str();

   static char* _records[] = 
   {
      "txtvers=1",    
      "qtotal="MDNS_TXT_QTOTAL,      
      pdl_str,
      "rp="MDNS_TXT_RP,          
      ty_printer,
      product,
      usb_mfg,
      usb_mdl,
      usb_CMD,     
      "priority="MDNS_TXT_PRIORITY,    
      mac,
      adminurl,
      uuid,
      "Transparent="MDNS_TXT_TRANSPARENT, 
      "Binary="MDNS_TXT_BINARY,      
      mdns_txt_bind,
      mdns_txt_collate,     
      mdns_txt_color,
      mdns_txt_copies,
      mdns_txt_duplex,
      "PaperCustom="MDNS_TXT_PAPERCUSTOM, 
      "PaperMax=legal-A4",
      mdns_txt_punch,
      mdns_txt_scan,
      mdns_txt_fax,
      mdns_txt_sort,
      mdns_txt_staple,
   };
    #define NUM_TXT_RECORDS (sizeof(_records)/sizeof(_records[0]))

    if ((index >= 0) && (index < NUM_TXT_RECORDS))
    {
        *txt_record = _records[index];
        return OK;
    }
    // Dynamic value for note
    if ( index == NUM_TXT_RECORDS)
    {
       char     *note_str = "note=";
       uint32_t note_str_len = strlen(note_str);

       strncpy( &location_note[0], note_str, note_str_len );
       location_note[note_str_len] = 0;

       oid_error = oid_get_string(OID_SM_SYSTEM_LOCATION,
                                  0,
                                  &location_note[note_str_len],
                                  sizeof(location_note) - note_str_len );
       ASSERT(oid_error == OK);

        *txt_record = location_note;
        return OK;
    }

    return FAIL;
}


static error_type_t _ipp_sub_types( int index, char **sub_type )
{
    if ((index >= 0) && (index < num_fixed_IPP_sub_types))
    {
        *sub_type = ipp_sub_types[index];
        return OK;
    }

    if (index == num_fixed_IPP_sub_types)
    {
       error_type_t error;
       uint32_t     airprint_enabled = 0;

       // Determine whether Airprint is enabled.
       error = oid_get_uint32(OID_SM_AIRPRINT_ENABLED, 0, &airprint_enabled);
       ASSERT(error == OK);

        if (airprint_enabled)
        {
            *sub_type = "_universal";
            return OK;
        }
    }
    
    return FAIL;
}


static char* get_MDNS_TXT_FAX()
{
   return(get_feature_support(OID_FAX_SUPPORT));
}

static char* get_MDNS_TXT_SCAN()
{
   return(get_feature_support(OID_SCAN_SUPPORT));
}

static char* get_MDNS_TXT_DUPLEX()
{
   return(get_feature_support(OID_DUPLEXER_SUPPORT));
}

static char* get_MDNS_TXT_COLOR()
{
   return(get_feature_support(OID_PRINT_HAVE_COLOR));
}

static char* get_MDNS_TXT_COPIES()
{
   return(get_feature_support(OID_SCAN_SUPPORT));
}

static char* get_MDNS_TXT_COLLATE()
{
   return(get_feature_support(OID_PRINT_COLLATE_SUPPORT));
}

static char* get_MDNS_TXT_BIND()
{
   return(get_feature_support(OID_PRINT_BIND_SUPPORT));
}

static char* get_MDNS_TXT_PUNCH()
{
   char     punch_val[2]={0};
   uint32_t error;

   // This is defined as being only one character.
   error = oid_get_string(OID_PRINT_PUNCH_SUPPORT, 0, &punch_val[0], 1);
   ASSERT(error == OK);

   // This is the value for no punch supported from the RFC 2911 specification.
   if (punch_val[0] == '3')
   {
      return(FEATURE_NOT_SUPPORTED);
   }

   return(FEATURE_SUPPORTED);
}

static char* get_MDNS_TXT_SORT()
{
   return(get_feature_support(OID_PRINT_SORT_SUPPORT));
}

static char* get_MDNS_TXT_STAPLE()
{
   return(get_feature_support(OID_PRINT_STAPLE_SUPPORT));
}


static char* get_feature_support(uint32_t OID)
{
   uint32_t oid_val = 0;
   char     *ret_val = FEATURE_NOT_SUPPORTED;
   uint32_t error;
   
   error = oid_get_uint32(OID, 0, &oid_val);                                         
   ASSERT(error == OK);

   if (oid_val > 0)
   {
      ret_val = FEATURE_SUPPORTED;
   }

   return(ret_val);
}



/******************************************************************************
*  GLOBAL FUNCTIONS
******************************************************************************/
#define TEMP_BUFFER_MAX_SIZE        80
void net_mdns_oem_pre_init(void)
{
    uint32_t oid_error;
    char     temp[TEMP_BUFFER_MAX_SIZE] = "";
    char     prt_name[FRIENDLY_NAME_MAX_SIZE];
    char     mfg_name[MFG_NAME_MAX_SIZE];
    char     bonjour_prod_str[sizeof(product)];
    char *vendorName=NULL;
    char *ModelName=NULL;
    error_type_t db_retval = FAIL;
    
    temp[0] = 0;
        
    populate_adminurl_str();
    
    /*oid_error = oid_get_string(OID_PLATFORM_PRINT_NAME_STRING, 
                               0,
                               prt_name, 
                               sizeof(prt_name));
                               */
    vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
    ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
    //remove_spaces(ModelName);
    // if(0==strcmp(vendorName,DEFAULT_VENDOR))
    if(0==strcasecmp(vendorName,DEFAULT_VENDOR)) // by mspyo, 2021-03-15
    {
        oid_error = oid_get_string(OID_PLATFORM_USB_MFG_STRING,
                                   0,
                                   mfg_name,
                                   sizeof(mfg_name));
        ASSERT(oid_error == OK);
    	getPrinterName(prt_name);
    }
    else
	{
    	strncpy(mfg_name,vendorName,strlen(vendorName));
		sprintf(prt_name,"%s %s", vendorName, ModelName);
	}

    ASSERT(oid_error == OK);

    sprintf(usb_mdl, "%s=%s", USB_MDL_KEY, prt_name);
    sprintf(ty_printer, "%s=%s", TY_KEY, prt_name);
    sprintf(usb_mfg, "%s=%s", USB_MFG_KEY, mfg_name);
    sprintf(mfg, "%s=%s", MFG_KEY, mfg_name);
    sprintf(mdl, "%s=%s", MDL_KEY, prt_name);

    // if(0==strcmp(vendorName,DEFAULT_VENDOR))
    if(0==strcasecmp(vendorName,DEFAULT_VENDOR)) // by mspyo, 2021-03-15
    {
		oid_error = oid_get_string(OID_SM_SYSTEM_MAKE_AND_MODEL,
								   0,
								   bonjour_prod_str,
								   sizeof(bonjour_prod_str));
    }
    else
    {
    	sprintf(bonjour_prod_str,"%s %s", vendorName, ModelName);
    }
	MEM_FREE_AND_NULL(vendorName);
	MEM_FREE_AND_NULL(ModelName);
    ASSERT(oid_error == OK);
    
    sprintf(product, "%s=(%s)", PRODUCT_KEY, bonjour_prod_str);
    
    if ( NET_IFACE_OK == net_iface_get_var_phys_addr((uint8_t *)temp,
                                                     NET_IFACE_PHYS_ADDR_SIZE) )
    {
       sprintf( mac,
                "%s=%02x:%02x:%02x:%02x:%02x:%02x",
                MAC_KEY,
                temp[0],
                temp[1],
                temp[2],
                temp[3],
                temp[4],
                temp[5] );
    }
    else
    {
       mac[0] = '\0';
    }

    get_device_uuid_string( temp, sizeof( temp ) );
    sprintf( uuid, "%s=%s", UUID_KEY, temp );
}

error_type_t net_mdns_config_num_txt_records(const char* type, uint32_t *num_txt_records)
{
    *num_txt_records = 1;
    return OK;
}

error_type_t net_mdns_config_txt_record(const char* service, 
                                        uint32_t txt_record_set_index,
                                        uint32_t txt_element_index,
                                        char** txt_record)
{
    int i;
    net_mdns_oem_pre_init();

    XASSERT( txt_record_set_index == 0, txt_record_set_index );

    for (i=0;i<sizeof(_handlers)/sizeof(_handlers[0]);i++)
    {
        if (strcmp(_handlers[i].service, service) == 0)
        {
            return _handlers[i].txt_handler(txt_element_index, txt_record);
        }
    }
    return FAIL;
}

//error_type_t net_mdns_config_services( uint32_t index, net_mdns_service_t *service_type )
//{
//    if ( index >= num_service_types )
//    {
//        return FAIL;
//    }
//
//    *service_type = types[index];
//    return OK;
//}

error_type_t net_mdns_config_sub_types(const char* service, uint32_t index, char** sub_type)
{
    int i;

    for (i=0;i<sizeof(_handlers)/sizeof(_handlers[0]);i++)
    {
        if (strcmp(_handlers[i].service, service) == 0)
        {
            return _handlers[i].sub_type_handler( index, sub_type );
        }
    }
    return FAIL;
}

error_type_t net_mdns_config_num_sub_types(const char* service, uint32_t *num_sub_types)
{
    int i;

    *num_sub_types = 0;

    for (i = 0;i < sizeof(_handlers)/sizeof(_handlers[0]); ++i)
    {
       if (strcmp(_handlers[i].service, service) == 0)
        {
          if (_handlers[i].func_get_num_sub_types != NULL)
          {
              *num_sub_types = _handlers[i].func_get_num_sub_types();
          }
          else
          {
              *num_sub_types = 0;
          }

          return OK;
        }
    }
    return FAIL;
}

error_type_t net_mdns_config_service_name_prefix(char** prefix)
{
    *prefix = "";
    return OK;
//    vpi_prt_rcode_t vpi_ret;
//    
//    *prefix = NULL;
//    
//    vpi_ret = vpi_prt_get_printer_prefix(prefix);
//    if (vpi_ret == VPI_PRT_OK)
//    {
//        return OK;
//    }
//    
//    return FAIL;
}

void net_mdns_config( void )
{
    map_init(); // TODO do this is a sysinit routine
    g_mdns_txt_map = map_create(0);
    mdns_txt_add_entries(g_mdns_txt_entries_ipp, g_mdns_txt_entry_cnt_ipp);
#ifdef HAVE_AIRPRINT // TODO make this a registered callback vs. static call
    void airprint_mdns_txt_add_entries(map_handle_t mdns_txt_map);
    airprint_mdns_txt_add_entries(g_mdns_txt_map);
#endif // HAVE_AIRPRINT
//    config->func_table->services = net_mdns_config_services;
//    config->func_table->num_txt_records = net_mdns_config_num_txt_records;
//    config->func_table->txt_entry = net_mdns_config_txt_record;
//    config->func_table->num_sub_types = net_mdns_config_num_sub_types;
//    config->func_table->sub_types = net_mdns_config_sub_types;
//    
//    config->func_table->service_name_prefix = net_mdns_config_service_name_prefix;
//    config->func_table->get_txt_rdata = mdns_get_txt_rdata;
}

#ifdef MDNS_DEBUG_HEX_DUMP
#include <ctype.h> // for isprint()
static void rdata_hex_dump( unsigned char *ptr, int size ) 
{
    static char hex_ascii[] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr;

    endptr = ptr + size;
    memset( line, ' ', 80 );
    line[69] = 0;
    while( ptr != endptr ) {
        hex = &line[2];
        ascii = &line[52];
        for( i = 0; i < 16; i++ ) {
            if( isprint( *ptr ) ) {
                *ascii++ = *ptr;
            }
            else {
                *ascii++ = '.';
            }
            *hex++ = hex_ascii[*ptr >> 4];
            *hex++ = hex_ascii[*ptr & 0x0f];
            *hex++ = ' ';
            ptr++;
            if( ptr == endptr ) {
                /* clean out whatever is left from the last line */ 
                memset( hex, ' ', ( 15 - i ) * 3 );
                memset( ascii, ' ', 15 - i );
                /* i+1 so we can find the starting pointer correctly in our
                 * ptr-i in the printf below
                 */
                i++;
                break;
            }
        }
        dbg_printf("%s\n", line );
    }
}
#endif // MDNS_DEBUG_HEX_DUMP

#ifdef __cplusplus
}
#endif

// eof net_mdns_config.c
