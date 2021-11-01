/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

//=========================================================================================
// Includes
//=========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "posix_ostools.h"
#include "ioutils.h"
#include "sm_job_api.h"
#include "memAPI.h"
#include "debug.h"
#include "ipp_api.h"
#include "logger.h"
#include "net_logger.h"
#include "ipp.h"
#include "ipp_types.h"
#include "ipp_const.h"
#include "ipp_attribute.h"
#include "ipp_dbg.h"
#include "ipp_cntxt.h"
#include "ipp_request.h"
#include "ipp_printer_requested_attr.h"
#include "ipp_job_requested_attr.h"
#include "ipp_validate_request.h"
#include "ipp_config_attr.h"
#include "http_api.h"
#include "net_iface_vars.h"
#include "oid_api.h"
#include "oid_sm_api.h"
#include "db_api.h"
//#define ENABLE_SINDOH_MOBILE_SCAN
#ifdef ENABLE_SINDOH_MOBILE_SCAN
#include "SysUI_Interface.h"
#endif
#ifdef ENABLE_SINDOH_MOBILE_APP
#include "file_to_pipe_api.h"
#include "agConnectMgr.h"
#include "stdlib.h"
#include "SysSharedMemory.h"
#include "posix_ostools.h"
#include "sys/statvfs.h"
#include "sys/stat.h"
#include "commonutils.h"
#define MOBILE_PRINT_FILEPATH "/tmp/sMobilePrint"
#define MAX_SPRINT_FILES 16
#define DEFAULT_VENDOR "SINDOH" // Map with previous BI/CI by mspyo, 2021-03-15

pthread_mutex_t _ipp_mutex;
bool isSprintBusy;
#ifdef ENABLE_SINDOH_MOBILE_SCAN
int startscan(stShmSendServer *sendServerInfo,stScan_GoScan *gGoScan);
void ipp_getMobileScanApp_Attr(ipp_attr_groups_t *grps, stScan_GoScan *pPrintAddInfo, stShmSendServer *sendServerInfo);

#endif
void ipp_lock(int Lock_nUnlock)
{
  if (Lock_nUnlock) 
    pthread_mutex_lock(&_ipp_mutex);
  else
    pthread_mutex_unlock(&_ipp_mutex);
}

#endif

//=========================================================================================
// Defines
//=========================================================================================

#define DBG_PRFX "IPP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IPP 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)

// This is larger than what is really needed for the host name but it is a good boundary.
#define MAX_HOSTNAME_SIZE   IPP_MAX_HOSTNAME_SIZE // from ipp_api.h

// Room for the required 45-octet string "urn:uuid:8-4-4-4-12" + NULL on
// a good bounday.
#define JOB_UUID_STR_LEN 64

#define JOB_ID_STR "job-id:"

// For turning on various forms of debug.
#define _DUMP_RAW (0)
#define _DUMP_IPP (0)
#define IPP_DEBUG_JOBS (0)

#define MAX_OID_INDEX 100

//=========================================================================================
// Macros
//=========================================================================================
#define _add_str_attr(grp, label, tag, fmt, ...) do {          \
  char* __str__ = (char*)IPP_MALLOC(1024);                     \
  ASSERT(__str__);                                             \
  int   __strlen__ = sprintf(__str__, fmt, ## __VA_ARGS__);    \
  ipp_add_attr_with_val(grp, label, tag, __str__, __strlen__); \
  IPP_FREE(__str__);                                  \
                                                    } while(0)

//=========================================================================================
// Local variables
//=========================================================================================

// Whether we support adding multiple documents per job.
static const bool allow_multiple_docs_per_job = false;

//=========================================================================================
// Local functions
//=========================================================================================
static ipp_requested_attrs_t *_get_the_req_attributes(ipp_request_t *ipp_req);
static void _free_the_req_attributes(ipp_requested_attrs_t *attributes);
static ipp_attr_t* _get_job_uuid(uint32_t job_id);
static int _create_job(ipp_cntxt_t* cntxt);
static int _send_document(ipp_cntxt_t* cntxt);
static void dump_data_and_send_complete(ipp_cntxt_t* cntxt);

#ifdef ENABLE_SINDOH_MOBILE_APP
#define MOBILE_PRINT_BUF_SIZE (1024*1024)
static void cbMobilePrintDone( char *path, void *context )
{
    if(path)
    {
        unlink(path);
    }
    char fileName[256]={0};
    if(context && ((stShmGoPrint *)context)->fileName)
    {
        snprintf(fileName,sizeof(fileName),"%s/%s",MOBILE_PRINT_FILEPATH,((stShmGoPrint *)context)->fileName);
        FILE *fpprintFile;
        if ((fpprintFile = fopen(fileName, "w+")) != NULL)
        {
          fclose(fpprintFile);
          //unlink(fileName);  //lsof returns file still open..so just clearing file content and not deleting the file..need to be checked
          sync();
        }
        else
        {
          printf("\n[%s] does not exist",fileName);
        }
    }
    if(context)
    {
        free(context);
        context=NULL;
    }
    isSprintBusy=false;
    return;
}
#endif

static bool get_uint32_oid(oid_t oid, uint32_t index, uint32_t *oid_val)                        
{
    uint32_t error = oid_get_uint32(oid, index, oid_val); 
    if (error != OK)
        DBG_ERR( "get_uint32_oid: ERROR - unable to get UINT32 for OID=%d, index=%d\n", oid, index);
        
    return (error == OK);
}

static bool get_string_oid(oid_t oid, uint32_t index,  char *str_ptr, uint32_t str_len)                        
{
    uint32_t error = oid_get_string(oid, index, str_ptr, str_len); 
    if (error != OK)
    {
        DBG_MSG( "get_string_oid: WARNING - unable to get string for OID=%d, index=%d\n", oid, index);
    }   

    return (error == OK);
}

static bool get_byte_array_oid(oid_t oid, uint32_t index,  uint8_t *bytearray, uint32_t *array_len_in_bytes)                        
{
    uint32_t error = oid_get_bytearray(oid, index, bytearray, array_len_in_bytes); 
    if (error != OK)
        DBG_ERR( "get_byte_array_oid: ERROR - unable to get UINT32 for OID=%d, index=%d\n", oid, index);
        
    return (error == OK);
}

static unsigned int _get_unum(unsigned char** data, int* length, int num)
{
  unsigned int op = 0;
  if (*length >= num)
  {
    while (num--)
    {
      op <<= 8;
      op  |= (*data)[0];
      *data += 1;
      *length -= 1;
    }
  }
  return op;
}

static unsigned int _parse_unum(unsigned char* data, int datalen, int size)
{
  return _get_unum(&data, &datalen, size);
}

static void _dump_data(unsigned char* buf, int length)
{
  if (!_DUMP_RAW) return;
  while (length)
  {
    unsigned char* ptr = buf;
    int i;
    for (i=0;i<16 && i<length;i++)
      _ipp_dbg_prt(" %2.2x", *ptr++);
    for (   ;i<16 && i<length;i++)
      _ipp_dbg_prt("   ");
    _ipp_dbg_prt(" | ");
    ptr = buf;
    for (i=0;i<16 && i<length;i++)
      _ipp_dbg_prt("%c", isprint(*ptr++)?ptr[-1]:'.');
    _ipp_dbg_prt("\n");
    length -= i;
    buf += i;
  }
}

static void _dump_ipp(unsigned char* data, int len, int req_nResp)
{
  if (!_DUMP_IPP) return;
  _dump_data(data, len);
  ipp_u16bit_t       vers = _get_unum(&data, &len, 2);
  ipp_u16bit_t       opst = _get_unum(&data, &len, 2);
  ipp_u32bit_t       reqi = _get_unum(&data, &len, 4);
  ipp_attr_groups_t *grps = ipp_parse_attr_groups((void**)&data, &len);

                 ipp_dbg("Version:%4.4x", vers);
  if (req_nResp) ipp_dbg("OPID:   %4.4x (%s)", opst,ipp_const_opid_str(opst));
  else           ipp_dbg("Status: %4.4x (%s)", opst,ipp_const_status_str(opst));
                 ipp_dbg("ReqID:  %8.8x", reqi);
  if (grps)
  {
    ipp_dump_attr_groups(grps);
    ipp_free_attr_groups(grps, 1);
  }
  else
    ipp_dbg("No attribute groups found\n");
}

#if IPP_DEBUG_JOBS
static void _debug_job(uint32_t job_id)
{
    smjob_rcode_t smjob_rcode;
    smjob_status_t s;

    ipp_dbg("Job ID:    %d", job_id);

    smjob_rcode = smjob_get_status(job_id, &s);
    if(smjob_rcode != SMJOB_OK)
    {
        goto done;
    }

    XASSERT(job_id == s.job_id, s.job_id);
    ipp_dbg("Job State: %d", s.state);

    smjob_ticket_t t;
    smjob_rcode = smjob_get_ticket(job_id, &t);
    if(smjob_rcode != SMJOB_OK)
    {
        goto done;
    }

    ipp_dbg(" Job Name: %s", t.job_description.job_name);
    ipp_dbg(" Job User: %s", t.job_description.originating_user_name);
    ipp_dbg(" Copies:   %d", t.doc_processing.type.print.copies);
    ipp_dbg(" Collate:  %d", t.doc_processing.type.print.sheet_collate);

done:
    return;
}

static void _debug_jobs(void)
{
    uint32_t job_id = smjob_get_next(SMJOB_INVALID_JOB_ID);
    while(job_id != SMJOB_INVALID_JOB_ID)
    {
        _debug_job(job_id);
        job_id = smjob_get_next(job_id);
    }
    return;
}
#endif // IPP_DEBUG_JOBS

static ipp_attr_grp_t* _default_operation_attributes(void)
{
  char def_oid_val[128] = {0};

  ipp_attr_grp_t* grp = ipp_create_attr_group(IPP_TAG_OPERATION_ATTRIBUTES);

  memset(&def_oid_val[0], 0, sizeof(def_oid_val));
  if (get_string_oid(OID_SM_CHARSET_CONFIGURED, 
                 0, 
                 &def_oid_val[0], 
                 sizeof(def_oid_val)))
  {                 
     ipp_add_attr_with_val(grp, 
                        "attributes-charset",
                        IPP_TAG_CHARSET, 
                        &def_oid_val[0], 
                        strlen(&def_oid_val[0]));
  }
  
  memset(&def_oid_val[0], 0, sizeof(def_oid_val));
  if (get_string_oid(OID_SM_NATURAL_LANGUAGE_CONFIGURED, 
                 0, 
                 &def_oid_val[0], 
                 sizeof(def_oid_val)))
  {
     ipp_add_attr_with_val(grp, 
			"attributes-natural-language",
                        IPP_TAG_NATURAL_LANGUAGE,
			&def_oid_val, 
			strlen(&def_oid_val[0]));
  }
  
  return grp;
}

/*----------------------------------------------------------------------------*/
static ipp_rcode_t _extract_preparse(ipp_attr_groups_t *grps, const char* attrname, uint32_t index, ipp_attr_t** attr)
{
  ipp_rcode_t ipp_rcode = IPP_OK;
  
  *attr = ipp_find_attr(grps, attrname);
  
  if (!*attr)
  {
      ipp_rcode = IPP_ATTR_NOT_FOUND;
      goto done;
  }
  
  if (index > (*attr)->num_val)
  {
      ipp_rcode = IPP_INVALID_INDEX;
      goto done;
  }

done:
  return ipp_rcode;
}

static ipp_rcode_t _extract_string(ipp_attr_groups_t *grps, const char* attrname, uint32_t index, char *buf, uint32_t len)
{
  ipp_rcode_t ipp_rcode = IPP_OK;
  ipp_attr_t* attr = NULL;
  
  ipp_rcode = _extract_preparse(grps, attrname, index, &attr);
  if (ipp_rcode != IPP_OK)
  {
    return ipp_rcode;
  }
    
  ipp_attr_val_t  *val = &attr->val[index];
  
  switch(val->syntax)
  {
      case IPP_TAG_CHARSET:
      case IPP_TAG_NATURAL_LANGUAGE:
      case IPP_TAG_MIME_MEDIA_TYPE:
      case IPP_TAG_KEYWORD:
      case IPP_TAG_URI:
      case IPP_TAG_URI_SCHEME:
      case IPP_TAG_TEXT_WITHOUT_LANGUAGE:
      case IPP_TAG_NAME_WITHOUT_LANGUAGE:
      {
        if (val->len > len)
        {
            ipp_rcode = IPP_INVALID_LENGTH;
        }
        else
        {
            memset(buf, 0, len);
            memcpy(buf, val->data, attr->val->len);
        }
          break;
      }

      default:
          ipp_rcode = IPP_FAIL;
          break;
  }

  return ipp_rcode;
}

static ipp_rcode_t _extract_boolean(ipp_attr_groups_t *grps, const char* attrname, uint32_t index, bool *value)
{
  ipp_rcode_t ipp_rcode = IPP_OK;
  ipp_attr_t* attr = NULL;
  
  ipp_rcode = _extract_preparse(grps, attrname, index, &attr);
  if (ipp_rcode != IPP_OK)
  {
    return ipp_rcode;
  }
    
  ipp_attr_val_t  *val = &attr->val[index];

  switch(val->syntax)
  {
      case IPP_TAG_BOOLEAN:
      {
        unsigned char *d = val->data;
        
        if (val->len != 1)
          ipp_rcode = IPP_INVALID_LENGTH;
        else if (d[index] > 1)
          ipp_rcode = IPP_FAIL;
        else
          *value = d[index];
        break;
      }

      default:
        ipp_rcode = IPP_FAIL;
        break;
  }

  return ipp_rcode;
}

static ipp_rcode_t _extract_integer(ipp_attr_groups_t *grps, 
                                    const char        *attrname, 
                                    uint32_t          index, 
                                    uint32_t          *value)
{
  ipp_rcode_t ipp_rcode = IPP_OK;
  ipp_attr_t* attr = NULL;
  
  ipp_rcode = _extract_preparse(grps, attrname, index, &attr);
  if (ipp_rcode != IPP_OK)
  {
    return ipp_rcode;
  }
  
  ipp_attr_val_t  *val = &attr->val[index];
  
  switch(val->syntax)
  {
      case IPP_TAG_INTEGER:
      case IPP_TAG_BOOLEAN:
      {
        *value = _parse_unum(val->data, val->len, val->len);
        break;
      }

      default:
        ipp_rcode = IPP_FAIL;
        break;
  }

  return ipp_rcode;
}

/*----------------------------------------------------------------------------*/
static int _validate_job(ipp_cntxt_t* cntxt, bool send_doc)
{
    ipp_rcode_t ipp_rcode = IPP_OK;
    int status = IPP_STAT_OK;

    /* JSA - eventually we would like to levergae as much of_print_job as
           possible. Perhaps it makes sense, I suppose, for _print_job to
           internally call _validate_job before processing the job payload */

    bool valid_attr_req = false;
    
    if (send_doc)
        valid_attr_req = ipp_chk_valid_send_document_attr_req(cntxt->reqattr);
    else
        valid_attr_req = ipp_chk_valid_print_job_attr_req(cntxt->reqattr);
    

    if (!valid_attr_req)
    {
        status = IPP_STAT_CLI_BAD_REQUEST;
        
        goto validate_job_done;
    }
  
    char document_format[SMJOB_MAX_NAME_LEN];
    memset(document_format, 0, SMJOB_MAX_NAME_LEN);
    ipp_rcode = _extract_string(cntxt->reqattr,
                      "document-format", 
                      0,
                      document_format, 
                      SMJOB_MAX_NAME_LEN-1);
    if (ipp_rcode == IPP_OK)
    {
        // if document-format is found, validate against our
        // supported formats
        uint32_t        oid_index = 0;
        error_type_t    error     = OK;
        char oid_str[OID_BUF_MAX_LEN];

        while (true)
        {
            error = oid_get_string(OID_SM_PRINT_DOC_FORMAT_SUPPORTED, 
                             oid_index, 
                             oid_str, 
                             OID_BUF_MAX_LEN);

            if (error == OK)
            {
                if (strcmp(document_format, oid_str) == 0)
                {
                    break;
                }
            }
            // We only break out when the index is invalid.
            else if (error == OID_ERROR_INDEX_INVALID)
            {
                status = IPP_STAT_CLI_DOCUMENT_FORMAT_NOT_SUPPORTED;
                break;
            }
            else
            {
                // should never get here
                ASSERT(0);
                break;
            }

            ++oid_index;
        }
    }
  
validate_job_done:
           
  return status;
}
  
static bool is_utf8(const char * string)
{
    if(!string)
        return 0;

    const unsigned char * bytes = (const unsigned char *)string;
    while(*bytes)
    {
        if( (// ASCII
             // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
        ) {
            bytes += 1;
            continue;
        }

        if( (// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
        ) {
            bytes += 2;
            continue;
        }

        if( (// excluding overlongs
                bytes[0] == 0xE0 &&
                (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            )
        ) {
            bytes += 3;
            continue;
        }

        if( (// planes 1-3
                bytes[0] == 0xF0 &&
                (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            )
        ) {
            bytes += 4;
            continue;
        }

        return 0;
    }

    return 1;
}


static int _print_job(ipp_cntxt_t* cntxt)
{
  smjob_rcode_t      smjob_rcode;
  smjob_ticket_t     job_ticket;
  smjob_doc_ticket_t doc_ticket;
  uint32_t           doc_id;
  ipp_rcode_t        iret;
  uint32_t           job_id = SMJOB_INVALID_JOB_ID;
  int                status = IPP_STAT_OK;
  bool               add_doc = false;
  ipp_request_t *ipp_req = cntxt->ipp_req;
  
  ASSERT(ipp_req);

  DBG_VERBOSE("%s - entered....\n", __func__);

  smjob_rcode = smjob_get_default_ticket(SMJOB_TYPE_PRINT, &job_ticket);
  XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
  
  iret = _extract_string(cntxt->reqattr,
                  "job-name", 
                  0,
                  job_ticket.job_description.job_name, 
                  SMJOB_MAX_NAME_LEN);
  if (iret != IPP_OK)
  {
    strncpy(job_ticket.job_description.job_name, "unknown", SMJOB_MAX_NAME_LEN);
  }  

  // make sure job-name is utf8
  if (!is_utf8(job_ticket.job_description.job_name))
  {
      DBG_ERR("req %d: non utf8 job-name! (%s)\n", 
              ipp_req->http_hndl, job_ticket.job_description.job_name);

      status = IPP_STAT_CLI_DOCUMENT_FORMAT_ERROR;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
   }

   status = _validate_job(cntxt, false);
   if (status != IPP_STAT_OK)
   {
        dump_data_and_send_complete(cntxt);
        
        goto send_attributes;
   }

  ipp_rcode_t ipp_rcode = _extract_string(cntxt->reqattr,
                                          "requesting-user-name", 
                                          0,
                                          job_ticket.job_description.originating_user_name, 
                                          SMJOB_MAX_NAME_LEN);
  if (ipp_rcode == IPP_ATTR_NOT_FOUND)
  {
    strncpy(job_ticket.job_description.originating_user_name,
            "anonymous",
            SMJOB_MAX_NAME_LEN);
  }
  else if (ipp_rcode != IPP_OK)
  {
    status = IPP_STAT_CLI_BAD_REQUEST;

    goto send_attributes;
  }

  // Add the requested changes to the job ticket.
  ipp_add_req_attr_to_job_ticket(cntxt->reqattr, &job_ticket);

  // See if the ipp-attribute-fidelity is present or not. If not then set a default
  // value of false.
  iret = _extract_integer(cntxt->reqattr, 
                          "ipp-attribute-fidelity",
                          0,
                          &job_ticket.ipp_attribute_fidelity);
  if (iret != IPP_OK)
  {
     DBG_MSG("%s - did not find ipp-attribute-fidelity, default to 0\n", __func__);
     job_ticket.ipp_attribute_fidelity = 0;
  }

  if (smjob_accepting_jobs())
  {
      job_id = smjob_create(&job_ticket, 
                            ipp_smjob_status_event, 
                            SMJOB_STATUS_EVENT_STATE, 
                            (void *)ipp_req);

      if(job_id == SMJOB_INVALID_JOB_ID)
      {
          DBG_ERR("req %d: invalid job id!\n", 
                  ipp_req->http_hndl);

          status = IPP_STAT_CLI_NOT_FOUND;

          dump_data_and_send_complete(cntxt);

          goto send_attributes;
      }
  }
  else
  {
    DBG_ERR("req %d: not accepting jobs!\n", ipp_req->http_hndl);

    status = IPP_STAT_SRV_NOT_ACCEPTING_JOBS;

    dump_data_and_send_complete(cntxt);

    goto send_attributes;
  }
  
  ipp_req->job_id = job_id;

  smjob_rcode = smjob_get_default_doc_ticket(job_id, &doc_ticket);
  //XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
  if(smjob_rcode != SMJOB_OK)
  {
      DBG_ERR("req %d: unable to get default document ticket!\n", 
              ipp_req->http_hndl);

      smjob_cancel(job_id);
         
      status = IPP_STAT_CLI_DOCUMENT_ACCESS_ERROR;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
  }

  doc_ticket.doc_description.type.print.recv_func = ipp_smjob_reader;
  doc_ticket.doc_description.type.print.user_data = ipp_req;

   // Add the user requested changes to the document ticket.
   ipp_add_req_attr_to_doc_ticket(cntxt->reqattr, &doc_ticket);

  // make sure job data is flowing before kicking connection mgr by adding doc
  DBG_MSG("req %d: print job waiting for initial data...\n", ipp_req->http_hndl);
 uint32_t start_time = posix_gettime_ticks();
 UNUSED_VAR(start_time);

  ipp_req->tmp_data_len = ipp_reader((void *)ipp_req, 
                                            (void *)ipp_req->tmp_data,
                                             IPP_TMP_DATA_SIZE);

  if(ipp_req->tmp_data_len <= 0)
  {
      DBG_ERR("req %d: print job timeout! (ms=%d)\n", 
              ipp_req->http_hndl, (posix_gettime_ticks() - start_time)*10);
      smjob_cancel(job_id);
      status = IPP_STAT_CLI_DOCUMENT_ACCESS_ERROR;

      // Treat this timeout as an HTTP error
      ipp_req->job_http_error = true;
      ipp_request_processing_complete(ipp_req);

      goto send_attributes;
  }

  add_doc = true;

  DBG_MSG("req %d: print job got initial data (len=%d ms=%d)\n", 
          ipp_req->http_hndl, ipp_req->tmp_data_len, (posix_gettime_ticks() - start_time)*10);
  
send_attributes:

  if (add_doc)
  {
      doc_id = smjob_add_document(job_id, &doc_ticket, true);
      if(doc_id == SMJOB_INVALID_DOC_ID)
      {
         DBG_ERR("req %d: Failed to add document!\n", 
                  ipp_req->http_hndl);
                  
         smjob_cancel(job_id);
         
         status = IPP_STAT_SRV_INTERNAL_ERROR;
      }
  }
  
  if (job_id != SMJOB_INVALID_JOB_ID)
    DBG_MSG("req %d: created job %d\n", ipp_req->http_hndl, job_id);
  else
    DBG_MSG("req %d: failed to create job\n", ipp_req->http_hndl);
   
#if IPP_DEBUG_JOBS
  _debug_jobs();
#endif // IPP_DEBUG_JOBS

  return status;
}


int _create_job(ipp_cntxt_t* cntxt)
{
   smjob_rcode_t  smjob_rcode;
   smjob_ticket_t job_ticket;
   uint32_t       job_id = SMJOB_INVALID_JOB_ID;
   ipp_rcode_t    iret;
   int            status = IPP_STAT_OK;
   ipp_request_t *ipp_req = cntxt->ipp_req;
  
   ASSERT(ipp_req);

   DBG_VERBOSE("%s - entered....\n", __func__);

   smjob_rcode = smjob_get_default_ticket(SMJOB_TYPE_PRINT, &job_ticket);
   XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);

   iret = _extract_string(cntxt->reqattr,
                          "job-name",
                          0,
                          job_ticket.job_description.job_name,
                          SMJOB_MAX_NAME_LEN);
   if (iret != IPP_OK)
   {
      strncpy(job_ticket.job_description.job_name, "unknown", SMJOB_MAX_NAME_LEN);
   }

   // make sure job-name is utf8
   if (!is_utf8(job_ticket.job_description.job_name))
   {
      status = IPP_STAT_CLI_DOCUMENT_FORMAT_ERROR;

      goto send_attributes;
   }

  ipp_rcode_t ipp_rcode = _extract_string(cntxt->reqattr,
                              "requesting-user-name", 
                              0,
                              job_ticket.job_description.originating_user_name, 
                              SMJOB_MAX_NAME_LEN);
   if (ipp_rcode == IPP_ATTR_NOT_FOUND)
   {
     strncpy(job_ticket.job_description.originating_user_name,
             "anonymous",
             SMJOB_MAX_NAME_LEN);
   }
   else if (ipp_rcode != IPP_OK)
   {
     status = IPP_STAT_CLI_BAD_REQUEST;

     goto send_attributes;
   }

   // Add the requested changes to the job ticket.
   ipp_add_req_attr_to_job_ticket(cntxt->reqattr, &job_ticket);

   // See if the ipp-attribute-fidelity is present or not. If not then set a default
   // value of false.
   iret = _extract_integer(cntxt->reqattr, 
                           "ipp-attribute-fidelity",
                           0,
                           &job_ticket.ipp_attribute_fidelity);
   if (iret != IPP_OK)
   {
      DBG_MSG("%s - did not find ipp-attribute-fidelity, default to 0\n", __func__);
      job_ticket.ipp_attribute_fidelity = 0;
   }

   if (smjob_accepting_jobs())
   {
      job_id = smjob_create(&job_ticket, 
                            ipp_smjob_status_event, 
                            SMJOB_STATUS_EVENT_STATE, 
                            (void *)ipp_req);

      if(job_id == SMJOB_INVALID_JOB_ID)
      {
          DBG_ERR("req %d: invalid job id!\n", 
                  ipp_req->http_hndl);

          status = IPP_STAT_CLI_NOT_FOUND;

          goto send_attributes;
      }
   }
   else
   {
      DBG_ERR("req %d: not accepting jobs!\n", ipp_req->http_hndl);

      status = IPP_STAT_SRV_NOT_ACCEPTING_JOBS; 

      goto send_attributes;
   }
   
   ipp_req->job_id = job_id;

send_attributes:

  if (job_id != SMJOB_INVALID_JOB_ID)
    DBG_MSG("req %d: created job %d without a document\n", ipp_req->http_hndl, job_id);
  else
    DBG_MSG("req %d: failed to create job\n", ipp_req->http_hndl);
#if IPP_DEBUG_JOBS
   _debug_jobs();
#endif // IPP_DEBUG_JOBS

   return status;
}

void dump_data_and_send_complete(ipp_cntxt_t* cntxt)
{
   ipp_request_t *ipp_req = cntxt->ipp_req;
  
   ASSERT(ipp_req);
   
   while (true)
   {
      ipp_req->tmp_data_len = ipp_reader((void *)ipp_req,
                                                (void *)ipp_req->tmp_data,
                                                 IPP_TMP_DATA_SIZE);
      if(ipp_req->tmp_data_len <= 0)
      {
         break;
      }
   }

   ipp_request_processing_complete(ipp_req);
}

bool _get_job_id_from_context(ipp_cntxt_t* cntxt, uint32_t *job_id)
{
   ipp_rcode_t iret;
   bool        found_job_id = false;

   // Check to see if the job-id value is here.
   iret = _extract_integer(cntxt->reqattr, "job-id", 0, job_id);

   // The job-id was not present. See if the job ID is present in the job-uri
   if (iret != IPP_OK)
   {
      char job_uri_str[64] = {0};

      iret = _extract_string(cntxt->reqattr,
                             "job-uri",
                             0,
                             &job_uri_str[0],
                             sizeof(job_uri_str));

      // The job-uri is present. Now it is time to extract it.
      if (iret == IPP_OK)
      {
         if (strlen(&job_uri_str[0]) > 0)
         {
            char     *job_id_str_loc = NULL;
            char     *job_id_str_end;
            char     *job_uri_str_end;
            char     job_id_as_str[16] = {0};
            uint32_t job_id_as_str_len;
            char     *end_ptr = NULL;

            // Look for the JOB_ID_STR in the job-uri string.
            job_id_str_loc = strstr(&job_uri_str[0], JOB_ID_STR);

            if (job_id_str_loc != NULL)
            {
               // Determine the end of the JOB_ID_STR. It is simply where it
               // starts plus the length of the string.
               job_id_str_end = job_id_str_loc + strlen(JOB_ID_STR);

               // Find out where the job-uri string ends.
               job_uri_str_end = &job_uri_str[0] + strlen(job_uri_str); 
               
               // The job ID as a string is this long.
               job_id_as_str_len = job_uri_str_end - job_id_str_end;

               ASSERT(job_id_as_str_len < sizeof(job_id_as_str));

               strncpy(&job_id_as_str[0], job_id_str_end, job_id_as_str_len);

               // Since it was copied over, now convert it.
               *job_id = strtol(&job_id_as_str[0], &end_ptr, 16);
               
               found_job_id = true;
            }
         }
      }
   }
   // The job-id value was present. Send back that we found it.
   else
   {
      found_job_id = true;
   }

   return (found_job_id);
}

int _send_document(ipp_cntxt_t* cntxt)
{
   smjob_rcode_t      smjob_rcode;
   smjob_doc_ticket_t doc_ticket;
   uint32_t           job_id;
   uint32_t           doc_id;
   smjob_status_t     job_status;
   int                status = IPP_STAT_OK;
   bool               add_the_document = false;
   ipp_request_t *ipp_req = cntxt->ipp_req;
  
   ASSERT(ipp_req);

   DBG_VERBOSE("%s - entered\n", __func__);

   // See if the job-id in some form is present. If not then we cannot do much else.
   if (!_get_job_id_from_context(cntxt, &job_id))
   {
      status = IPP_STAT_CLI_NOT_POSSIBLE;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
   }
   
   ipp_req->job_id = job_id;

   // validate the job
   status = _validate_job(cntxt, true);
   if (status != IPP_STAT_OK)
   {
        dump_data_and_send_complete(cntxt);
        
        goto send_attributes;
   }

   // Get the job status. If we cannot do this or the job has already been cancelled,
   // set the status accordingly and go about ending things.
   smjob_rcode = smjob_get_status(job_id, &job_status);

   if (smjob_rcode != SMJOB_OK || job_status.state == SMJOB_STATE_CANCELED)
   {
      status = IPP_STAT_CLI_NOT_POSSIBLE;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
   }

   // Check that this is a valid document addition. We only support one document
   // added per created job.
   if (!ipp_chk_valid_document_addtion(cntxt->reqattr, 
                                           &job_status,
                                           &add_the_document))
   {
      status = IPP_STAT_SRV_MULTIPLE_DOCUMENT_JOBS_NOT_SUPPORTED;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
   }

   // If there is no data associated with the document addition, this means that
   // the user sent in the data previously and this call to send document has the
   // last document flag set with no document data. This is OK but we do not need
   // to add anything.
   if (!add_the_document)
   {
      status = IPP_STAT_OK;

      ipp_request_processing_complete(ipp_req);

      goto send_attributes;
   }

   // We need to add the user_data to the job ticket. The previous user_data from
   // the create_job is no longer valid and we need this one in its place.
   smjob_rcode = smjob_add_user_data_to_job_ticket(job_id, ipp_req);
   if (smjob_rcode != SMJOB_OK)
   {
      status = IPP_STAT_SRV_INTERNAL_ERROR;

      dump_data_and_send_complete(cntxt);

      goto send_attributes;
   }

   smjob_rcode = smjob_get_default_doc_ticket(job_id, &doc_ticket);
   XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);

   doc_ticket.doc_description.type.print.recv_func = ipp_smjob_reader;
   doc_ticket.doc_description.type.print.user_data = ipp_req;

   // Add the user requested changes to the document ticket.
   ipp_add_req_attr_to_doc_ticket(cntxt->reqattr, &doc_ticket);

   // Make sure job data is flowing before kicking connection manager by adding 
   // the document.
   DBG_MSG("req %d: print job waiting for initial data...\n", ipp_req->http_hndl);
   uint32_t start_time = posix_gettime_ticks();
   UNUSED_VAR(start_time);

   ipp_req->tmp_data_len = ipp_reader((void *)ipp_req,
                                             (void *)ipp_req->tmp_data,
                                             IPP_TMP_DATA_SIZE);
   if (ipp_req->tmp_data_len <= 0)
   {
      DBG_MSG("req %d: print job timeout! (ms=%d)\n",
              ipp_req->http_hndl, (posix_gettime_ticks() - start_time) * 10);

      smjob_cancel(job_id);
         
      status = IPP_STAT_CLI_DOCUMENT_ACCESS_ERROR;

      ipp_req->job_http_error = true;
      ipp_request_processing_complete(ipp_req);

      goto send_attributes;
   }

   DBG_MSG("req %d: print job got initial data (len=%d ms=%d)\n",
           ipp_req->http_hndl, ipp_req->tmp_data_len, (posix_gettime_ticks() - start_time) * 10);

   doc_id = smjob_add_document(job_id, &doc_ticket, true);
   if (doc_id == SMJOB_INVALID_DOC_ID)
   {
      status = IPP_STAT_SRV_INTERNAL_ERROR;

      ipp_request_processing_complete(ipp_req);

      goto send_attributes;
   }

  if (job_id != SMJOB_INVALID_JOB_ID)
    DBG_MSG("req %d: created job %d\n", ipp_req->http_hndl, job_id);
  else
    DBG_MSG("req %d: failed to create job\n", ipp_req->http_hndl);
#if IPP_DEBUG_JOBS
   _debug_jobs();
#endif // IPP_DEBUG_JOBS

send_attributes:

   return status;
}

static ipp_attr_t* _ipp_add_mult_enum_vals_to_attr(const char *attr_name,
                                                   oid_t      oid)
{
   ipp_attr_t      *attr     = NULL;
   ipp_attr_val_t  *val      = NULL;
   uint32_t        oid_index = 0;
   uint32_t        oid_val = 0;

   attr = ipp_create_attr(attr_name, 0);

   while (OK == oid_get_uint32(oid, oid_index, &oid_val))
   {
     // Make sure that we do not get stuck in an infinite loop if an
     // OID_ERROR_INDEX_INVALID is not returned.
     ASSERT(oid_index < MAX_OID_INDEX);
      
     val = ipp_create_attr_val_enum(oid_val);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     ++oid_index;
   }

   return(attr);
}

static ipp_attr_t* _ipp_add_mult_str_vals_to_attr(const char *attr_name,
                                                  uint8_t    syntax,
                                                  oid_t      oid,
                                                  char       *oid_str,
                                                  uint32_t   str_len)
{
   ipp_attr_t     *attr     = NULL;
   ipp_attr_val_t *val      = NULL;
   uint32_t       oid_index = 0;
   error_type_t   error     = OK;

   attr = ipp_create_attr(attr_name, 0);

   while (true)
   {
      error = oid_get_string(oid, oid_index, oid_str, str_len);

      if (error == OK)
      {
         val = ipp_create_attr_val(syntax, oid_str, strlen(oid_str));
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);
      }

      // If there is an error, we will keep looping if the index is not implemented.
      // This does not mean an invalid index, just for this device, the particular
      // attribute is not implemented. For other errors go ahead and break.
      else if (error != OID_ERROR_NOT_IMPLEMENTED)
      {
         break;
      }

      ++oid_index;

      // Make sure that we do not get stuck in an infinite loop if an
      // OID_ERROR_INDEX_INVALID is not returned.
      ASSERT(oid_index < MAX_OID_INDEX);
   }

   return(attr);
}

static ipp_attr_t* _ipp_add_mult_byte_arrays_to_attr(const char *attr_name,
                                                     uint8_t    syntax,
                                                     oid_t      oid,
                                                     uint8_t    *oid_array,
                                                     uint32_t   *array_len)

{
   ipp_attr_t*     attr      = NULL;
   ipp_attr_val_t  *val      = NULL;
   uint32_t        oid_index = 0;

   attr = ipp_create_attr(attr_name, 0);

   while (OK == oid_get_bytearray(oid, oid_index, oid_array, array_len))
   {
      // Make sure that we do not get stuck in an infinite loop if an
      // OID_ERROR_INDEX_INVALID is not returned.
      ASSERT(oid_index < MAX_OID_INDEX);
      
      val = ipp_create_attr_val(syntax, oid_array, *array_len);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);

      // This needs to be reset for the next pass as the call to oid_getbytearray()
      // used the variable to return the number of bytes added.
      *array_len = OID_BYTE_ARRAY_SIZE;

      ++oid_index;
   }

   return(attr);
}

static ipp_attr_t* _get_printer_uuid(char *oid_str, uint32_t str_len)
{
   ipp_attr_t     *attr            = NULL;
   ipp_attr_val_t *val             = NULL;
   error_type_t   error            = OK;
   char           *uuid_pre_str    = "urn:uuid:";
   uint32_t       uuid_pre_str_len = strlen(uuid_pre_str);


   attr = ipp_create_attr("printer-uuid", 0);

   sprintf(&oid_str[0], "%s", uuid_pre_str);

   // The sprintf call above will put in a null, so start the write of the
   // next string at that point.
   error = oid_get_string(OID_PLATFORM_UUID, 
                          0, 
                          &oid_str[uuid_pre_str_len], 
                          str_len - uuid_pre_str_len);

   if (error == OK)
   {
      val = ipp_create_attr_val(IPP_TAG_URI, oid_str, strlen(oid_str));
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
   }
   
   return(attr);
}

ipp_attr_t* _get_job_uuid(uint32_t job_id)
{
   ipp_attr_t     *attr                      = NULL;
   ipp_attr_val_t *val                       = NULL;
   error_type_t   error                      = OK;
   char           uuid_str[JOB_UUID_STR_LEN] = {0};
   uint32_t       uuid_str_len               = sizeof(uuid_str);
   char           *uuid_pre_str              = "urn:uuid:";
   uint32_t       uuid_pre_str_len           = strlen(uuid_pre_str);

   attr = ipp_create_attr("job-uuid", 0);

   sprintf(&uuid_str[0], "%s", uuid_pre_str);

   // Get the platform UUID as our starting point.
   // The sprintf call above will put in a null, so start the write of the
   // next string at that point.
   error = oid_get_string(OID_PLATFORM_UUID, 
                          0, 
                          &uuid_str[uuid_pre_str_len], 
                          uuid_str_len - uuid_pre_str_len);

   if (error == OK)
   {
      char     job_id_str[8]  = {0};
      uint32_t job_id_str_len = 0;

      // Only take the first four bytes. That should be sufficient for uniqueness.
      sprintf(&job_id_str[0], "%04x", (uint16_t)job_id);

      job_id_str_len = strlen(job_id_str);

      uuid_str_len = strlen(uuid_str);

      strncpy(&uuid_str[uuid_str_len - job_id_str_len], &job_id_str[0], job_id_str_len);

      val = ipp_create_attr_val(IPP_TAG_URI, uuid_str, strlen(uuid_str));
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
   }

   return(attr);

}

static bool _media_type_is_supported(char *media_str)
{
#if 1
   // Support everything
   return (true);
#else
   if (strstr(media_str, "letter") || 
       strstr(media_str, "4x6")    ||
       strstr(media_str, "legal")  ||
       strstr(media_str, "a4"))
   {
      return(true);
   }

   return(false);
#endif
}

static void _get_media_supported(ipp_attr_grp_t *grp,
                                 char           *oid_str,
                                 uint32_t       str_len)
{
   ipp_attr_t      *attr     = NULL;
   ipp_attr_val_t  *val      = NULL;
   uint32_t        oid_index = 0;

   attr = ipp_create_attr("media-supported", 0);

   while (OK == oid_get_string(OID_SM_PRINT_MEDIA_SUPPORTED, oid_index, oid_str, str_len))
   {
      if (_media_type_is_supported(oid_str))
      {
         val = ipp_create_attr_val(IPP_TAG_KEYWORD, oid_str, strlen(oid_str));
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);

      }
      
      ++oid_index;
   }
   
   val = ipp_create_attr_val(IPP_TAG_KEYWORD, 
                          "custom_min_3.875x5.75in", 
                          strlen("custom_min_3.875x5.75in"));
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_KEYWORD, 
                          "custom_max_8.5x13in", 
                          strlen("custom_max_8.5x13in"));
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   ipp_add_attr(grp, attr);
   ipp_free_attr(attr, 1);
}

static void _get_media_col(ipp_attr_grp_t        *grp,
                           char                  *collection_name,
                           char                  *default_media_name,
                           bool                   include_media_name)
{
   ipp_attr_t     *attr = NULL;
   ipp_attr_val_t *val = NULL;
   uint32_t       i = 0;
   uint32_t       int_val = 0;
   char           media_name[128]={0};
   char           media_str[64]={0};

   attr = ipp_create_attr(collection_name, 0);

   while (OK == oid_get_string(OID_SM_PRINT_MEDIA_SUPPORTED,
                               i,
                               media_name,
                               sizeof(media_name)))
   {
#if 0 //Fix disabled media-size issue
      bool add_media_to_collection = false;
      
      if (_media_type_is_supported(media_name))
      {
        if (default_media_name != NULL)
        {
            if (strcmp(default_media_name, media_name) == 0)
            {
                add_media_to_collection = true;
            }
        }
        else
        {
            add_media_to_collection = true;
        }
      }
#endif 
      val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-size", 10);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0 );
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "x-dimension", 11);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
      
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "y-dimension", 11);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_PRINT_MEDIA_Y_DIM_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
      
      if (include_media_name)
      {
          val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-size-name", 15);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
          val = ipp_create_attr_val(IPP_TAG_NAME_WITHOUT_LANGUAGE, 
                                    media_name, 
                                    strlen(media_name));
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
 
      // NOTE: margins can all be the same, but ask here to give oem layer a vote.
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-top-margin", 16);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_MEDIA_TOP_MARGIN_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-bottom-margin", 19);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_MEDIA_BOTTOM_MARGIN_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-left-margin", 17);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_MEDIA_LEFT_MARGIN_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
      
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-right-margin", 18);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_MEDIA_RIGHT_MARGIN_SUPPORTED, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-source", 12);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_string_oid(OID_SM_PRINT_MEDIA_TABLE_SOURCE, i, &media_str[0], sizeof(media_str)))
      {
          val = ipp_create_attr_val(IPP_TAG_KEYWORD, media_str, strlen(media_str));
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "media-type", 10);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_string_oid(OID_SM_PRINT_MEDIA_TABLE_TYPE, i, &media_str[0], sizeof(media_str)))
      {
          val = ipp_create_attr_val(IPP_TAG_NAME_WITHOUT_LANGUAGE, media_str, strlen(media_str));
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "duplex-supported", 16);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);
      
      if (get_uint32_oid(OID_SM_PRINT_MEDIA_TABLE_DUPLEX, i, &int_val))
      {
          val = ipp_create_attr_val_int(int_val);
          ipp_add_attr_val(attr, val);
          ipp_free_attr_val(val, 1);
      }
 
      val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);

      // Move to the next media index
      i++;
   }

   ipp_add_attr(grp, attr);
   ipp_free_attr(attr, 1);
}

static void _get_media_size_supported(ipp_attr_grp_t *grp)
{
   ipp_attr_t     *attr;
   ipp_attr_val_t *val;
   uint32_t       i = 0;
   uint32_t       int_val;
   char           media_name[128];

   attr = ipp_create_attr("media-size-supported", 0);

   while (OK == oid_get_string(OID_SM_PRINT_MEDIA_SUPPORTED, i, media_name, sizeof(media_name)))
   {
      if (_media_type_is_supported(media_name))
      {
         val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0);
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);

         val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "x-dimension", 11);
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);
         
         if (get_uint32_oid(OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED, i, &int_val))
         {
             val = ipp_create_attr_val_int(int_val);
             ipp_add_attr_val(attr, val);
             ipp_free_attr_val(val, 1);
         }

         val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "y-dimension", 11);
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);
         
         if (get_uint32_oid(OID_SM_PRINT_MEDIA_Y_DIM_SUPPORTED, i, &int_val))
         {
             val = ipp_create_attr_val_int(int_val);
             ipp_add_attr_val(attr, val);
             ipp_free_attr_val(val, 1);
         }

         val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
         ipp_add_attr_val(attr, val);
         ipp_free_attr_val(val, 1);
      }

      // Move to the next media index
      i++;
   }


   val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "x-dimension", 11);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "lower", 5);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val_int(9842);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "upper", 5);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val_int(21590);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "y-dimension", 11);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_BEG_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "lower", 5);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val_int(14605);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_MEMBER_ATTR_NAME, "upper", 5);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val_int(33020);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);


   val = ipp_create_attr_val(IPP_TAG_END_COLLECTION, "", 0);
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);


   ipp_add_attr(grp, attr);
   ipp_free_attr(attr, 1);
}

error_type_t ipp_get_network_ip_address_from_connection(
                             uint32_t  index,
                             char     *data_ptr,
                             uint32_t  max_string_len_in_bytes )
{
    error_type_t e_res = OK;
// TODO removed during linux port 3/2014
//    struct sockaddr_storage *from_addr;
//    int32_t size;
//
//    // WARNING: this code used to be in OID_NETWORK_IP_ADDRESS_FROM_CONNECTION,
//    //   but OID code calling back to http is a recipe for deadlock!
//
//#ifdef HAVE_IPV6
//    ASSERT (max_string_len_in_bytes >= INET6_ADDRSTRLEN);
//#else
//    ASSERT (max_string_len_in_bytes >= INET_ADDRSTRLEN);
//#endif
//
//    size = sizeof( struct sockaddr_storage );
//    from_addr = ( struct sockaddr_storage* )MEM_MALLOC( size );
//    if ( from_addr == NULL )
//    {
//        return FAIL;
//    }
//    memset( from_addr, 0x00, size );
//
//    e_res = http_resource_get_sock_addr( ( http_request_t  )index, from_addr );
//    if ( e_res != OK )
//    {
        uint32_t ipv4_addr;
        unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);
        
        e_res = net_iface_get_var_ipv4_addr( if_index, &ipv4_addr );
        if ( NET_IFACE_OK == e_res )
        {
            inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
        }
// TODO removed during linux port 3/2014
//    }
//    else
//    {
//        #ifdef HAVE_IPV6
//        if ( ( from_addr->addr.ipv6.sin6_family == AF_INET6 ) &&
//             ( from_addr->addr.ipv6.sin6_addr.ip6Addr.ip6U32[ 0 ] != 0 ) )
//        {   
//            inet_ntop(AF_INET6, (void *)(&from_addr->addr.ipv6.sin6_addr), data_ptr, INET6_ADDRSTRLEN);
//        }
//        else
//        {
//            inet_ntop(AF_INET, (void *)(&from_addr->addr.ipv6.sin6_addr.ip6Addr.ip6U32[3]), data_ptr, INET6_ADDRSTRLEN);
//        }
//        #else
//        inet_ntop(AF_INET, (void *)(&from_addr->addr.ipv4.sin_addr), data_ptr, INET_ADDRSTRLEN);
//        #endif // HAVE_IPV6
//    }
//    MEM_FREE_AND_NULL( from_addr );

    return e_res;
}

void _get_correct_host_address(char          *host_adrs_str,
                                      uint32_t      host_adrs_str_len,
                                      ipp_request_t *ipp_req)
{
   char *host_str     = "Host: ";
   char *port_str     = ":631";
   char *host_str_loc = NULL;
   char *port_str_loc = NULL;
   bool str_found     = false;

   ASSERT(host_adrs_str);
   ASSERT(host_adrs_str_len);

   host_adrs_str[0] = '\0'; // return empty str on error
   
   host_str_loc = strstr(ipp_req->http_hdr, host_str);
   
   if (host_str_loc)
   {
      port_str_loc = strstr(host_str_loc, port_str);

      if (port_str_loc)
      {
         char     *start_adrs  = host_str_loc + strlen(host_str);
         uint32_t addr_len     = port_str_loc - start_adrs;
         if(host_adrs_str_len > addr_len)
         {
             strncpy(host_adrs_str, start_adrs, host_adrs_str_len);
             host_adrs_str[addr_len] = '\0';
         }

         str_found = true;
      }
   }

   // Just send back the socket connection address, if the HTTP header does not include host addr
   if (!str_found)
   {
        if (ipp_req->ipp_ctxt && ipp_req->ipp_ctxt->server_address)
        {
            strncpy(&host_adrs_str[0], ipp_req->ipp_ctxt->server_address, host_adrs_str_len);
        }
        else
        {
            #ifdef HAVE_IPV6
               char network_address[INET6_ADDRSTRLEN];
            #else
               char network_address[INET_ADDRSTRLEN];
            #endif

            if (ipp_get_network_ip_address_from_connection(
                         ipp_req->http_hndl,
                         &network_address[0],
                         sizeof(network_address)))
            {
                strncpy(&host_adrs_str[0], &network_address[0], host_adrs_str_len);
            }
        }
   }
}

static void _get_printer_icons(ipp_attr_grp_t *grp,
                               char           *icon_str,
                               uint32_t       icon_str_len,
                               ipp_request_t  *ipp_req)
{
   ipp_attr_t     *attr;
   ipp_attr_val_t *val;
   uint32_t       i                    = 0;
   char           *protocol_str        = "http://";
   uint32_t       protocol_str_len     = strlen(protocol_str);
   uint32_t       prot_and_adrs_offset = 0;
   char           icon_oid_val[128]    = {0};

   memset(icon_str, 0, icon_str_len);

   strncpy(&icon_str[0], protocol_str, protocol_str_len);

   _get_correct_host_address(&icon_str[protocol_str_len],
                             icon_str_len - protocol_str_len,
                             ipp_req);

   prot_and_adrs_offset = strlen(icon_str);

   attr = ipp_create_attr("printer-icons", 0);


   while (get_string_oid(OID_SM_PRINTER_ICON, 
                       i, 
                       &icon_oid_val[0], 
                       sizeof(icon_oid_val)))
   {
      strcpy(&icon_str[prot_and_adrs_offset], icon_oid_val);


      val = ipp_create_attr_val(IPP_TAG_URI, icon_str, strlen(icon_str));
      ipp_add_attr_val(attr, val);
      ipp_free_attr_val(val, 1);

      memset(&icon_str[prot_and_adrs_offset], 0, icon_str_len - prot_and_adrs_offset);

      i++;
   }

   // If we don't have any printer icons, leave without responding
   if (i > 0)
   {
       ipp_add_attr(grp, attr);
   }

   ipp_free_attr(attr, 1);
}

static void _get_printer_uri_supported(ipp_attr_grp_t *grp,
                                       char           *uri_str,
                                       uint32_t       max_uri_str_len,
                                       ipp_request_t  *ipp_req)
{
   ipp_attr_t     *attr;
   ipp_attr_val_t *val;
   char           *uri_base_str               = "ipp://%s/ipp/print";
   uint32_t       uri_str_len                 = strlen(uri_base_str);
   char           hostname[MAX_HOSTNAME_SIZE] = {0};

   memset(uri_str, 0, max_uri_str_len);

   attr = ipp_create_attr("printer-uri-supported", 0);

   _get_correct_host_address(&hostname[0],
                             sizeof(hostname),
                             ipp_req);


   uri_str_len += strlen(hostname);

   ASSERT(uri_str_len < max_uri_str_len);

   sprintf(uri_str, uri_base_str, hostname);

   val = ipp_create_attr_val(IPP_TAG_URI, uri_str, strlen(uri_str));
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   ipp_add_attr(grp, attr);
   ipp_free_attr(attr, 1);
}

static void _get_printer_supply_info_uri(ipp_attr_grp_t *grp,
                                         char           *psi_uri_str,
                                         uint32_t       max_psi_uri_str_len,
                                         ipp_request_t  *ipp_req)
{
   ipp_attr_t     *attr;
   ipp_attr_val_t *val;
   char           *psi_uri_base_str           = "http://%s/%s";
   uint32_t       psi_uri_str_len             = strlen(psi_uri_base_str);
   char           hostname[MAX_HOSTNAME_SIZE] = {0};
   char           *psi_file                   = "printer_status.html";

   memset(psi_uri_str, 0, max_psi_uri_str_len);

   attr = ipp_create_attr("printer-supply-info-uri", 0);

   _get_correct_host_address(&hostname[0],
                             sizeof(hostname),
                             ipp_req);

   psi_uri_str_len += strlen(hostname);
   psi_uri_str_len += strlen(psi_file);

   ASSERT(psi_uri_str_len < max_psi_uri_str_len);

   sprintf(psi_uri_str, psi_uri_base_str, hostname, psi_file);

   val = ipp_create_attr_val(IPP_TAG_URI, psi_uri_str, strlen(psi_uri_str));
   ipp_add_attr_val(attr, val);
   ipp_free_attr_val(val, 1);

   ipp_add_attr(grp, attr);
   ipp_free_attr(attr, 1);
}

static void _get_printer_geo_location(ipp_attr_grp_t *grp,
                                      char           *geo_loc_str,
                                      uint32_t       geo_loc_str_len)
{
   ipp_attr_t     *attr;
   ipp_attr_val_t *val;

   attr = ipp_create_attr("printer-geo-location", 0);

   memset(geo_loc_str, 0, geo_loc_str_len);

   if (get_string_oid(OID_SM_PRINTER_GEO_LOCATION,
                  0,
                  geo_loc_str,
                  geo_loc_str_len))
   {
       ipp_u8bit_t value_tag = IPP_TAG_URI;
       if(strlen(geo_loc_str)==0)
       {
           // must return out-of-band value 'unknown' when the geo location is unknown or 
           // not specified http://www.pwg.org/ipp/ PWG 5100.13-2012 (JPS3) section 5.6.28 
           // 
           value_tag = IPP_TAG_UNKNOWN;
       }
       val = ipp_create_attr_val(value_tag, geo_loc_str, strlen(geo_loc_str));
       ipp_add_attr_val(attr, val);
       ipp_free_attr_val(val, 1);

       ipp_add_attr(grp, attr);
       ipp_free_attr(attr, 1);
   }
}

/*----------------------------------------------------------------------------*/
static int _cups_get_printers(ipp_cntxt_t* cntxt)
{
  ipp_request_t *ipp_req = cntxt->ipp_req;
  
  ASSERT(ipp_req);

  bool valid_attr_req = ipp_chk_valid_cups_get_printers_req(cntxt->reqattr);

  if (!valid_attr_req || ipp_req->ipp_request_id == 0)
  {
     return(IPP_STAT_CLI_BAD_REQUEST);
  }

  if (ipp_req->ipp_version == 0)
  {
     return(IPP_STAT_SRV_VERSION_NOT_SUPPORTED);
  }
  
  return IPP_STAT_OK;
}

/*----------------------------------------------------------------------------*/
static int _get_printer_attr(ipp_cntxt_t* cntxt)
{
  ipp_request_t *ipp_req = cntxt->ipp_req;
  
  ASSERT(ipp_req);

  bool valid_attr_req = ipp_chk_valid_get_print_attr_req(cntxt->reqattr);

  if (!valid_attr_req || ipp_req->ipp_request_id == 0)
  {
     return(IPP_STAT_CLI_BAD_REQUEST);
  }

  if (ipp_req->ipp_version == 0)
  {
     return(IPP_STAT_SRV_VERSION_NOT_SUPPORTED);
  }
  
  return IPP_STAT_OK;
}

/*----------------------------------------------------------------------------*/
static int _get_jobs(ipp_cntxt_t* cntxt)
{
   ipp_rcode_t ipp_rcode;
   uint32_t attr_int;
   bool attr_bool;
   char attr_string[SMJOB_MAX_NAME_LEN];

   
   /*** Attribute "which-jobs" ***/
   /* 
     The client OPTIONALLY supplies this attribute.  The Printer
     object MUST support this attribute.  It indicates which Job
     objects MUST be returned by the Printer object. The values for
     this attribute are:

    'completed': This includes any Job object whose state is
        'completed', 'canceled', or 'aborted'.
    'not-completed': This includes any Job object whose state is
        'pending', 'processing', 'processing-stopped', or 'pending-
        held'.

     A Printer object MUST support both values.  However, if the
     implementation does not keep jobs in the 'completed',
     'canceled', and 'aborted' states, then it returns no jobs when
     the 'completed' value is supplied.

     If a client supplies some other value, the Printer object MUST
     copy the attribute and the unsupported value to the Unsupported
     Attributes response group, reject the request, and return the
     'client-error-attributes-or-values-not-supported' status code.

     If the client does not supply this attribute, the Printer
     object MUST respond as if the client had supplied the attribute
     with a value of 'not-completed'.   
   */
   memset(attr_string, 0, SMJOB_MAX_NAME_LEN);
   ipp_rcode = _extract_string(cntxt->reqattr,
                      "which-jobs", 
                      0,
                      attr_string, 
                      SMJOB_MAX_NAME_LEN-1);
   if (ipp_rcode == IPP_OK)
   {
       /* If provided but does not match one of the below then an error will be reported */
       if (strcmp( attr_string, "completed" ) != 0 && strcmp( attr_string, "not-completed" ) != 0)
       {
        return IPP_STAT_CLI_ATTRIBUTES_OR_VALUES_NOT_SUPPORTED;
       }
   }
   else if (ipp_rcode == IPP_ATTR_NOT_FOUND)
   {
    // ok
   }
   else if (ipp_rcode == IPP_INVALID_LENGTH)
   {
    return IPP_STAT_CLI_REQUEST_VALUE_TOO_LONG;
   }
   else
   {
    return IPP_STAT_CLI_BAD_REQUEST;
   }  
   
   
   /*** Attribute "my-jobs" ***/
   /* 
    IF NEITHER a single 'true' NOR a single 'false' 'boolean' value,
    REJECT/RETURN 'client-error-bad-request'.

    IF the value length is NOT equal to 1 octet, REJECT/RETURN
    'client-error-request-value-too-long'

    IF NOT supplied by the client, the IPP object assumes the 'false'
    value.
   */
   ipp_rcode = _extract_boolean(cntxt->reqattr, "my-jobs", 0, &attr_bool);
   if (ipp_rcode == IPP_ATTR_NOT_FOUND)
   {
    // ok
   }
   else if (ipp_rcode == IPP_INVALID_LENGTH)
   {
    return IPP_STAT_CLI_REQUEST_VALUE_TOO_LONG;
   }
   else if (ipp_rcode != IPP_OK)
   {
    return IPP_STAT_CLI_BAD_REQUEST;
   }

   /*** Attribute "requesting-user-name" ***/
   /* 
    The "requesting-user-name" (name(MAX)) attribute SHOULD be
    supplied by the client as described in section 8.3.
    
    Section 8.3:
    When the authentication mechanism is 'none', all authenticated users are
   "anonymous".
    
    Section 4.4.2:
    If the "requesting-user-name" attribute is absent in a request, the
    Printer object assumes that the authenticated user is "anonymous".
   */
    memset(attr_string, 0, SMJOB_MAX_NAME_LEN);
    ipp_rcode = _extract_string(cntxt->reqattr,
                      "requesting-user-name", 
                      0,
                      attr_string, 
                      SMJOB_MAX_NAME_LEN-1);
    if (ipp_rcode == IPP_ATTR_NOT_FOUND)
    {
        // ok
    }
    else if (ipp_rcode == IPP_INVALID_LENGTH)
    {
        return IPP_STAT_CLI_REQUEST_VALUE_TOO_LONG;
    }
    else if (ipp_rcode != IPP_OK)
    {
        return IPP_STAT_CLI_BAD_REQUEST;
    }
   
   /*** Attribute "limit" ***/
   /* 
    The client OPTIONALLY supplies this attribute.  The Printer
    object MUST support this attribute. It is an integer value that
    determines the maximum number of jobs that a client will
    receive from the Printer even if "which-jobs" or "my-jobs"
    constrain which jobs are returned.  The limit is a "stateless
    limit" in that if the value supplied by the client is 'N', then
    only the first 'N' jobs are returned in the Get-Jobs Response.
    There is no mechanism to allow for the next 'M' jobs after the
    first 'N' jobs.  If the client does not supply this attribute,
    the Printer object responds with all applicable jobs.
   */
   ipp_rcode = _extract_integer(cntxt->reqattr, "limit", 0, &attr_int);
   if (ipp_rcode == IPP_ATTR_NOT_FOUND)
   {
    // ok
   }
   else if (ipp_rcode != IPP_OK)
   {
    return IPP_STAT_CLI_BAD_REQUEST;
   }

  return IPP_STAT_OK;
}

/*----------------------------------------------------------------------------*/
static int _get_job(ipp_cntxt_t* cntxt)
{
  uint32_t job_id = SMJOB_INVALID_JOB_ID;
  ipp_request_t *ipp_req = cntxt->ipp_req;
  
  ASSERT(ipp_req);

  if (!_get_job_id_from_context(cntxt, &job_id))
  {
    return IPP_STAT_CLI_BAD_REQUEST;
  }
  
  ipp_req->job_id = job_id;
  
  smjob_rcode_t smjob_rcode;
  smjob_status_t job_status;
  smjob_rcode = smjob_get_status(job_id, &job_status);
  if(smjob_rcode != SMJOB_OK)
  {
      return IPP_STAT_CLI_NOT_FOUND;
  }

  XASSERT(job_id == job_status.job_id, job_status.job_id);

  /* If provided then this is the requested attributes per job. If not
     provided then we should assume "job-uri" and "job-id" */
/*   ipp_attr_t* reqattr = ipp_find_attr(request, "requested-attributes"); */

  return IPP_STAT_OK;
}

/*----------------------------------------------------------------------------*/
static int _cancel_job(ipp_cntxt_t* cntxt)
{
   uint32_t job_id = SMJOB_INVALID_JOB_ID;
   ipp_request_t *ipp_req = cntxt->ipp_req;
  
   ASSERT(ipp_req);
   
   if (!_get_job_id_from_context(cntxt, &job_id))
   {
        return IPP_STAT_CLI_BAD_REQUEST;
   }
    
    ipp_req->job_id = job_id;    
                                     
   smjob_rcode_t smjob_rcode;
   smjob_status_t job_status;
   smjob_rcode = smjob_get_status(job_id, &job_status);
   if (smjob_rcode != SMJOB_OK)
   {
      return IPP_STAT_CLI_NOT_FOUND;
   }
   XASSERT(job_id == job_status.job_id, job_status.job_id);

   /* client error not possible */
   if (job_status.state == SMJOB_STATE_COMPLETED) return IPP_STAT_CLI_NOT_POSSIBLE;
   if (job_status.state == SMJOB_STATE_ABORTED)   return IPP_STAT_CLI_NOT_POSSIBLE;
   if (job_status.state == SMJOB_STATE_CANCELED)  return IPP_STAT_CLI_NOT_POSSIBLE;

   smjob_rcode = smjob_cancel(job_id);
   if (smjob_rcode != SMJOB_OK)
   {
      return IPP_STAT_SRV_INTERNAL_ERROR;
   }

   return IPP_STAT_OK;
}


/*----------------------------------------------------------------------------*/
static int _identify_printer(ipp_cntxt_t* cntxt)
{
    error_type_t retCode = oid_set_uint32(OID_WIFI_DEVICE_IDENTIFY, 0, 1);
    if (retCode != OK)
    {
        return IPP_STAT_SRV_INTERNAL_ERROR;
    }

   return IPP_STAT_OK;
}

static ipp_requested_attrs_t *_get_the_req_attributes(ipp_request_t *ipp_req)
{
   uint32_t              i;
   uint32_t              j;
   ipp_attr_t            *reqattr             = NULL;
   uint32_t              data_len_needed      = 0;
   uint32_t              data_offset          = 0;
   ipp_requested_attrs_t *ipp_req_attrs = NULL;

   ipp_cntxt_t *cntxt = ipp_req->ipp_ctxt;
   ASSERT(cntxt);

   reqattr = ipp_find_attr(cntxt->reqattr, "requested-attributes");

   if (reqattr)
   {
      ipp_req_attrs = IPP_MALLOC(sizeof(ipp_requested_attrs_t));

      ASSERT(ipp_req_attrs);

      data_len_needed = reqattr->num_val;
      for (i = 0; i < reqattr->num_val; ++i)
      {
         data_len_needed += reqattr->val[i].len;
      }

      ipp_req_attrs->num_req_attr = reqattr->num_val;

      ipp_req_attrs->req_attr_str = IPP_MALLOC(data_len_needed);
      ASSERT(ipp_req_attrs->req_attr_str);

      ipp_req->req_attrs.grp_mask = 0;
      for (i = 0; i < reqattr->num_val; ++i)
      {
          // rdj feb-2014
          // check if attribute has been ported over to the hash table yet
          uint32_t len = reqattr->val[i].len;
          char *name = reqattr->val[i].data;
          ipp_attribute_t *attr = (void *)map_lookup(ipp_req->ipp_instance->attr_map, name, len);
          if(attr)
          {
              if(ipp_req->req_attrs.map == MAP_INVALID_HANDLE)
              {
                  ipp_req->req_attrs.map = map_create(0);
              }

              if(attr->flags & IPP_CAT_GRP_NAME)
              {
                  // this is a group name (IPP_CAT_GRP_NAME)
                  ipp_req->req_attrs.grp_mask |= attr->flags & IPP_GRP_MASK;
              }
              else
              {
                  // assume this is an attribute name (IPP_CAT_ATTR)
                  // map will automatically filter out any duplicate attribute requests
                  XASSERT(attr->flags & IPP_CAT_ATTR, (int)(attr->flags & IPP_CAT_MASK)); // confirm IPP_CAT_ATTR
                  map_insert(ipp_req->req_attrs.map, name, len, (void *)attr);
              }
          }

         for (j = 0; j < reqattr->val[i].len; ++j)
         {
            ipp_req_attrs->req_attr_str[data_offset] = ((char *)(reqattr->val[i].data))[j];

            ++data_offset;
         }
         ipp_req_attrs->req_attr_str[data_offset] = 0;

         ++data_offset;
      }

      ASSERT(data_offset <= data_len_needed);

   }

   return(ipp_req_attrs);

}

static void _free_the_req_attributes(ipp_requested_attrs_t *req_attributes)
{
    if (req_attributes)
    {
        if (req_attributes->req_attr_str)
        {
            IPP_FREE(req_attributes->req_attr_str);
        }
        
       IPP_FREE(req_attributes);
    }
}

static void _handle_unsupported_req(ipp_cntxt_t* cntxt)
{
     /* Group 1 - Operation Attributes */
  ipp_attr_grp_t* grp = _default_operation_attributes();
  ipp_add_attr_group(cntxt->respattr, grp);
  ipp_free_attr_group(grp, 1);

}


#ifdef ENABLE_SINDOH_MOBILE_APP

static char *_makestr(char *data, int len)
{
   char *s = MEM_MALLOC(len + 1);

   if (s)
   {
      memcpy(s, data, len);
      s[len] = 0;
   }
   return s;
}


char *getStringAttr(ipp_attr_val_t *val)
{
   if (!val)
   {
      return NULL;
   }
   char          *s = NULL;
   if ((val->syntax == IPP_TAG_CHARSET) ||
       (val->syntax == IPP_TAG_NATURAL_LANGUAGE) ||
       (val->syntax == IPP_TAG_MIME_MEDIA_TYPE) ||
       (val->syntax == IPP_TAG_KEYWORD) ||
       (val->syntax == IPP_TAG_URI) ||
       (val->syntax == IPP_TAG_URI_SCHEME) ||
       (val->syntax == IPP_TAG_TEXT_WITHOUT_LANGUAGE) ||
       (val->syntax == IPP_TAG_NAME_WITHOUT_LANGUAGE))
   {
      s = _makestr(val->data, val->len);
      //printf("      VALUE: Syntax=%s, Data=%s", syntax, s);
   }

   return s;
}


int getIntAttr(ipp_attr_val_t *val)
{
   if (!val)
   {
      return 0;
   }
   unsigned char *d = (unsigned char *)val->data;
   const char *syntax = ipp_const_tag_str(val->syntax);
   if (val->syntax == IPP_TAG_BOOLEAN)
   {
      if (val->len != 1)
      {
         printf("      VALUE: Syntax=%s, (invalid len = %d)", syntax, val->len);
         return 0;
      }
      else if (d[0] > 1)
      {
         printf("      VALUE: Syntax=%s, (invalid value = %d)", syntax, d[0]);
         return 0;
      }
      else
      {
         printf("      VALUE: Syntax=%s, %s", syntax, d[0] ? "TRUE" : "FALSE");

         return d[0] ? 1 : 0;
      }
   }
   else if ((val->syntax == IPP_TAG_INTEGER) ||
            (val->syntax == IPP_TAG_ENUM))
   {
      if (val->len != 4)
      {
         printf("      VALUE: Syntax=%s, (invalid len = %d)", syntax, val->len);
         return 0;
      }
      else
      {
         union
         {
            signed int   sval;
            unsigned int uval;
         }
         v;
         v.uval   = d[0];
         v.uval <<= 8;
         v.uval  += d[1];
         v.uval <<= 8;
         v.uval  += d[2];
         v.uval <<= 8;
         v.uval  += d[3];
         printf("      VALUE: Syntax=%s, %d", syntax, v.sval);
         return v.sval;
      }
   }
   else
   {
      printf("      VALUE: Syntax=%s, Data_len=%d", syntax, val->len);
      return 0;
   }
}


void ipp_getMobileApp_Attr(ipp_attr_groups_t *grps, stShmGoPrint **pPrintAddInfo)
{
   if (grps)
   {
      ipp_u32bit_t groupIndex = 0;
      for (groupIndex = 0; groupIndex < grps->num_grp; groupIndex++)
      {
         ipp_attr_grp_t *grp = grps->grp + groupIndex;
         if (grp)
         {
            ipp_u32bit_t a = 0;
            printf("  GROUP: %s", ipp_const_tag_str(grp->tag));
            for (a = 0; a < grp->num_attr; a++)

            {
               ipp_attr_t *attr = grp->attr + a;

               if (attr)
               {
                  ipp_u32bit_t v = 0;
                  printf("    ATTRIBUTE: %s", attr->name);
				  //Following block requires refinement and addition of parsing more attributes 
				  //This will be also be modified for every new attribute added by Mobile APP.
                  for (v = 0; v < attr->num_val; v++)
                  {
                     if (strstr(attr->name, "copies") != NULL)
                     {
                        (*pPrintAddInfo)->repeat = getIntAttr(attr->val + v);
                     }
                     else if (strstr(attr->name, "printer-quality") != NULL)
                     {
                        (*pPrintAddInfo)->darkness = getIntAttr(attr->val + v);
                     }
                     else if (strstr(attr->name, "sides") != NULL)
                     {
                        char *sides = getStringAttr(attr->val + v);

                        if (sides == NULL)
                        {
                           (*pPrintAddInfo)->duplex = 0;
                        }
                        else if (strstr(sides, "one-sided") != NULL)
                        {
                           (*pPrintAddInfo)->duplex = 0;
                        }
                        else if (strstr(sides, "two-sided-short-edge") != NULL)
                        {
                           (*pPrintAddInfo)->duplex = 1;
                        }
                        else if (strstr(sides, "two-sided-long-edge") != NULL)
                        {
                           (*pPrintAddInfo)->duplex = 2;
                        }

                        if (sides)
                        {
                           free(sides);
                           sides = NULL;
                        }
                        //ipp_dump_attr_val(attr->val + v);
                     }
                     else if (strstr(attr->name, "copies") != NULL)
                     {
                        (*pPrintAddInfo)->fileCount = getIntAttr(attr->val + v);
                     }
					 else if (strstr(attr->name, "job-media-sheets") != NULL)
                     {
                        (*pPrintAddInfo)->inTray = getIntAttr(attr->val + v);
						if((*pPrintAddInfo)->inTray == 4)
							(*pPrintAddInfo)->inTray=0;
                        else if((*pPrintAddInfo)->inTray == 0)
							(*pPrintAddInfo)->inTray=8;
                     }
					 
                     else if (strstr(attr->name, "multiple-document-handling") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *collate = getStringAttr(attr->val + v);

                        if (collate == NULL)
                        {
                           (*pPrintAddInfo)->sort = 0;
                        }
                        else if (strcmp(collate, "single-document") == 0)
                        {
                           (*pPrintAddInfo)->sort = 1;
                        }
                        else
                        {
                           (*pPrintAddInfo)->sort = 0;
                        }

                        if (collate)
                        {
                           free(collate);
                           collate = NULL;
                        }
                     }
                     else if (strstr(attr->name, "page-size-height") != NULL)
                     {
                        (*pPrintAddInfo)->image_height= getIntAttr(attr->val + v);
                        
                     }
                     else if (strstr(attr->name, "page-size-width") != NULL)
                     {
                         (*pPrintAddInfo)->image_width = getIntAttr(attr->val + v);
                        
                     }
                     else if (strstr(attr->name, "page-size-rotate") != NULL)
                     {
                        printf(" page-size-rotate = %d",getIntAttr(attr->val + v));                        
                     }
                  }
               }
            }
         }
      }
   }
}


bool checkSpaceOK(const char *filePath)
{
   struct statvfs fsentry;
   struct stat fsEntryDetail;
   unsigned long value=0;
   if(filePath == NULL)
   return false;
   memset(&fsEntryDetail, 0, sizeof(struct stat));
   memset(&fsentry, 0, sizeof(struct statvfs));
   if (stat(filePath, &fsEntryDetail) == 0)
   {
      value = fsEntryDetail.st_size;
   }
   else
   {
      return(false);
   }			
   /*printf("\ntmp Block size %lu; Available blocks: %lu\n",
	 fsentry.f_bsize,
	 fsentry.f_bavail);*/
   if (statvfs("/tmp", &fsentry))
   return false;  
   
   return (fsentry.f_bavail > ((value/fsentry.f_bsize)+1));   
}


bool checkMemOK(unsigned long value)
{
   struct statvfs fsentry;   
   if (statvfs("/tmp", &fsentry))
   return false;  
   /*printf("\ntmp Block size %lu; Available blocks: %lu\n",
	 fsentry.f_bsize,
	 fsentry.f_bavail);*/
   return (fsentry.f_bavail > ((value/fsentry.f_bsize)+1));   
}


#endif

#ifdef ENABLE_SINDOH_MOBILE_SCAN
void ipp_getMobileScanApp_Attr(ipp_attr_groups_t *grps, stScan_GoScan *pPrintAddInfo, stShmSendServer *sendServerInfo)
{
   DBG_MSG("\n Parsing Scan Attrubutes %s:%d\n",__FUNCTION__, __LINE__);
#define MIN_SCAN_BRIGHTNESS       1
#define MAX_SCAN_BRIGHTNESS       9
#define MIN_SCAN_CONTRAST         1
#define MAX_SCAN_CONTRAST         5
#define MIN_SCAN_ERASEERBORDER    2
#define MAX_SCAN_ERASEERBORDER    99
   enum UI_SCAN_DEFALUT_DESTINATION
   {
      UI_SCAN_TO_THUMBDRIVE           = 0,              
      UI_SCAN_TO_EMAIL                = 1,              
      UI_SCAN_TO_FTP                  = 2,              
      UI_SCAN_TO_SMB                  = 3,              
      UI_SCAN_TO_WEBDAV               = 40,             
      UI_SCAN_DEFALUT_DESTINATION_MAX =4,               
   };
   enum UI_SCAN_COLOR
   {
      UI_SCAN_COLOR_COLOR    = 0,
      UI_SCAN_COLOR_GRAY     = 1,
      UI_SCAN_COLOR_BW_TEXT  = 2,
      UI_SCAN_COLOR_BW_PHOTO = 3,
      UI_SCAN_COLOR_BLANK1   = 4,
      UI_SCAN_COLOR_BLANK2   = 5,
      UI_SCAN_COLOR_MAX      = 4,
   };
   enum UI_SCAN_RESOLUTION
   {
      UI_SCAN_RESOLUTION_75x75   = 1,
      UI_SCAN_RESOLUTION_100x100 = 2,
      UI_SCAN_RESOLUTION_150x150 = 3,
      UI_SCAN_RESOLUTION_200x200 = 4,
      UI_SCAN_RESOLUTION_300x300 = 5,
      UI_SCAN_RESOLUTION_600x600 = 6,
      UI_SCAN_RESOLUTION_MAX     = 6,
   };
   typedef enum
   {
      UI_SCAN_FILE_FORMAT_TIFF = 0,
      UI_SCAN_FILE_FORMAT_PDF,
      UI_SCAN_FILE_FORMAT_JPG,
      UI_SCAN_FILE_FORMAT_secPDF,
      UI_SCAN_FILE_FORMAT_TIFF_MULTY_PAGES,
      UI_SCAN_FILE_FORMAT_PDF_MULTY_PAGES,
      UI_SCAN_FILE_FORMAT_secPDF_MULTY_PAGES,
      UI_SCAN_FILE_FORMAT_BLANK1,
      UI_SCAN_FILE_FORMAT_BLANK2,
      UI_SCAN_FILE_FORMAT_MAX  = 7,
   } UI_SCAN_FILE_FORMAT;
   typedef enum
   {
      UI_SCAN_PAGE_TYPE_SINGLE = 0,
      UI_SCAN_PAGE_TYPE_MULTY  = 1,
      UI_SCAN_PAGE_TYPE_MAX    = 2,
   } UI_SCAN_PAGE_TYPE;
   enum UI_SCAN_JPEG_QUALITY
   {
      UI_SCAN_JPEG_QUALITY_LOW  = 0,
      UI_SCAN_JPEG_QUALITY_MID  = 1,
      UI_SCAN_JPEG_QUALITY_HIGH =2,
      UI_SCAN_JPEG_QUALITY_MAX  = 3,
   };
   enum UI_SCAN_CONTRAST_NUMBER
   {
      UI_SCAN_CONTRAST_M2        =0,
      UI_SCAN_CONTRAST_M1,
      UI_SCAN_CONTRAST_0,
      UI_SCAN_CONTRAST_1,
      UI_SCAN_CONTRAST_2,
      UI_SCAN_CONTRAST_BLANK,
      UI_SCAN_CONTRAST_NUMBER_MAX=5,
   };
   enum UI_SCAN_BRIGHTNESS_NUMBER
   {
      UI_SCAN_BRIGHTNESS_M4        = 0,
      UI_SCAN_BRIGHTNESS_M3,
      UI_SCAN_BRIGHTNESS_M2,
      UI_SCAN_BRIGHTNESS_M1,
      UI_SCAN_BRIGHTNESS_0,
      UI_SCAN_BRIGHTNESS_1,
      UI_SCAN_BRIGHTNESS_2,
      UI_SCAN_BRIGHTNESS_3,
      UI_SCAN_BRIGHTNESS_4,
      UI_SCAN_BRIGHTNESS_NUMBER_MAX=9,
   };
   enum UI_SCAN_DUPLEX
   {
      UI_SCAN_DUPLEX_1SIDE  = 0,
      UI_SCAN_DUPLEX_2SIDES = 3,
      UI_SCAN_DUPLEX_BLANK  = 10,
      UI_SCAN_DUPLEX_MAX    = 2,
   };
   enum UI_SCAN_FUNCTION_NUMBER
   {
      UI_Scan_NOPFuncNo = 0,
      UI_Scan_DftDestFuncNo,
      UI_Scan_DftDestFTPFuncNo,
      UI_Scan_DftDestEmailFuncNo,
      UI_Scan_DftDestEmailTitleFuncNo,
      UI_Scan_ColorMenuFuncNo,
      UI_Scan_ResolutionFuncNo,
      UI_Scan_OriginalSizeFuncNo,
      UI_Scan_FileFormatFuncNo,
      UI_Scan_JPEGQualityFuncNo,
      UI_Scan_ContrastFuncNo,
      UI_Scan_BrightnessFuncNo,
      UI_Scan_DuplexFuncNo,
      UI_Scan_EraserBorderFuncNo,
      UI_Scan_EraserBorderTopFuncNo,
      UI_Scan_EraserBorderBottomFuncNo,
      UI_Scan_EraserBorderLeftFuncNo,
      UI_Scan_EraserBorderRightFuncNo,
      UI_Scan_UnsupportedYetFuncNo,
   };
   typedef enum
   {
      SCAN_AUTO,
      SCAN_HALFTONE,
      SCAN_LINE_ART,
      SCAN_MAGAZINE,
      SCAN_PHOTO,
      SCAN_TEXT,
      SCAN_TEXT_AND_PHOTO
   } CONTENT_TYPE;
   typedef struct
   {
      CONTENT_TYPE type_enum;
      char         *name;
   } scan_content_type_map_t;
   scan_content_type_map_t pscan_content_type_map_t[] =
   {
      { SCAN_AUTO,           "auto"           },
      { SCAN_PHOTO,          "halftone"       },
      { SCAN_PHOTO,          "line-art"       },
      { SCAN_PHOTO,          "magazine"       },
      { SCAN_PHOTO,          "photo"          },
      { SCAN_TEXT,           "text"           },
      { SCAN_TEXT_AND_PHOTO, "text-and-photo" },
   };
#define SCAN_CONTENT_TYPE_MAP_CNT    (sizeof(pscan_content_type_map_t) / sizeof(scan_content_type_map_t))
   int i = 0;
   pPrintAddInfo->act           = SCAN_ACTION_START;
   pPrintAddInfo->addInfo       = 0;
   pPrintAddInfo->pagetype      = 0;
   pPrintAddInfo->destination   = UI_SCAN_TO_FTP;
   pPrintAddInfo->fileformat    = UI_SCAN_FILE_FORMAT_JPG;
   pPrintAddInfo->pagetype      = UI_SCAN_PAGE_TYPE_SINGLE;
   pPrintAddInfo->dpi_hori      = 150;
   pPrintAddInfo->dpi_vert      = 150;
   pPrintAddInfo->sizecode      = MEDIASIZE_A4;
   pPrintAddInfo->contrast      = 3;
   pPrintAddInfo->brightness    = 5;
   pPrintAddInfo->border_top    = 0;
   pPrintAddInfo->border_bottom = 0;
   pPrintAddInfo->border_left   = 0;
   pPrintAddInfo->border_right  = 0;
   int nscan_content_type;
   int nscan_color_mode;
   typedef enum
   {
      SCAN_COLOR_AUTO,
      SCAN_COLOR_BI_LABEL,
      SCAN_COLOR_COLOR,
      SCAN_COLOR_MONOCHROME
   } SCAN_COLOR_MODE;
   typedef struct
   {
      SCAN_COLOR_MODE mode_enum;
      char            *name;
   } scan_color_mode_map_t;
   scan_color_mode_map_t pscan_color_mode_map_t[] =
   {
      { SCAN_COLOR_AUTO,       "auto"       },
      { SCAN_COLOR_BI_LABEL,   "bi-label"   },
      { SCAN_COLOR_COLOR,      "color"      },
      { SCAN_COLOR_MONOCHROME, "monochrome" },
   };
#define SCAN_COLOR_MODE_MAP_CNT    (sizeof(pscan_color_mode_map_t) / sizeof(scan_color_mode_map_t))
   typedef struct
   {
      UI_SCAN_FILE_FORMAT mime_enum;
      char                *name;
   } scan_mime_type_map_t;
   scan_mime_type_map_t pscan_mime_type_map_t[] =
   {
      { UI_SCAN_FILE_FORMAT_PDF,  "application/pdf" },
      { UI_SCAN_FILE_FORMAT_TIFF, "image/tiff"      },
      { UI_SCAN_FILE_FORMAT_JPG,  "image/jpeg"      },
   };
#define SCAN_MIME_TYPE_MAP_CNT    (sizeof(pscan_mime_type_map_t) / sizeof(scan_mime_type_map_t))
   typedef struct
   {
      UI_SCAN_PAGE_TYPE doc_type_enum;
      char              *name;
   } scan_doc_type_map_t;
   scan_doc_type_map_t pscan_doc_type_map_t[] =
   {
      { UI_SCAN_PAGE_TYPE_SINGLE, "single-document"                      },
      { UI_SCAN_PAGE_TYPE_SINGLE, "single-document-new-sheet"            },
      { UI_SCAN_PAGE_TYPE_MULTY,  "separate-documents-uncollated-copies" },
      { UI_SCAN_PAGE_TYPE_MULTY,  "separate-documents-collated-copies"   },
   };
#define SCAN_DOC_TYPE_MAP_CNT    (sizeof(pscan_doc_type_map_t) / sizeof(scan_doc_type_map_t))
   int orientation = 3;
   if (grps)
   {
      ipp_u32bit_t groupIndex = 0;
      for (groupIndex = 0; groupIndex < grps->num_grp; groupIndex++)
      {
         ipp_attr_grp_t *grp = grps->grp + groupIndex;
         if (grp)
         {
            ipp_u32bit_t a = 0;
            printf("  GROUP: %s", ipp_const_tag_str(grp->tag));
            for (a = 0; a < grp->num_attr; a++)
            {
               ipp_attr_t *attr = grp->attr + a;
               if (attr)
               {
                  ipp_u32bit_t v = 0;
                  printf("    ATTRIBUTE: %s", attr->name);
                  //Following block requires refinement and addition of parsing more attributes
                  //This will be also be modified for every new attribute added by Mobile APP.
                  for (v = 0; v < attr->num_val; v++)
                  {
                     if (strcasestr(attr->name, "input-contrast") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        pPrintAddInfo->contrast = getIntAttr(attr->val + v);
                        printf("    Contrast : %d", pPrintAddInfo->contrast);
                        if ((pPrintAddInfo->contrast < MIN_SCAN_CONTRAST) || (pPrintAddInfo->contrast > MAX_SCAN_CONTRAST))
                        {
                           pPrintAddInfo->contrast = UI_SCAN_CONTRAST_1;
                        }
                     }
                     else if (strcasestr(attr->name, "input-brightness") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        pPrintAddInfo->brightness = getIntAttr(attr->val + v);
                        printf("    Brightness : %d", pPrintAddInfo->brightness);
                        if ((pPrintAddInfo->brightness < MIN_SCAN_BRIGHTNESS) || (pPrintAddInfo->brightness > MAX_SCAN_BRIGHTNESS))
                        {
                           pPrintAddInfo->brightness = UI_SCAN_BRIGHTNESS_0;
                        }
                     }
                     else if (strcasestr(attr->name, "input-quality") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        pPrintAddInfo->quantlevel = getIntAttr(attr->val + v);
                        printf("    JPEG Quality : %d", pPrintAddInfo->quantlevel);
                        if ((pPrintAddInfo->quantlevel < UI_SCAN_JPEG_QUALITY_LOW) || (pPrintAddInfo->quantlevel > UI_SCAN_JPEG_QUALITY_MAX))
                        {
                           pPrintAddInfo->quantlevel = UI_SCAN_JPEG_QUALITY_MID;
                        }
                     }
                     else if (strcasestr(attr->name, "input-sides") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *sides = getStringAttr(attr->val + v);
                        if (sides)
                        {
                           printf("    Duplex : %s", sides);
                        }
                        if (sides == NULL)
                        {
                           pPrintAddInfo->source = 0;
                        }
                        else if (strcasestr(sides, "one-sided") != NULL)
                        {
                           pPrintAddInfo->source = 0;
                        }
                        else if (strcasestr(sides, "two-sided-short-edge") != NULL)
                        {
                           pPrintAddInfo->source = 1;
                        }
                        else if (strcasestr(sides, "two-sided-long-edge") != NULL)
                        {
                           pPrintAddInfo->source = 1;
                        }
                        if (sides)
                        {
                           free(sides);
                           sides = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "input-media") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *media = getStringAttr(attr->val + v);
                        if (media)
                        {
                           printf("    Media Size : %s", media);
                        }
                        if (media)
                        {
                           pPrintAddInfo->sizecode = sm_media_size_name_to_enum(media);
                           free(media);
                           media = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "input-orientation-requested") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        orientation = getIntAttr(attr->val + v);
                        printf("    Orientation Requested : %d", orientation);
                     }
                     else if (strcasestr(attr->name, "input-resolution") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        int dpi = getIntAttr(attr->val + v);
                        printf("    DPI : %d", dpi);
                        switch (dpi)
                        {
                        case UI_SCAN_RESOLUTION_75x75:
                           pPrintAddInfo->dpi_hori = 75;
                           pPrintAddInfo->dpi_vert = 75;
                           break;
                        case UI_SCAN_RESOLUTION_100x100:
                           pPrintAddInfo->dpi_hori = 100;
                           pPrintAddInfo->dpi_vert = 100;
                           break;
                        case UI_SCAN_RESOLUTION_150x150:
                           pPrintAddInfo->dpi_hori = 150;
                           pPrintAddInfo->dpi_vert = 150;
                           break;
                        case UI_SCAN_RESOLUTION_200x200:
                           pPrintAddInfo->dpi_hori = 200;
                           pPrintAddInfo->dpi_vert = 200;
                           break;
                        case UI_SCAN_RESOLUTION_300x300:
                           pPrintAddInfo->dpi_hori = 300;
                           pPrintAddInfo->dpi_vert = 300;
                           break;
                        case UI_SCAN_RESOLUTION_600x600:
                           pPrintAddInfo->dpi_hori = 600;
                           pPrintAddInfo->dpi_vert = 600;
                           break;
                        default:
                           pPrintAddInfo->dpi_hori = 300;
                           pPrintAddInfo->dpi_vert = 300;
                           break;
                        }
                     }
                     else if (strcasestr(attr->name, "input-content-type") != NULL)
                     {
                        char *strContentType = getStringAttr(attr->val + v);
                        if (strContentType)
                        {
                           printf("    ContentType : %s", strContentType);
                           for (i = 0; i < SCAN_CONTENT_TYPE_MAP_CNT; i++)
                           {
                              if (0 == strcmp(pscan_content_type_map_t[i].name, strContentType))
                              {
                                 nscan_content_type = pscan_content_type_map_t[i].type_enum;
                                 break;
                              }
                           }
                           free(strContentType);
                           strContentType = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "input-color-mode") != NULL)
                     {
                        char *strcolorMode = getStringAttr(attr->val + v);
                        if (strcolorMode)
                        {
                           printf("    ColorMode : %s", strcolorMode);
                           for (i = 0; i < SCAN_COLOR_MODE_MAP_CNT; i++)
                           {
                              if (0 == strcmp(pscan_color_mode_map_t[i].name, strcolorMode))
                              {
                                 nscan_color_mode = pscan_color_mode_map_t[i].mode_enum;
                                 break;
                              }
                           }
                           free(strcolorMode);
                           strcolorMode = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "document-format") != NULL)
                     {
                        char *strMimeType = getStringAttr(attr->val + v);
                        if (strMimeType)
                        {
                           printf("    Doc Format : %s", strMimeType);
                           for (i = 0; i < SCAN_MIME_TYPE_MAP_CNT; i++)
                           {
                              if (0 == strcmp(pscan_mime_type_map_t[i].name, strMimeType))
                              {
                                 pPrintAddInfo->fileformat = pscan_mime_type_map_t[i].mime_enum;
                                 break;
                              }
                           }
                           free(strMimeType);
                           strMimeType = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "multiple-document-handling") != NULL)
                     {
                        char *strDocType = getStringAttr(attr->val + v);
                        if (strDocType)
                        {
                           printf("    Multi Doc handling : %s", strDocType);
                           for (i = 0; i < SCAN_DOC_TYPE_MAP_CNT; i++)
                           {
                              if (0 == strcmp(pscan_doc_type_map_t[i].name, strDocType))
                              {
                                 pPrintAddInfo->pagetype = pscan_doc_type_map_t[i].doc_type_enum;
                                 break;
                              }
                           }
                           free(strDocType);
                           strDocType = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "ftp-info-ip") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *ftp_info_ip = getStringAttr(attr->val + v);
                        if (ftp_info_ip)
                        {
                           strncpy(sendServerInfo->serverAddress, ftp_info_ip, sizeof(sendServerInfo->serverAddress));
                           free(ftp_info_ip);
                           ftp_info_ip = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "ftp-info-port") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *ftp_info_port = getStringAttr(attr->val + v);
                        if (ftp_info_port)
                        {
                           sendServerInfo->serverPort = atoi(ftp_info_port);
                           free(ftp_info_port);
                           ftp_info_port = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "ftp-info-user") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *ftp_info_user = getStringAttr(attr->val + v);
                        if (ftp_info_user)
                        {
                           strncpy(sendServerInfo->serverId, ftp_info_user, sizeof(sendServerInfo->serverId));
                           free(ftp_info_user);
                           ftp_info_user = NULL;
                        }
                     }
                     else if (strcasestr(attr->name, "ftp-info-password") != NULL)
                     {
                        //Handling need to be corrected as per RFC and supported options
                        char *ftp_info_password = getStringAttr(attr->val + v);
                        if (ftp_info_password)
                        {
                           strncpy(sendServerInfo->serverPassword, ftp_info_password, sizeof(sendServerInfo->serverPassword));
                           free(ftp_info_password);
                           ftp_info_password = NULL;
                        }
                     }
                     else
                     {
                        char *junk = getStringAttr(attr->val + v);
                        if (junk)
                        {
                           printf("   other not added: %s", junk);
                           free(junk);
                        }
                     }
                  }
               }
            }
         }
      }
   }
   switch (nscan_content_type)
   {
   case SCAN_AUTO:
      switch (nscan_color_mode)
      {
      case SCAN_COLOR_AUTO:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_BI_LABEL:
         pPrintAddInfo->color = UI_SCAN_COLOR_BW_PHOTO;
         break;
      case SCAN_COLOR_COLOR:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_MONOCHROME:
         pPrintAddInfo->color = UI_SCAN_COLOR_GRAY;
         break;
      default:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      }
      break;
   case SCAN_PHOTO:
      switch (nscan_color_mode)
      {
      case SCAN_COLOR_AUTO:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_BI_LABEL:
         pPrintAddInfo->color = UI_SCAN_COLOR_BW_PHOTO;
         break;
      case SCAN_COLOR_COLOR:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_MONOCHROME:
         pPrintAddInfo->color = UI_SCAN_COLOR_GRAY;
         break;
      default:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      }
      break;
   case SCAN_TEXT:
      switch (nscan_color_mode)
      {
      case SCAN_COLOR_AUTO:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_BI_LABEL:
         pPrintAddInfo->color = UI_SCAN_COLOR_BW_TEXT;
         break;
      case SCAN_COLOR_COLOR:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_MONOCHROME:
         pPrintAddInfo->color = UI_SCAN_COLOR_GRAY;
         break;
      default:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      }
      break;
   case SCAN_TEXT_AND_PHOTO:
      switch (nscan_color_mode)
      {
      case SCAN_COLOR_AUTO:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_BI_LABEL:
         pPrintAddInfo->color = UI_SCAN_COLOR_BW_PHOTO;
         break;
      case SCAN_COLOR_COLOR:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      case SCAN_COLOR_MONOCHROME:
         pPrintAddInfo->color = UI_SCAN_COLOR_GRAY;
         break;
      default:
         pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
         break;
      }
      break;
   default:
      pPrintAddInfo->color = UI_SCAN_COLOR_COLOR;
      break;
   }
   if ((orientation == 4) && (pPrintAddInfo->sizecode == MEDIASIZE_A5))
   {
      pPrintAddInfo->sizecode = MEDIASIZE_A5_ROTATED;
   }
}


int startscan(stShmSendServer *sendServerInfo, stScan_GoScan *gGoScan)
{
   printf("\nStart Scanning");
   stShmSendServer *ptr_save_file;
   ptr_save_file = (stShmSendServer *)shm_create(SHM_KEY_SEND_FUNCTION, sizeof(stShmSendServer));
   if (ptr_save_file == (void *)-1)
   {
      return(0);
   }
   memcpy(ptr_save_file, sendServerInfo, sizeof(stShmSendServer));
   int ret;
   ret = shm_detach(ptr_save_file);
   gGoScan->addInfo = shm_get_keyid(SHM_KEY_SEND_FUNCTION);
   SYS_UI_MESSAGE sndToSysMsg;
   sndToSysMsg.cmd = 0x0601;      // 06-01
   memcpy(sndToSysMsg.data, (UINT8 *)gGoScan, sizeof(stScan_GoScan));
   SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &sndToSysMsg);
   return(0);
}


#endif

int ipp_request(ipp_cntxt_t* cntxt)
{
  ipp_u8bit_t *buf = NULL;
  ipp_u8bit_t *data = NULL;
  int len = 0;
  char header_data[21]= {0};
  ipp_request_t *ipp_req = cntxt->ipp_req;
  
  ASSERT(ipp_req);

#ifdef ENABLE_SINDOH_MOBILE_APP
  bool isMobileApp = false;
  stShmGoPrint *pPrintAddInfo=NULL;
  static int cursPrintFile = -1;
#endif

  if (!(buf = (ipp_u8bit_t*)IPP_MALLOC(8))) return -1;
   
  if (8 != cntxt->reader(ipp_req, buf, 8))
  {
     IPP_FREE(buf);
     return -2;
  }

  len = 8;
  if (ipp_read_attributes(cntxt->reader, ipp_req, &buf, &len))
  {
     IPP_FREE(buf);
     return -3;
  }

  data = buf;
  IPP_TIME(ipp_req, "ipp_read_attributes");

  _dump_ipp(data, len, 1);

  ipp_u16bit_t reqver   = _get_unum(&data, &len, 2);
  ipp_u16bit_t reqop    = _get_unum(&data, &len, 2);
  ipp_u32bit_t reqid    = _get_unum(&data, &len, 4);
  ipp_u16bit_t status   = IPP_STAT_SRV_OPERATION_NOT_SUPPORTED;
  
  ipp_req->ipp_version    = reqver;
  ipp_req->ipp_request_op = reqop;
  ipp_req->ipp_request_id = reqid;
   
  cntxt->uri_authentication = URI_AUTH_NONE;

  cntxt->reqattr  = ipp_parse_attr_groups((void**)&data, &len);
  IPP_TIME(ipp_req, "ipp_parse_attr_groups");

  IPP_FREE(buf);
#ifdef ENABLE_SINDOH_MOBILE_APP
  //Ad Hoc method to differentiate request from AirPrint App or Sindoh Mobile APP
  char userAgent[256] = {0};
  if (http_resource_get_header_value(cntxt->ipp_req->http_hndl, "User-Agent", userAgent, 255) == OK)
  {
    if (strcmp(userAgent, "") == 0)
    {
      //DBG_MSG("Empty user agent -- set Sindoh Mobile APP");
      isMobileApp = true;
    }
    else
    // if (strstr(userAgent, "Sindoh") != NULL)
   	if (strcasestr(userAgent, DEFAULT_VENDOR) != NULL)
    {
      isMobileApp = true;
    }
    else
    {
      //DBG_MSG("\nRequest Not from Sindoh Mobile ");
      isMobileApp = false;
    }
  }
  else
  {
    isMobileApp = true;
  }

#endif
  DBG_VERBOSE("req %d: got reqop %d\n", ipp_req->http_hndl, reqop);
  
  // send 100-continue if it is expected  
  if ( ( http_resource_get_header_value( ipp_req->http_hndl, "Expect", header_data, sizeof(header_data) ) == OK ) &&
       ( strcmp( header_data, "100-continue" ) == 0 ) )
  {
      DBG_MSG("req %d: send 100 continue\n", ipp_req->http_hndl);
      http_resource_send_100_continue(ipp_req->http_hndl);
  }

  /* Now generate specific status and response attributes */
  switch (reqop)
  {
    case IPP_OPID_CUPS_GET_PRINTERS:
    {
         DBG_MSG("%s - CUPS Get Printers\n", __func__);
         strncpy(ipp_req->name,
                 "CUPS Get Printers",
                 sizeof(ipp_req->name));
         status = _cups_get_printers(cntxt);
       break;
    }
    
    case IPP_OPID_GET_PRINTER_ATTR:
    {
         DBG_MSG("%s - Get Printer Attributes\n", __func__);
         strncpy(ipp_req->name,
                 "Get Printer Attributes",
                 sizeof(ipp_req->name));
         status = _get_printer_attr(cntxt);
       break;
    }

    case IPP_OPID_VALIDATE_JOB:
      DBG_MSG("%s - Validate Job\n", __func__);
      strncpy(ipp_req->name, "Validate Job", sizeof(ipp_req->name));
      status = _validate_job(cntxt, false);
      break;

      case IPP_OPID_PRINT_JOB:
          {
              DBG_MSG("%s - Print Job\n", __func__);
#ifdef ENABLE_SINDOH_MOBILE_APP
/*
           Sindoh Mobile App Support
           Known Issues: 
           - No Chunked encoding support yet
           - Paper Type cannot be set, as PDF file itself contains paper type.
           - Status codes need to be rectified
           - Status code handling for cancelled job
           - Printing selected number of pages
           - No N-UP printing
*/
              if(isMobileApp)
              {
                 char   pdfFile[256]={0};
                 char   prnFile[256]={0};
                 pPrintAddInfo = (stShmGoPrint *)malloc(sizeof(stShmGoPrint));
                 if(pPrintAddInfo == NULL)
                 {
                    status = IPP_STAT_SRV_TEMPORARY_ERROR;
                    ipp_request_processing_complete(cntxt->ipp_req);
                    break;
                 }
                 memset(pPrintAddInfo, 0, sizeof(stShmGoPrint));
                 pPrintAddInfo->fileCount = 1;
                 pPrintAddInfo->duplex    = 0;
                 pPrintAddInfo->inTray    = 1;
                 pPrintAddInfo->darkness  = 6;
                 pPrintAddInfo->repeat    = 1;
                 ipp_getMobileApp_Attr(cntxt->reqattr, &pPrintAddInfo);
                   
                 //ipp_request_t *ipp_reqdata = (ipp_request_t *)cntxt->reader_data;
                 if(ipp_req == NULL)
                 {
                    status = IPP_STAT_SRV_TEMPORARY_ERROR;
                    ipp_request_processing_complete(cntxt->ipp_req);
                    break;
                 }
                 unsigned int length = 0;
                 http_resource_get_content_length(ipp_req->http_hndl, &length);
                 if(checkMemOK(length) == false)
                 {
                    status = IPP_STAT_SRV_TEMPORARY_ERROR;
                    ipp_request_processing_complete(cntxt->ipp_req);
                    printf("\nLow Memory..Cannot Print");
                    break;
                 }
                 if(length)
                 {
                   http_set_tcp_recv_buf_size(ipp_req->http_hndl, IPP_BULK_TCP_RECV_BUF);
                   //http_resource_send_100_continue(ipp_req->http_hndl);
                   char *pdfData        = (char *)malloc(MOBILE_PRINT_BUF_SIZE + 1);
                   if(pdfData == NULL)
                   {
                     status = IPP_STAT_SRV_TEMPORARY_ERROR;
                     ipp_request_processing_complete(cntxt->ipp_req);
                     printf("\nLow Memory..Cannot Print");
                     break;
                   }
                   memset(pdfData,0,MOBILE_PRINT_BUF_SIZE + 1);
                   ipp_lock(1);
                   cursPrintFile++;
                   cursPrintFile%=MAX_SPRINT_FILES;
                   ipp_lock(0);
                   FILE *fpprintFile = NULL;
                   FILE *filePrintFP = NULL;
                   #if 0
                   tmpnam(pdfFile);
                   strcpy(prnFile,pdfFile);
                   strcat(pdfFile,".pdf");
                   while (file_exists(pdfFile))
                   {
                      tmpnam(pdfFile);
                      strcpy(prnFile,pdfFile);
                      strcat(pdfFile,".pdf");
                   }
                   #endif
                   if(!isDirectory(MOBILE_PRINT_FILEPATH))
                   {
                      mkdir(MOBILE_PRINT_FILEPATH,0666);
                      sync();
                   }
                   snprintf(pdfFile,sizeof(pdfFile),"%s/file%d",MOBILE_PRINT_FILEPATH,cursPrintFile);
                   snprintf(prnFile,sizeof(prnFile),"%s",pdfFile);
                   if ((fpprintFile = fopen(pdfFile, "w")) == NULL)
                   {
                     if(pPrintAddInfo)
                     {
                       free(pPrintAddInfo);
                       pPrintAddInfo=NULL;
                     }
                     if (pdfData)
                     {
                       free(pdfData);
                       pdfData=NULL;
                     } 
                     ipp_request_processing_complete(cntxt->ipp_req);
                     status = IPP_STAT_SRV_TEMPORARY_ERROR;
                     break;
                   }
                   long bytesret = 0;
                   long totalbytesret = 0; //For debugging
                   while ((bytesret = cntxt->reader(ipp_req, pdfData, MOBILE_PRINT_BUF_SIZE)) > 0)
                   {
                     totalbytesret += bytesret;
                     if(checkMemOK(bytesret) == false)
                     {
                        status = IPP_STAT_SRV_TEMPORARY_ERROR;
                        printf("\nLow Memory..Cannot Print");
                        ipp_request_processing_complete(cntxt->ipp_req);
                        break;
                     }
                     fwrite(pdfData, 1, bytesret, fpprintFile);
                     memset(pdfData,0,MOBILE_PRINT_BUF_SIZE + 1);
                   }  
                   //printf("\nTotal %ld",totalbytesret);
                   if (pdfData)
                   {
                     free(pdfData);
                     pdfData=NULL;
                   }            
                   fclose(fpprintFile); 
                   if(status == IPP_STAT_SRV_TEMPORARY_ERROR)
                   {
                     if(pPrintAddInfo)
                     {
                       free(pPrintAddInfo);
                       pPrintAddInfo=NULL;
                     }
                     ipp_request_processing_complete(cntxt->ipp_req);
                     unlink(pdfFile);
                     break;
                   }
                   strcat(prnFile,".prn");
                   filePrintFP = fopen(prnFile, "w");
                   if (filePrintFP)
                   {
                     fprintf(filePrintFP, "@PJL ENTER LANGUAGE=PDF_FILE\n");
                     fprintf(filePrintFP, "%s\n", pdfFile);
                     fclose(filePrintFP);
                   }
                   else
                   {
                     DBG_ERR("[%s]:   File Open Error \n", __FUNCTION__);
                     if(pPrintAddInfo)
                     {
                       free(pPrintAddInfo);
                       pPrintAddInfo=NULL;
                     }
                     ipp_request_processing_complete(cntxt->ipp_req);
                     status = IPP_STAT_SRV_TEMPORARY_ERROR;
                     break;
                   }
                   char *fileBaseName = basename(pdfFile);
                   if (fileBaseName == NULL)
                   {
                     if(pPrintAddInfo)
                     {
                       free(pPrintAddInfo);
                       pPrintAddInfo=NULL;
                     }
                     ipp_request_processing_complete(cntxt->ipp_req);
                     status = IPP_STAT_SRV_TEMPORARY_ERROR;
                     break;
                   }
                   snprintf(pPrintAddInfo->fileName,sizeof(pPrintAddInfo->fileName),"%s",fileBaseName);
                   snprintf(pPrintAddInfo->filePath,sizeof(pPrintAddInfo->filePath),"%s",MOBILE_PRINT_FILEPATH);
                   //printf("\nFile Print %s/%s","/tmp",pPrintAddInfo->fileName?pPrintAddInfo->fileName:"NULL");
                   //printf("\nprnFIle =%s",prnFile);
                   printf("\npPrintAddInfo Properties : makeApp=%02x orginType=%02x sort=%02x inTray=%02x staple=%02x duplex=%02x coverSlip=%02x stamp=%02x fileCount=%02x repeat=%02x darkness=%02x\n", pPrintAddInfo->makeApp, pPrintAddInfo->orginType, pPrintAddInfo->sort, pPrintAddInfo->inTray, pPrintAddInfo->staple, pPrintAddInfo->duplex, pPrintAddInfo->coverSlip, pPrintAddInfo->stamp, pPrintAddInfo->fileCount, pPrintAddInfo->repeat, pPrintAddInfo->darkness);
                   while(isSprintBusy)
                   {
                      printf("\nMobile print Busy..Waiting\n");
                      sleep(1);
                   }
                   if (!smjob_accepting_jobs())
                   {  
                     DBG_ERR("Not accepting jobs!\n");
                     ipp_request_processing_complete(cntxt->ipp_req);
                     status = IPP_STAT_SRV_NOT_ACCEPTING_JOBS;
                     break;
                   }
                   ipp_lock(1);
                   isSprintBusy = true;
                   ipp_lock(0);
                   file_to_pipe(prnFile, cbMobilePrintDone, pPrintAddInfo, e_USBDRIVEPDFPAGE);
                   ipp_request_processing_complete(cntxt->ipp_req);
                   status = IPP_STAT_OK;
               }
               else
               {
                   printf("\nInvalid Content Length \n===Exit Mobile App Printing====");
                   ipp_request_processing_complete(cntxt->ipp_req);
                   status = IPP_STAT_SRV_TEMPORARY_ERROR;
                   break;
               }
              }
              else
              {
#endif            
              http_set_tcp_recv_buf_size(ipp_req->http_hndl, IPP_BULK_TCP_RECV_BUF);

              strncpy(ipp_req->name, "Print Job", sizeof(ipp_req->name));
              status = _print_job(cntxt);
              if(status == (ipp_u16bit_t)-2)
              {
                  DBG_MSG("req %d: Reset Connection\n", ipp_req->http_hndl);
                  http_resource_reset_connection(ipp_req->http_hndl);
              }
#ifdef ENABLE_SINDOH_MOBILE_APP
              }
#endif  
          }
      break;

    case IPP_OPID_GET_JOBS:
       DBG_MSG("%s - Get Jobs\n", __func__);
       strncpy(ipp_req->name, "Get Jobs", sizeof(ipp_req->name));
       status = _get_jobs(cntxt);
       break;

    case IPP_OPID_CANCEL_JOB:
       DBG_MSG("%s - Cancel Job\n", __func__);
       strncpy(ipp_req->name, "Cancel Job", sizeof(ipp_req->name));
       status = _cancel_job(cntxt);
       break;

    case IPP_OPID_GET_JOB_ATTR:
      DBG_MSG("%s - Get Job Attributes\n", __func__);
      strncpy(ipp_req->name, "Get Job Attributes", sizeof(ipp_req->name));
      status = _get_job(cntxt);
      break;

    case IPP_OPID_IDENTIFY_PRINTER:
      status = _identify_printer(cntxt);
      break;

    case IPP_OPID_CREATE_JOB:
      DBG_MSG("%s - Create Job\n", __func__);
#ifdef ENABLE_SINDOH_MOBILE_SCAN
      if (strcasestr(ipp_req->http_hdr, "POST /ipp/scan/"))
      {
        DBG_ERR("Request from Mobile Scan App %s\n", __func__);
        stScan_GoScan *pScanInfo = (stScan_GoScan *)malloc(sizeof(stScan_GoScan));
        if (pScanInfo == NULL)
        {
          status = IPP_STAT_SRV_TEMPORARY_ERROR;
          DBG_ERR("Cannot Allocate stScan_GoScan %s\n", __func__);
          break;
        }
        memset(pScanInfo, 0, sizeof(stScan_GoScan));
        stShmSendServer *sendServerInfo = (stShmSendServer *)malloc(sizeof(stShmSendServer));
        if (sendServerInfo == NULL)
        {
          status = IPP_STAT_SRV_TEMPORARY_ERROR;
          DBG_ERR("Cannot Allocate stShmSendServer %s\n", __func__);
          free(pScanInfo);
          break;
        }
        memset(sendServerInfo, 0, sizeof(stShmSendServer));
        ipp_getMobileScanApp_Attr(cntxt->reqattr, pScanInfo, sendServerInfo);
        startscan(sendServerInfo, pScanInfo);
        free(pScanInfo);
        free(sendServerInfo);
      }
      else
      {
        strncpy(ipp_req->name, "Create Job", sizeof(ipp_req->name));
        status = _create_job(cntxt);
      }
#else
      strncpy(ipp_req->name, "Create Job", sizeof(ipp_req->name));
      status = _create_job(cntxt);
#endif

       break;

    case IPP_OPID_SEND_DOCUMENT:
      {
         DBG_MSG("%s - Send Document\n", __func__);

         http_set_tcp_recv_buf_size(ipp_req->http_hndl, IPP_BULK_TCP_RECV_BUF);

         strncpy(ipp_req->name, "Send Document", sizeof(ipp_req->name));

         status = _send_document(cntxt);
         if(status == (ipp_u16bit_t)-2)
         {
           DBG_MSG("req %d: Reset Connection\n", ipp_req->http_hndl);
           http_resource_reset_connection(ipp_req->http_hndl);
         }
         
         break;
      }
      
    default:
      DBG_MSG("%s - Unsupported request: %x\n", __func__, reqop);

      strncpy(ipp_req->name, "Unsupported Request", sizeof(ipp_req->name));

       _handle_unsupported_req(cntxt);
       DBG_ERR( "req %d: unrecognized reqop %d", ipp_req->http_hndl, reqop);
      break;
  }

  if (reqop != IPP_OPID_PRINT_JOB && reqop != IPP_OPID_SEND_DOCUMENT)
  {
      IPP_TIME(ipp_req, "generated response");
      ipp_request_processing_complete(ipp_req);
  }

  ipp_req->ipp_status = status;
  return 0;
}

bool ipp_chk_multiple_docs_per_job_supported()
{
   return(allow_multiple_docs_per_job);
}

bool ipp_chk_job_attr_valid_for_status(uint8_t job_attr, ipp_u16bit_t status)
{
    bool result = true;
    
    switch (job_attr)
    {
        case JOB_URI:
        case JOB_ID:
            if (status == IPP_STAT_CLI_BAD_REQUEST)
            {
                result = false;
            }
            break;
            
        case JOB_STATE:
        case JOB_STATE_REASONS:
            if (status == IPP_STAT_CLI_BAD_REQUEST)
            {
                result = false;
            }
            break;
        
        default:
            result = true;
            break;
    }
    
    return result;
}

static ipp_attr_grp_t* _printer_job_attributes(ipp_cntxt_t* cntxt, uint32_t job_id, ipp_u16bit_t status)
{
  smjob_rcode_t smjob_rcode;
  smjob_status_t job;
  
  smjob_rcode = smjob_get_status(job_id, &job);
  if (smjob_rcode != SMJOB_OK)
  {
    DBG_ERR( "_printer_job_attributes - ERROR smjob_get_status call failed!\n");
    return NULL;
  }

  smjob_ticket_t tck;
  smjob_rcode = smjob_get_ticket(job_id, &tck);
  if (smjob_rcode != SMJOB_OK)
  {
    DBG_ERR( "_printer_job_attributes - ERROR smjob_get_ticket call failed!\n");
    return NULL;
  }

  ipp_attr_grp_t* grp = ipp_create_attr_group(IPP_TAG_JOB_ATTRIBUTES);

  /* JSA - REVISIT - jobs need to be returned in a specified order.
                     see section 3.2.6.2 of rfc 2911 */

  /* JSA - this is where we would parse through reqattr and fill in
           accordingly (see table 4.3 in rfc 2911) */
  if (has_job_attr_been_requested(JOB_URI) && ipp_chk_job_attr_valid_for_status(JOB_URI, status))
  {
     _add_str_attr(grp, 
                   "job-uri", 
                   IPP_TAG_URI,
                   "%s://%s:%d/%s/"JOB_ID_STR"%8.8x",
                   cntxt->protocol,
                                             cntxt->server_address,
                                             cntxt->port_number,
                   cntxt->root_uri,
                   (unsigned int)job_id);
  }

  if (has_job_attr_been_requested(JOB_ID) && ipp_chk_job_attr_valid_for_status(JOB_ID, status))
  {
     ipp_add_attr_with_int(grp, "job-id", job.job_id); 
  }

  if (has_job_attr_been_requested(JOB_PRINTER_URI) && ipp_chk_job_attr_valid_for_status(JOB_PRINTER_URI, status))
  {
     _add_str_attr(grp, "job-printer-uri", IPP_TAG_URI,
                   "%s://%s:%d/%s/", cntxt->protocol,
                                     cntxt->server_address,
                                     cntxt->port_number,
                                     cntxt->root_uri);
  }

  if (has_job_attr_been_requested(JOB_UUID) && ipp_chk_job_attr_valid_for_status(JOB_UUID, status))
  {
     ipp_attr_t *attr = _get_job_uuid(job_id);

     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
  }

  if (has_job_attr_been_requested(JOB_STATE) && ipp_chk_job_attr_valid_for_status(JOB_STATE, status))
  {
     ipp_add_attr_with_enum(grp, "job-state", job.state); 
  }

  if (has_job_attr_been_requested(JOB_STATE_REASONS) && ipp_chk_job_attr_valid_for_status(JOB_STATE_REASONS, status))
  {
    uint32_t i = 0;
    ipp_attr_t *attr = ipp_create_attr("job-state-reasons", 0);
    
    while (i < job.num_reasons)
    {
        char *job_state_reason_str = smjob_get_job_state_reasons_str(job.reasons[i]);
        ipp_attr_val_t *val = ipp_create_attr_val(IPP_TAG_KEYWORD, job_state_reason_str, strlen(job_state_reason_str));
        
        ipp_add_attr_val(attr, val);
        ipp_free_attr_val(val, 1);
        
        i++;
    }

    ipp_add_attr(grp, attr);
    ipp_free_attr(attr, 1);
  }

  if (has_job_attr_been_requested(JOB_NAME) && ipp_chk_job_attr_valid_for_status(JOB_NAME, status))
  {
     ipp_add_attr_with_name(grp, "job-name", tck.job_description.job_name);
  }

  if (has_job_attr_been_requested(JOB_ORIGINATING_USER_NAME) && ipp_chk_job_attr_valid_for_status(JOB_ORIGINATING_USER_NAME, status))
  {
     ipp_add_attr_with_name(grp, "job-originating-user-name",
                            tck.job_description.originating_user_name);
  }

  if (has_job_attr_been_requested(TIME_AT_CREATION) && ipp_chk_job_attr_valid_for_status(TIME_AT_CREATION, status))
  {
     ipp_add_attr_with_int(grp, "time-at-creation", 0);
  }

  if (has_job_attr_been_requested(TIME_AT_PROCESSING) && ipp_chk_job_attr_valid_for_status(TIME_AT_PROCESSING, status))
  {
     ipp_add_attr_with_int(grp, "time-at-processing", 0);
  }

  if (has_job_attr_been_requested(TIME_AT_COMPLETED) && ipp_chk_job_attr_valid_for_status(TIME_AT_COMPLETED, status))
  {
     ipp_add_attr_with_int(grp, "time-at-completed", 0);
  }

  if (has_job_attr_been_requested(JOB_PRINTER_UP_TIME) && ipp_chk_job_attr_valid_for_status(JOB_PRINTER_UP_TIME, status))
  {
     ipp_add_attr_with_int(grp, "job-printer-up-time", 0);
  }

  if (has_job_attr_been_requested(JOB_IMPRESSIONS) && ipp_chk_job_attr_valid_for_status(JOB_IMPRESSIONS, status))
  {
     ipp_add_attr_with_int(grp, "job-impressions", job.sheets_completed);
  }

  if (has_job_attr_been_requested(JOB_IMPRESSIONS_COMPLETED) && ipp_chk_job_attr_valid_for_status(JOB_IMPRESSIONS_COMPLETED, status))
  {
     ipp_add_attr_with_int(grp, "job-impressions-completed", job.sheets_completed);
  }



#if 0 /* These are optional but could be added */
  ipp_add_attr_with_val(grp, "job-state-message",
                        IPP_TAG_TEXT_WITHOUT_LANGUAGE,
                        "A nice message", 14); /* REVISIT */
  ipp_add_attr_with_int(grp, "number-of-intervening-jobs", 0); 
  /* .... many more - see table 4.3 in rfc 2911 */
#endif
  return grp;
}



// rdj feb-2014
static void ipp_get_requested_attribute(ipp_request_t *ipp_req, ipp_attr_grp_t* grp, ipp_attribute_t *attr)
{
    int32_t ret = SYS_OK;
    ipp_attr_t *resp_attr = NULL;
    ipp_attr_val_t *val;
    uint32_t oid_index;
    uint8_t oid_type;
    uint32_t oid_len;

    if(attr->oid == OID_INVALID && attr->trans_func == NULL)
    {
        // invalid attribute -- no way to acquire a value
        ASSERT(0);
        goto done; 
    }

    // could be -1 if attr->oid == OID_INVALID but this is OK
    oid_type = oid_type_id_get(attr->oid); 

    oid_index = 0;
    resp_attr = ipp_create_attr(attr->name, 0);
    do
    {
        XASSERT(OID_BUF_MAX_LEN >= sizeof(uint32_t), OID_BUF_MAX_LEN);

        if(attr->trans_func)
        {
            // use the translation func
            oid_len = OID_BUF_MAX_LEN;
            ret = attr->trans_func((ipp_req_hndl_t)ipp_req, attr->oid, oid_index, ipp_req->oid_buf, &oid_len);
        }
        else
        {
            // call the OID directly
            oid_len = 0;
            switch(oid_type) 
            {
                case OID_TYPE_STRING:
                {
                    ret = oid_get_string(attr->oid, oid_index, ipp_req->oid_buf, OID_BUF_MAX_LEN); 
                    if(ret == SYS_OK)
                    {
                        oid_len = strlen(ipp_req->oid_buf);
                    }
                    break; 
                }
                case OID_TYPE_UINT32:
                {
                    val = NULL;
                    ret = oid_get_uint32(attr->oid, oid_index, (uint32_t *)ipp_req->oid_buf);
                    if(ret == SYS_OK)
                    {
                        *((uint32_t *)ipp_req->oid_buf) = cpu_to_be32( *((uint32_t *)ipp_req->oid_buf) );
                        oid_len = sizeof(uint32_t);
                    }
                    break;
                }
                case OID_TYPE_BYTE_ARRAY:
                    break;
                default:
                    // unsupported oid_type
                    XASSERT(0, oid_type);
                    ret = SYS_FAIL;
                    break;
            }
        }

        if(ret == SYS_OK)
        {
            val = ipp_create_attr_val(attr->tag, ipp_req->oid_buf, oid_len);
            ipp_add_attr_val(resp_attr, val);
            ipp_free_attr_val(val, 1);
            oid_index++;
        }
        else if(ret == OID_ERROR_INDEX_UNAVAILABLE)
        {
            // val at this index is not available -- just skip it and continue
            oid_index++;
            ret = SYS_OK;
        }

    } while(ret == SYS_OK);

    if(oid_index)
    {
        // have at least one value so add the attribute
        ipp_add_attr(grp, resp_attr); 
    }
    ipp_free_attr(resp_attr, 1);

done:
    return;
}

static void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

static ipp_attr_grp_t* _get_printer_attributes(ipp_cntxt_t* cntxt)
{
    ipp_attr_t            *attr;
    ipp_attr_val_t        *val;
    uint8_t               oid_byte_array[OID_BYTE_ARRAY_SIZE];
    uint32_t              oid_byte_array_size;
    uint32_t              oid_val  = 0;
    char                  *oid_str = NULL;
    
    ipp_request_t *ipp_req = cntxt->ipp_req;
    ASSERT(ipp_req);

    oid_str = cntxt->ipp_req->oid_buf;
    ASSERT(oid_str);
    
    /* Group 3 - Printer Object Attributes */
    ipp_attr_grp_t* grp = ipp_create_attr_group(IPP_TAG_PRINTER_ATTRIBUTES);
    ASSERT(grp);

    // rdj feb-2014
    // check hash table for requested attributes
    if(cntxt->ipp_req->req_attrs.grp_mask)
    {
        // attribute group requests
        // respond with all attributes that apply to the requested group mask
        ipp_attribute_t *ipp_attr = NULL;
        map_iterator_t iter_hndl;
        iter_hndl = map_iterate_begin(cntxt->ipp_req->ipp_instance->attr_map);
        do
        {
            ipp_attr = (ipp_attribute_t *)map_iterate_next(iter_hndl);
            if(ipp_attr && (ipp_attr->flags & IPP_CAT_ATTR) &&
               (ipp_attr->flags & cntxt->ipp_req->req_attrs.grp_mask))
            {
                // attribute is in the requested group(s) so include it
                ipp_get_requested_attribute(ipp_req, grp, ipp_attr);
            }
        } while(ipp_attr); 
        map_iterate_end(iter_hndl);
    }
    if(cntxt->ipp_req->req_attrs.map != MAP_INVALID_HANDLE)
    {
        // individual attribute requests
        ipp_attribute_t *ipp_attr = NULL;
        map_iterator_t iter_hndl;
        iter_hndl = map_iterate_begin(cntxt->ipp_req->req_attrs.map);
        do
        {
            ipp_attr = (ipp_attribute_t *)map_iterate_next(iter_hndl);
            if(ipp_attr && !(ipp_attr->flags & cntxt->ipp_req->req_attrs.grp_mask))
            {
                // a unique attribute not already included in any requested group
                ipp_get_requested_attribute(ipp_req, grp, ipp_attr);
            }
        } while(ipp_attr); 
        map_iterate_end(iter_hndl);
    }

    // printer-uri-supported will be set in this module at this time.

    if (has_printer_attr_been_requested(PRINTER_URI_SUPPORTED))
    {
     _get_printer_uri_supported(grp, oid_str, OID_BUF_MAX_LEN, ipp_req);    
    }

    if (has_printer_attr_been_requested(PRINTER_SUPPLY_INFO_URI))
    {
     _get_printer_supply_info_uri(grp, oid_str, OID_BUF_MAX_LEN, ipp_req);
    }

    // uri-security-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(URI_SECURITY_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "uri-security-supported",
                           IPP_TAG_KEYWORD, "none", 4);
    }

    // uri-authentication-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(URI_AUTHENTICATION_SUPPORTED))
    {
     switch (cntxt->uri_authentication)
     {
        case URI_AUTH_REQUESTED_USER_NAME:
            ipp_add_attr_with_val(grp, "uri-authentication-supported",
                               IPP_TAG_KEYWORD, "requesting-user-name", 20);
            break;
        
        case URI_AUTH_BASIC:
            ipp_add_attr_with_val(grp, "uri-authentication-supported",
                               IPP_TAG_KEYWORD, "basic", 5);
            break;
        
        case URI_AUTH_DIGEST:
            ipp_add_attr_with_val(grp, "uri-authentication-supported",
                               IPP_TAG_KEYWORD, "digest", 6);
            break;
        
        case URI_AUTH_CERTIFICATE:
            ipp_add_attr_with_val(grp, "uri-authentication-supported",
                               IPP_TAG_KEYWORD, "certificate", 11);
            break;
        
        case URI_AUTH_NONE:
        default:
            ipp_add_attr_with_val(grp, "uri-authentication-supported",
                               IPP_TAG_KEYWORD, "none", 4);
            break;
     }
    }

    // identify-actions-default  will be set in this module at this time.
    if (has_printer_attr_been_requested(IDENTIFY_ACTIONS_DEFAULT))
    {
     ipp_add_attr_with_val(grp, "identify-actions-default",
                           IPP_TAG_KEYWORD, "flash", 5);
    }

    // identify-actions-supported  will be set in this module at this time.  
    if (has_printer_attr_been_requested(IDENTIFY_ACTIONS_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "identify-actions-supported",
                           IPP_TAG_KEYWORD, "flash", 5);
    }


    if (has_printer_attr_been_requested(PRINTER_NAME))
    {
     if (get_string_oid(OID_SM_SYSTEM_MAKE_AND_MODEL, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "printer-name",
                           IPP_TAG_NAME_WITHOUT_LANGUAGE, 
                           oid_str, 
                           strlen(oid_str));
     }
    }

    if (has_printer_attr_been_requested(COLOR_SUPPORTED))
    {
     if (get_uint32_oid(OID_PRINT_HAVE_COLOR, 0, &oid_val))
     {
        ipp_add_attr_with_bool(grp, "color-supported", oid_val);
     }
    }


    if (has_printer_attr_been_requested(PRINT_COLOR_MODE_DEFAULT))
    {
     if (get_string_oid(OID_SM_PRINT_COLOR_MODE_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "print-color-mode-default",
                           IPP_TAG_KEYWORD, 
                           oid_str, 
                           strlen(oid_str));
     }                           
    }

    if (has_printer_attr_been_requested(PRINT_COLOR_MODE_SUPPORTED))
    {
    	// Return monochrome only by mspyo, 2021-03-10
    	char *colorMode = "monochrome";
    	ipp_add_attr_with_val(grp, "print-color-mode-supported", IPP_TAG_KEYWORD, colorMode, strlen(colorMode));
	/*// blocked by mspyo, 2021-03-10
     attr = _ipp_add_mult_str_vals_to_attr("print-color-mode-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_PRINT_COLOR_MODE_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
	*/
    }


    if (has_printer_attr_been_requested(PRINTER_STATE))
    {
     if (get_uint32_oid(OID_SM_PRINTER_STATE, 0, &oid_val))
     {
        ipp_add_attr_with_enum(grp, "printer-state", oid_val);
     }
    }

    if (has_printer_attr_been_requested(PRINTER_STATE_REASONS))
    {
     attr = _ipp_add_mult_str_vals_to_attr("printer-state-reasons",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_PRINTER_STATE_REASONS,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    // ipp-versions-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(IPP_VERSIONS_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("ipp-versions-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_IPP_VERSIONS_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);

     //ipp_add_attr_with_val(grp, "ipp-versions-supported", /* page 134 rfc 2911 */
     //                      IPP_TAG_KEYWORD, "2.0", 3);
    }


    // operations-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(OPERATIONS_SUPPORTED))
    {
     attr = ipp_create_attr("operations-supported", 0);

     val = ipp_create_attr_val_enum(IPP_OPID_GET_PRINTER_ATTR);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_PRINT_JOB);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_GET_JOBS);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_CREATE_JOB);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_SEND_DOCUMENT);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_CANCEL_JOB);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_GET_JOB_ATTR);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_VALIDATE_JOB);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_IDENTIFY_PRINTER);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     val = ipp_create_attr_val_enum(IPP_OPID_CUPS_GET_PRINTERS);
     ipp_add_attr_val(attr, val);
     ipp_free_attr_val(val, 1);

     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

//    if (has_printer_attr_been_requested(CHARSET_CONFIGURED))
//    {
//     if (get_string_oid(OID_SM_CHARSET_CONFIGURED, 0, oid_str, OID_BUF_MAX_LEN))
//     {
//        ipp_add_attr_with_val(grp, "charset-configured",
//                          IPP_TAG_CHARSET, oid_str, strlen(oid_str));
//     }
//    }

    if (has_printer_attr_been_requested(CHARSET_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("charset-supported",
                                           IPP_TAG_CHARSET,
                                           OID_SM_CHARSET_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(NATURAL_LANGUAGE_CONFIGURED))
    {
     if (get_string_oid(OID_SM_NATURAL_LANGUAGE_CONFIGURED, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "natural-language-configured",
                           IPP_TAG_NATURAL_LANGUAGE, oid_str, strlen(oid_str));
     }
    }

    if (has_printer_attr_been_requested(GENERATED_NATURAL_LANGUAGE_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("generated-natural-language-supported",
                                           IPP_TAG_NATURAL_LANGUAGE,
                                           OID_SM_GENERATED_NATURAL_LANGUAGE_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(DOCUMENT_FORMAT_DEFAULT))
    {
     if (get_string_oid(OID_SM_PRINT_DOC_FORMAT_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "document-format-default",
                           IPP_TAG_MIME_MEDIA_TYPE, oid_str, strlen(oid_str));
     }
    }

    if (has_printer_attr_been_requested(DOCUMENT_FORMAT_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("document-format-supported",
                                           IPP_TAG_MIME_MEDIA_TYPE,
                                           OID_SM_PRINT_DOC_FORMAT_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(JOB_CREATION_ATTRIBUTES_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("job-creation-attributes-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_JOB_CREATION_ATTR_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(PRINTER_IS_ACCEPTING_JOBS))
    {
     bool accepting_jobs = smjob_accepting_jobs();
     ipp_add_attr_with_bool(grp, "printer-is-accepting-jobs", accepting_jobs);
    }

    if (has_printer_attr_been_requested(QUEUED_JOB_COUNT))
    {
     uint32_t queued_jobs = smjob_get_queued_job_count();
     ipp_add_attr_with_int(grp, "queued-job-count", queued_jobs);
    }

    // pdl-override will be set in this module at this time.
    if (has_printer_attr_been_requested(PDL_OVERRIDE_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "pdl-override-supported",
                           IPP_TAG_KEYWORD, "attempted", 9);
    }

    // printer-up-time will be set in this module at this time.
    if (has_printer_attr_been_requested(PRINTER_UP_TIME))
    {
     ipp_add_attr_with_int(grp, "printer-up-time", time(NULL));
    }


    if (has_printer_attr_been_requested(PRINTER_MAKE_AND_MODEL))
    {
     if (get_string_oid(OID_SM_SYSTEM_MAKE_AND_MODEL, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "printer-make-and-model",
                           IPP_TAG_TEXT_WITHOUT_LANGUAGE, oid_str, strlen(oid_str));
     }
    }

    if (has_printer_attr_been_requested(PRINTER_LOCATION))
    {
     if (get_string_oid(OID_SM_SYSTEM_LOCATION, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "printer-location",
                           IPP_TAG_TEXT_WITHOUT_LANGUAGE, oid_str, strlen(oid_str));
     }
    }

    if (has_printer_attr_been_requested(PRINTER_INFO))
    {
     if (get_string_oid(OID_PLATFORM_PRINT_NAME_STRING, 0, oid_str, OID_BUF_MAX_LEN))
     {
         char *vendorName=NULL;
         char *ModelName=NULL;
         error_type_t db_retval = FAIL;
         vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);
         ModelName= db_get_var("INST_MACHINE_INFO","modelName",&db_retval);
         remove_spaces(ModelName);
         // if(0==strncmp(vendorName,"Sindoh",strlen("Sindoh")))
         if(0==strcasecmp(vendorName, DEFAULT_VENDOR)) // by mspyo, 2021-03-15
         {
            ipp_add_attr_with_val(grp, "printer-info",
                           IPP_TAG_TEXT_WITHOUT_LANGUAGE, oid_str, strlen(oid_str));
         }
         else
         {
            char tempstr[IPP_TAG_TEXT_WITHOUT_LANGUAGE]="";
            snprintf(tempstr,IPP_TAG_TEXT_WITHOUT_LANGUAGE,"%s %s",vendorName,ModelName);
            ipp_add_attr_with_val(grp, "printer-info",
                                    IPP_TAG_TEXT_WITHOUT_LANGUAGE, tempstr, strlen(tempstr));
         }
     }
    }


    if (has_printer_attr_been_requested(PRINTER_DEVICE_ID))
    {
     if (get_string_oid(OID_PLATFORM_1284_STRING, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "printer-device-id",
                           IPP_TAG_TEXT_WITHOUT_LANGUAGE, oid_str, strlen(oid_str));
     }
    }

// rdj feb-2014 TODO add a local version of this attribute -- currently overridden by airprint-specific value
//    if (has_printer_attr_been_requested(PRINTER_MORE_INFO))
//    {
//     _get_printer_more_info(grp, oid_str, OID_BUF_MAX_LEN, ipp_req);
//    }

    if (has_printer_attr_been_requested(PRINTER_DNS_SD_NAME))
    {
     if (get_string_oid(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "printer-dns-sd-name",
                           IPP_TAG_NAME_WITHOUT_LANGUAGE, oid_str, strlen(oid_str));  
     }
    }


    if (has_printer_attr_been_requested(COMPRESSION_SUPPORTED))
    {
     if (get_string_oid(OID_SM_COMPRESSION_SUPPORTED, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "compression-supported",
                           IPP_TAG_KEYWORD, oid_str, strlen(oid_str));
     }
    }

    // Support this if holds are supported....
    #ifdef HOLDS_SUPPORTED
    /* job-template group */
    if (has_printer_attr_been_requested())
    {
     ipp_add_attr_with_int(grp, "job-priority-default", 100);
    }

    if (has_printer_attr_been_requested())
    {
     ipp_add_attr_with_int(grp, "job-priority-supported", 100);
    }
    #endif


    // job-hold-until-* will be set in this module at this time.
    if (has_printer_attr_been_requested(JOB_HOLD_UNTIL_DEFAULT))
    {
     ipp_add_attr_with_val(grp, "job-hold-until-default",
                           IPP_TAG_KEYWORD, "no-hold", 7);
    }

    if (has_printer_attr_been_requested(JOB_HOLD_UNTIL_SUPPORTED))
    {
     val = ipp_create_attr_val(IPP_TAG_KEYWORD, "no-hold", 7);
     attr = ipp_create_attr("job-hold-until-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
     ipp_free_attr_val(val, 1);
    }

    // job-sheets-* will be set in this module at this time.
    if (has_printer_attr_been_requested(JOB_SHEETS_DEFAULT))
    {
     ipp_add_attr_with_val(grp, "job-sheets-default",
                           IPP_TAG_KEYWORD, "none", 4);
    }

    if (has_printer_attr_been_requested(JOB_SHEETS_SUPPORTED))
    {
     val = ipp_create_attr_val(IPP_TAG_KEYWORD, "none", 4);
     attr = ipp_create_attr("job-sheets-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
     ipp_free_attr_val(val, 1);
    }

    if (has_printer_attr_been_requested(MULTIPLE_DOCUMENT_JOBS_SUPPORTED))
    {
     ipp_add_attr_with_bool(grp, 
                            "multiple-document-jobs-supported", 
                            allow_multiple_docs_per_job);
    }

    if (has_printer_attr_been_requested(MULTIPLE_OPERATION_TIME_OUT))
    {
     ipp_add_attr_with_int(grp, "multiple-operation-time-out", 120);
    }

    // pages-per-minute will be set in this module at this time.
    if (has_printer_attr_been_requested(PAGES_PER_MINUTE))
    {
     ipp_add_attr_with_int(grp, "pages-per-minute", 2);
    }

    if (get_string_oid(OID_SM_PRINT_COLOR_MODE_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
    {
      if (!strcmp(oid_str, "color"))
      {
         // pages-per-minute-color will be set in this module at this time.
         if (has_printer_attr_been_requested(PAGES_PER_MINUTE_COLOR))
         {
            ipp_add_attr_with_int(grp, "pages-per-minute-color", 2);
         }
      }
    }

    if (has_printer_attr_been_requested(COPIES_DEFAULT))
    {
     if (get_uint32_oid(OID_SM_COPIES_DEFAULT, 0, &oid_val))
     {
        ipp_add_attr_with_int(grp, "copies-default", oid_val);
     }
    }

    // We will give it the size of the array now and it will be set to the number of
    // bytes in the array on return.
    if (has_printer_attr_been_requested(COPIES_SUPPORTED))
    {
     oid_byte_array_size = OID_BYTE_ARRAY_SIZE;

     if (get_byte_array_oid(OID_SM_COPIES_SUPPORTED, 
                        0, 
                        &oid_byte_array[0], 
                        &oid_byte_array_size))
     {
        ipp_add_attr_with_val(grp, 
                           "copies-supported",
                           IPP_TAG_RANGE_OF_INTEGER, 
                           &oid_byte_array[0], 
                           oid_byte_array_size);
     }
    }


    // The jpeg-[x,y]-dimension-supported will be set here at this time.
    if (has_printer_attr_been_requested(JPEG_X_DIMENSION_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "jpeg-x-dimension-supported",
                           IPP_TAG_RANGE_OF_INTEGER, "\0\0\0\0\0\0\x60\0", 8);
    }

    if (has_printer_attr_been_requested(JPEG_Y_DIMENSION_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "jpeg-y-dimension-supported",
                           IPP_TAG_RANGE_OF_INTEGER, "\0\0\0\0\0\0\x60\0", 8);
    }

    if (has_printer_attr_been_requested(LANDSCAPE_ORIENTATION_REQUESTED_PREFERRED))
    {
     ipp_add_attr_with_enum(grp, "landscape-orientation-requested-preferred", 4);
    }

    // finishings-* will be set in this module at this time.
    val = ipp_create_attr_val_enum(3); /* is 'none' for finishings */

    if (has_printer_attr_been_requested(FINISHINGS_DEFAULT))
    {
     attr = ipp_create_attr("finishings-default", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    if (has_printer_attr_been_requested(FINISHINGS_SUPPORTED))
    {
     attr = ipp_create_attr("finishings-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    ipp_free_attr_val(val, 1);

    // page-ranges-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(PAGE_RANGES_SUPPORTED))
    {
     ipp_add_attr_with_bool(grp, "page-ranges-supported", 0);
    }


    if (has_printer_attr_been_requested(SIDES_DEFAULT))
    {
     if (get_string_oid(OID_SM_PRINT_SIDES_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "sides-default", IPP_TAG_KEYWORD, oid_str, strlen(oid_str));
     }
    }


    if (has_printer_attr_been_requested(SIDES_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("sides-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_PRINT_SIDES_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    // number-up-* will be set in this module at this time.
    if (has_printer_attr_been_requested(NUMBER_UP_DEFAULT))
    {
     ipp_add_attr_with_int(grp, "number-up-default", 1);
    }

    if (has_printer_attr_been_requested(NUMBER_UP_SUPPORTED))
    {
     val = ipp_create_attr_val_int(1);
     attr = ipp_create_attr("number-up-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
     ipp_free_attr_val(val, 1);
    }


    if (has_printer_attr_been_requested(ORIENTATION_REQUESTED_DEFAULT))
    {
      if (get_uint32_oid(OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT, 0, &oid_val))
      {
         ipp_add_attr_with_enum(grp, "orientation-requested-default", oid_val);
      }
    }

    //EJL - the following was defined but never used or freed
    // val = ipp_create_attr_val_enum(oid_val);

    if (has_printer_attr_been_requested(ORIENTATION_REQUESTED_SUPPORTED))
    {
     attr = _ipp_add_mult_enum_vals_to_attr("orientation-requested-supported",
                                           OID_SM_PRINT_ORIENTATION_REQUESTED_SUPPORTED);

     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    if (has_printer_attr_been_requested(MEDIA_TYPE_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("media-type-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_PRINT_MEDIA_TYPE_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    if (has_printer_attr_been_requested(MEDIA_SOURCE_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("media-source-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_MEDIA_SOURCE_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    // We will give it the size of the array now and it will be set to the number of
    // bytes in the array on return.
    if (has_printer_attr_been_requested(PRINTER_RESOLUTION_DEFAULT))
    {
     oid_byte_array_size = OID_BYTE_ARRAY_SIZE;

     if (get_byte_array_oid(OID_SM_PRINT_RESOLUTION_DEFAULT, 
                        0, 
                        &oid_byte_array[0], 
                        &oid_byte_array_size))
     {                        
         ipp_add_attr_with_val(grp, 
                               "printer-resolution-default",
                               IPP_TAG_RESOLUTION,
                               &oid_byte_array[0], 
                               oid_byte_array_size);
     }
    }


    // Need to reset this value.
    if (has_printer_attr_been_requested(PRINTER_RESOLUTION_SUPPORTED))
    {
     oid_byte_array_size = OID_BYTE_ARRAY_SIZE;
     attr = _ipp_add_mult_byte_arrays_to_attr("printer-resolution-supported",
                                              IPP_TAG_RESOLUTION,
                                              OID_SM_PRINT_RESOLUTION_SUPPORTED,
                                              &oid_byte_array[0],
                                              &oid_byte_array_size);

     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    if (has_printer_attr_been_requested(PRINT_QUALITY_DEFAULT))
    {
     if (get_uint32_oid(OID_SM_PRINT_QUALITY_DEFAULT, 0, &oid_val))
     {
        ipp_add_attr_with_enum(grp, "print-quality-default", oid_val);
     }
    }

    if (has_printer_attr_been_requested(PRINT_QUALITY_SUPPORTED))
    {
     attr = _ipp_add_mult_enum_vals_to_attr("print-quality-supported",
                                           OID_SM_PRINT_QUALITY_SUPPORTED);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(PRINTER_UUID))
    {
     attr = _get_printer_uuid(oid_str, OID_BUF_MAX_LEN);

     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    // *-k-octets-supported will be set in this module at this time.
    if (has_printer_attr_been_requested(JPEG_K_OCTETS_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "jpeg-k-octets-supported",
                           IPP_TAG_RANGE_OF_INTEGER, "\0\0\0\0\0\0\x40\0", 8);
    }

    if (has_printer_attr_been_requested(PDF_K_OCTETS_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "pdf-k-octets-supported",
                           IPP_TAG_RANGE_OF_INTEGER, "\0\0\0\0\0\0\x40\0", 8);
    }

    if (has_printer_attr_been_requested(MEDIA_LEFT_MARGIN_SUPPORTED))
    {
     if (get_uint32_oid(OID_SM_MEDIA_LEFT_MARGIN_SUPPORTED, 0, &oid_val))
     {
        ipp_add_attr_with_int(grp, "media-left-margin-supported", oid_val);
     }
    }

    if (has_printer_attr_been_requested(MEDIA_RIGHT_MARGIN_SUPPORTED))
    {
     if (get_uint32_oid(OID_SM_MEDIA_RIGHT_MARGIN_SUPPORTED, 0, &oid_val))
     {
        ipp_add_attr_with_int(grp, "media-right-margin-supported", oid_val);
     }
    }

    if (has_printer_attr_been_requested(MEDIA_TOP_MARGIN_SUPPORTED))
    {
     if (get_uint32_oid(OID_SM_MEDIA_TOP_MARGIN_SUPPORTED, 0, &oid_val))
     {
        ipp_add_attr_with_int(grp, "media-top-margin-supported", oid_val);
     }
    }

    if (has_printer_attr_been_requested(MEDIA_BOTTOM_MARGIN_SUPPORTED))
    {
     if (get_uint32_oid(OID_SM_MEDIA_BOTTOM_MARGIN_SUPPORTED, 0, &oid_val))
     {
        ipp_add_attr_with_int(grp, "media-bottom-margin-supported", oid_val);
     }
    }


    if (has_printer_attr_been_requested(OUTPUT_BIN_DEFAULT))
    {
     if (get_string_oid(OID_SM_OUTPUT_BIN_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
     {
        ipp_add_attr_with_val(grp, "output-bin-default", 
                           IPP_TAG_KEYWORD, 
                           oid_str, 
                           strlen(oid_str));
     }
    }


    if (has_printer_attr_been_requested(OUTPUT_BIN_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("output-bin-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_OUTPUT_BIN_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }


    if (has_printer_attr_been_requested(MEDIA_SUPPORTED))
    {
     _get_media_supported(grp, oid_str, OID_BUF_MAX_LEN);
    }

    if (has_printer_attr_been_requested(MEDIA_COL_SUPPORTED))
    {
     attr = _ipp_add_mult_str_vals_to_attr("media-col-supported",
                                           IPP_TAG_KEYWORD,
                                           OID_SM_MEDIA_COL_SUPPORTED,
                                           oid_str,
                                           OID_BUF_MAX_LEN);
     ipp_add_attr(grp, attr);
     ipp_free_attr(attr, 1);
    }

    char *default_media = NULL;
    
    // get the ready media name
    if (get_string_oid(OID_SM_PRINT_MEDIA_DEFAULT, 0, oid_str, OID_BUF_MAX_LEN))
    {
        default_media = oid_str;
    }
    
    if (has_printer_attr_been_requested(MEDIA_READY))
    {
     if (default_media)
     {
        ipp_add_attr_with_val(grp, "media-ready",
                           IPP_TAG_KEYWORD, 
                           default_media, 
                           strlen(default_media));
     }
    }

    if (has_printer_attr_been_requested(MEDIA_COL_READY))
    {
     if (default_media)
     {
         _get_media_col(grp,
                        "media-col-ready",
                        default_media,
                        true);
     }
    }

    
    default_media = NULL;
    
    // get the default media name
    if (get_string_oid(OID_SM_PRINT_MEDIA_SUPPORTED, 0, oid_str, OID_BUF_MAX_LEN))
    {
        default_media = oid_str;
    }
    
    if (has_printer_attr_been_requested(MEDIA_DEFAULT))
    {
     if (default_media)
     {
        ipp_add_attr_with_val(grp, "media-default", IPP_TAG_KEYWORD, default_media, strlen(default_media));
     }
    }

    if (has_printer_attr_been_requested(MEDIA_COL_DEFAULT))
    {
      if (default_media)
      {
         _get_media_col(grp, 
                        "media-col-default",
                        default_media,
                        false);
      }                        
    }

    if (has_printer_attr_been_requested(MEDIA_COL_DATABASE))
    {
     _get_media_col(grp,
                    "media-col-database",
                    NULL,
                    false);
    }

    if (has_printer_attr_been_requested(MEDIA_SIZE_SUPPORTED))
    {
     _get_media_size_supported(grp);
    }
    

    if (has_printer_attr_been_requested(PRINTER_ICONS))
    {
     _get_printer_icons(grp, oid_str, OID_BUF_MAX_LEN, ipp_req);
    }

    if (has_printer_attr_been_requested(PRINT_CONTENT_OPTIMIZE_DEFAULT))
    {
     ipp_add_attr_with_val(grp, "print-content-optimize-default",
                           IPP_TAG_KEYWORD, "auto", 4);
    }

    if (has_printer_attr_been_requested(PRINT_CONTENT_OPTIMIZE_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "print-content-optimize-supported",
                           IPP_TAG_KEYWORD, "auto", 4);
    }

    if (has_printer_attr_been_requested(PRINTER_GEO_LOCATION))
    {
     _get_printer_geo_location(grp, oid_str, OID_BUF_MAX_LEN);
    }

    // Start of ePCL additions.

    // epcl-version-supported
    if (has_printer_attr_been_requested(EPCL_VERSION_SUPPORTED))
    {
     ipp_add_attr_with_val(grp, "epcl-version-supported",
                           IPP_TAG_TEXT_WITHOUT_LANGUAGE, "epcl1.0", 7);
    }

    // margins-pre-applied
    if (has_printer_attr_been_requested(MARGINS_PRE_APPLIED))
    {
     ipp_add_attr_with_bool(grp, "margins-pre-applied", 0);
    }

    // pclm-compression-method-preferred
    if (has_printer_attr_been_requested(PCLM_COMPRESSION_METHOD_PREFERRED))
    {
     ipp_add_attr_with_val(grp, "pclm-compression-method-preferred",
                           IPP_TAG_KEYWORD, "rle", 3);
    }

    // pclm-raster-back-side
    if (has_printer_attr_been_requested(PCLM_RASTER_BACK_SIDE))
    {
     ipp_add_attr_with_val(grp, "pclm-raster-back-side",
                           IPP_TAG_KEYWORD, "normal", 6);
    }

    // pclm-source-resolution
    if (has_printer_attr_been_requested(PCLM_SOURCE_RESOLUTION))
    {
     ipp_add_attr_with_val(grp, "pclm-source-resolution",
                           IPP_TAG_RESOLUTION,
                           "\0\0\x02\x58\0\0\x02\x58\x03", 9); /* 600x600 in DPI */
    }

    // pclm-source-resolution-supported
    if (has_printer_attr_been_requested(PCLM_SOURCE_RESOLUTION_SUPPORTED))
    {
     val = ipp_create_attr_val(IPP_TAG_RESOLUTION,
                               "\0\0\x02\x58\0\0\x02\x58\x03", 9);
     attr = ipp_create_attr("pclm-source-resolution-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);

     ipp_free_attr(attr, 1);
     ipp_free_attr_val(val, 1);
    }

    // pclm-strip-height-preferred
    uint32_t strip_height = 128;

    if (has_printer_attr_been_requested(PCLM_STRIP_HEIGHT_PREFERRED))
    {
     ipp_add_attr_with_int(grp, "pclm-strip-height-preferred", strip_height);
    }

    // pclm-strip_height-supported
    if (has_printer_attr_been_requested(PCLM_STRIP_HEIGHT_SUPPORTED))
    {
     val = ipp_create_attr_val_int(strip_height);

     attr = ipp_create_attr("pclm-strip_height-supported", 0);
     ipp_add_attr_val(attr, val);
     ipp_add_attr(grp, attr);

     ipp_free_attr(attr, 1);
     ipp_free_attr_val(val, 1);
    }

  return grp;
}

void ipp_create_response_attributes(ipp_request_t *ipp_req)
{
    ipp_cntxt_t *cntxt = ipp_req->ipp_ctxt;
    ASSERT(cntxt);

    ipp_attr_grp_t     *grp;
  
    ASSERT(ipp_req);
    
    // if response attriubtes already exists, free them
    if (cntxt->respattr)
    {
        /* Free the response attribute groups */
        ipp_free_attr_groups(cntxt->respattr, 1);
    }
    cntxt->respattr = ipp_create_attr_groups();
    IPP_TIME(ipp_req, "ipp_create_attr_groups");

    /* Group 1 - Operation Attributes */
    grp = _default_operation_attributes();
    ipp_add_attr_group(cntxt->respattr, grp);
    ipp_free_attr_group(grp, 1);

        /* Group 2 - Unsupported Attributes (can be skipped) */
        

    /* Group 3 - Job Object Attributes */
    ipp_requested_attrs_t *requested_attributes;

    // See what attributes are being requested.
    requested_attributes = _get_the_req_attributes(ipp_req);

    // rdj feb-2014
    if(!requested_attributes)
    {
        /* RFC 2911 3.2.5.1 - If the client omits 'requested-attributes' then the Printer MUST
         * respond as if this attribute had been supplied with a value of 'all'
         */
        ipp_req->req_attrs.grp_mask |= IPP_GRP_MASK; 
    }

    switch (ipp_req->ipp_request_op)
    {
        case IPP_OPID_CUPS_GET_PRINTERS:
        case IPP_OPID_GET_PRINTER_ATTR:
        {
            if (ipp_req->ipp_status == IPP_STAT_OK)
            {
                // Set which job attributes we are going to respond to. 
                set_printer_attributes_requested(requested_attributes);

                /* Group 3 - Job Object Attributes */
                grp = _get_printer_attributes(cntxt);
                ipp_add_attr_group(cntxt->respattr, grp);
                ipp_free_attr_group(grp, 1);
            }
        }
        break;
        
        case IPP_OPID_GET_JOB_ATTR:
        case IPP_OPID_PRINT_JOB:
        case IPP_OPID_CREATE_JOB:
        case IPP_OPID_SEND_DOCUMENT:
        {
            // Set which job attributes we are going to respond to. 
            set_job_attributes_requested(requested_attributes, false);

            /* Group 3 - Job Object Attributes */
            grp = _printer_job_attributes(cntxt, ipp_req->job_id, ipp_req->ipp_status);
            ipp_add_attr_group(cntxt->respattr, grp);
            ipp_free_attr_group(grp, 1);
        }
        break;
        
        case IPP_OPID_GET_JOBS:
        {
            ipp_rcode_t           ipp_rcode;
            
            if (ipp_req->ipp_status == IPP_STAT_OK)
            {
                // Set which job attributes we are going to respond to. 
                set_job_attributes_requested(requested_attributes, true);
                
                /*** Attribute "which-jobs" ***/
                /* 
                 The client OPTIONALLY supplies this attribute.  The Printer
                 object MUST support this attribute.  It indicates which Job
                 objects MUST be returned by the Printer object. The values for
                 this attribute are:

                'completed': This includes any Job object whose state is
                    'completed', 'canceled', or 'aborted'.
                'not-completed': This includes any Job object whose state is
                    'pending', 'processing', 'processing-stopped', or 'pending-
                    held'.

                 A Printer object MUST support both values.  However, if the
                 implementation does not keep jobs in the 'completed',
                 'canceled', and 'aborted' states, then it returns no jobs when
                 the 'completed' value is supplied.

                 If a client supplies some other value, the Printer object MUST
                 copy the attribute and the unsupported value to the Unsupported
                 Attributes response group, reject the request, and return the
                 'client-error-attributes-or-values-not-supported' status code.

                 If the client does not supply this attribute, the Printer
                 object MUST respond as if the client had supplied the attribute
                 with a value of 'not-completed'.   
                */
                char attr_string[SMJOB_MAX_NAME_LEN];
                bool get_completed = false;
                memset(attr_string, 0, SMJOB_MAX_NAME_LEN);
                ipp_rcode = _extract_string(cntxt->reqattr,
                                  "which-jobs", 
                                  0,
                                  attr_string, 
                                  SMJOB_MAX_NAME_LEN-1);
                if (ipp_rcode == IPP_OK)
                {
                   /* If provided but does not match one of the below then an error will be reported */
                   if (strcmp( attr_string, "completed" ) == 0)
                   {
                    get_completed = true;
                   }
                   else if (strcmp( attr_string, "not-completed" ) == 0)
                   {
                    get_completed = false;
                   }
                   else
                   {
                    ASSERT(0); // should have been validated before now
                   }
                }
                else if (ipp_rcode == IPP_ATTR_NOT_FOUND)
                {
                    get_completed = false;
                }
                else if (ipp_rcode == IPP_INVALID_LENGTH)
                {
                    ASSERT(0); // should have been validated before now
                }
                else
                {
                    ASSERT(0); // should have been validated before now
                }  


                /*** Attribute "my-jobs" ***/
                /* 
                IF NEITHER a single 'true' NOR a single 'false' 'boolean' value,
                REJECT/RETURN 'client-error-bad-request'.

                IF the value length is NOT equal to 1 octet, REJECT/RETURN
                'client-error-request-value-too-long'

                IF NOT supplied by the client, the IPP object assumes the 'false'
                value.
                */
                bool my_jobs = false;
                ipp_rcode = _extract_boolean(cntxt->reqattr, "my-jobs", 0, &my_jobs);
                if (ipp_rcode == IPP_ATTR_NOT_FOUND)
                {
                    my_jobs = false;
                }
                else if (ipp_rcode == IPP_INVALID_LENGTH)
                {
                    ASSERT(0); // should have been validated before now
                }
                else if (ipp_rcode != IPP_OK)
                {
                    ASSERT(0); // should have been validated before now
                }

                /*** Attribute "requesting-user-name" ***/
                /* 
                The "requesting-user-name" (name(MAX)) attribute SHOULD be
                supplied by the client as described in section 8.3.

                Section 8.3:
                When the authentication mechanism is 'none', all authenticated users are
                "anonymous".

                Section 4.4.2:
                If the "requesting-user-name" attribute is absent in a request, the
                Printer object assumes that the authenticated user is "anonymous".
                */
                char requesting_user_name[SMJOB_MAX_NAME_LEN];
                memset(requesting_user_name, 0, SMJOB_MAX_NAME_LEN);
                ipp_rcode = _extract_string(cntxt->reqattr,
                                  "requesting-user-name", 
                                  0,
                                  requesting_user_name, 
                                  SMJOB_MAX_NAME_LEN-1);
                if (ipp_rcode == IPP_ATTR_NOT_FOUND)
                {
                    strncpy(requesting_user_name, "anonymous", SMJOB_MAX_NAME_LEN);
                }
                else if (ipp_rcode == IPP_INVALID_LENGTH)
                {
                    ASSERT(0); // should have been validated before now
                }
                else if (ipp_rcode != IPP_OK)
                {
                    ASSERT(0); // should have been validated before now
                }

                /*** Attribute "limit" ***/
                /* 
                The client OPTIONALLY supplies this attribute.  The Printer
                object MUST support this attribute. It is an integer value that
                determines the maximum number of jobs that a client will
                receive from the Printer even if "which-jobs" or "my-jobs"
                constrain which jobs are returned.  The limit is a "stateless
                limit" in that if the value supplied by the client is 'N', then
                only the first 'N' jobs are returned in the Get-Jobs Response.
                There is no mechanism to allow for the next 'M' jobs after the
                first 'N' jobs.  If the client does not supply this attribute,
                the Printer object responds with all applicable jobs.
                */
                uint32_t limitN = 0;
                ipp_rcode = _extract_integer(cntxt->reqattr, "limit", 0, &limitN);
                if (ipp_rcode == IPP_ATTR_NOT_FOUND)
                {
                    limitN = 0;
                }
                else if (ipp_rcode != IPP_OK)
                {
                    ASSERT(0); // should have been validated before now
                }
                
                smjob_rcode_t smjob_rcode;
                uint32_t job_id = SMJOB_INVALID_JOB_ID;
                smjob_status_t job_status;
                smjob_ticket_t job_ticket;
                job_id = smjob_get_next(job_id);

                uint32_t valid_added = 0;

                while(job_id != SMJOB_INVALID_JOB_ID)
                {
                    bool valid = false;

                    smjob_rcode = smjob_get_status(job_id, &job_status);
                    if(smjob_rcode != SMJOB_OK)
                    {
                        job_id = smjob_get_next(job_id);
                        continue;
                    }

                    smjob_rcode = smjob_get_ticket(job_id, &job_ticket);
                    if(smjob_rcode != SMJOB_OK)
                    {
                        job_id = smjob_get_next(job_id);
                        continue;
                    }

                    switch(job_status.state)
                    {
                        case SMJOB_STATE_CANCELED:
                        case SMJOB_STATE_ABORTED:
                        case SMJOB_STATE_COMPLETED:
                        {
                            if(get_completed)
                            {
                                valid = true;
                            }
                            break;
                        }

                        default:
                        {
                            if(!get_completed)
                            {
                                valid = true;
                            }
                            break;
                        }
                    }

                    if (valid)
                    {
                        if (my_jobs)
                        {
                            valid = false;
                            
                            if (strcmp( requesting_user_name, job_ticket.job_description.originating_user_name ) == 0)
                            {
                                valid = true;
                            }
                        }
                    }

                    if (valid)
                    {
                        if (limitN && (valid_added >= limitN))
                        {
                            valid = false;
                        }
                    }

                    if(valid)
                    {
                        valid_added++;
                        grp = _printer_job_attributes(cntxt, job_id, ipp_req->ipp_status);
                        ipp_add_attr_group(cntxt->respattr, grp);
                        ipp_free_attr_group(grp, 1);
                    }

                    job_id = smjob_get_next(job_id);
                }
            }
        }
        break;
        
        default:
        // No Group 3 Attributes
        break;
    }

    // Free attributes when done.
    _free_the_req_attributes(requested_attributes);
}

int ipp_build_response(ipp_request_t *ipp_req)
{
  ipp_cntxt_t *cntxt = ipp_req->ipp_ctxt;
  ASSERT(cntxt);

  unsigned char* data;
  
  ASSERT(ipp_req);
  
  ipp_create_response_attributes(ipp_req);
  
  if (ipp_req->response_data) 
  {
    MEM_FREE_AND_NULL(ipp_req->response_data);
    ipp_req->response_data = NULL;
    ipp_req->response_len = 0;
  }

  int len = ipp_encoded_attr_groups_len(cntxt->respattr);
  if (len < 0) return -2;
  len += 2 + 2 + 4; /* Leave space for header */
  
  // we don't track this allocation (IPP_MALLOC) because it is typicall freed by HTTP
  data = (unsigned char*)MEM_MALLOC(len); 
  if (!data) 
  {
     ASSERT(0);
     return -3;
  }
  ipp_req->response_len = len;
  len -= ipp_encode_attr_groups(cntxt->respattr, data+8, len-8);
  if (len != 8)
  {
    ipp_req->response_len = len;
    MEM_FREE_AND_NULL(data);
    return -4;
  }
  
  data[1] = ipp_req->ipp_version&0xff;
  data[0] = ipp_req->ipp_version>>8;

  data[3] = ipp_req->ipp_status&0xff;
  data[2] = ipp_req->ipp_status>>8;

  data[7] = ipp_req->ipp_request_id&0xff; ipp_req->ipp_request_id >>= 8;
  data[6] = ipp_req->ipp_request_id&0xff; ipp_req->ipp_request_id >>= 8;
  data[5] = ipp_req->ipp_request_id&0xff; ipp_req->ipp_request_id >>= 8;
  data[4] = ipp_req->ipp_request_id&0xff;
  
  ipp_req->response_data = data;
  
  _dump_ipp(ipp_req->response_data, ipp_req->response_len, 0);
  
  return 0;
}
