/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef GCPP_HELPER_H
#define GCPP_HELPER_H

#include "gcpp.h"

#ifdef __cplusplus
extern "C" {
#endif

// Strings for GCP Parameters
#define GCPP_PARAM_PRINTERID_STR            "printerid"
#define GCPP_PARAM_PRINTER_NAME_STR            "printer"
#define GCPP_PARAM_DISPLAY_NAME_STR            "display_name"
#define GCPP_PARAM_PROXY_STR                "proxy"
#define GCPP_PARAM_CAPABILITIES_STR            "capabilities"
#define GCPP_PARAM_DEFAULTS_STR                "defaults"
#define GCPP_PARAM_TAGS_STR                    "tag"
#define GCPP_PARAM_STATUS_STR                "status"
#define GCPP_PARAM_STATUS_DESCRIPTION_STR     "description"
#define GCPP_PARAM_CAPABILITES_HASH_STR        "capsHash"


typedef size_t GCPP_CURL_FUNCTION_CALLBACK(void *ptr, size_t size, size_t nmemb, void *data);


struct gcppDataStruct {
  char *data;
  size_t size;
};

struct gcppJobRecvStruct {
  gcpp_recv_job_header_t recv_header_func;
  gcpp_recv_job_data_t recv_data_func;
  void *context;
};


char* gcpp_strdup(char* string);

gcpp_error_t gcpp_initiate_anonymous_registration(net_gcpp_context_t *gcpp_context, gcpp_registration_anonymous_context_t *anon_reg_context, void *callback_context);
gcpp_error_t gcpp_claim_printer(net_gcpp_context_t *gcpp_context, char *polling_url, gcpp_registration_claim_printer_context_t *reg_completion_context, void *callback_context);
gcpp_error_t gcpp_get_authorization_tokens(net_gcpp_context_t *gcpp_context, char *auth_code, gcpp_authorization_tokens_context_t *auth_tokens_context, void *callback_context);
gcpp_error_t gcpp_update_authorization_tokens(net_gcpp_context_t *gcpp_context, char *refesh_token, gcpp_authorization_tokens_context_t *access_token_context, void *callback_context);

gcpp_error_t gcpp_update_xsrf_token(net_gcpp_context_t *gcpp_context);

gcpp_error_t gcpp_add_printer(net_gcpp_context_t *gcpp_context, char **printer_id);
gcpp_error_t gcpp_update_printer(net_gcpp_context_t *gcpp_context, char *printer_id);
gcpp_error_t gcpp_remove_printer(net_gcpp_context_t *gcpp_context, char *printer_id);
gcpp_error_t gcpp_remove_printers(net_gcpp_context_t *gcpp_context);

gcpp_error_t gcpp_job_update_status(net_gcpp_context_t *gcpp_context, char *job_id, char *status, char *code, char *message);

gcpp_error_t gcpp_get_printer_id(net_gcpp_context_t *gcpp_context, char **printer_id);
gcpp_error_t gcpp_printer_get_jobs(net_gcpp_context_t *gcpp_context, char *printer_id, gcpp_job_t ***jobs);
gcpp_job_t *gcpp_printer_new_job();
gcpp_job_t *gcpp_printer_copy_job(gcpp_job_t *source);
void gcpp_printer_delete_job(gcpp_job_t *job);
void gcpp_printer_delete_jobs(gcpp_job_t **job);

gcpp_error_t gcpp_printer_get_job_ticket(net_gcpp_context_t *gcpp_context, char *job_ticket_url, gcpp_recv_job_header_t recv_job_header_func, char **job_ticket_data, uint32_t *job_ticket_size, void *context);
gcpp_error_t gcpp_printer_get_job(net_gcpp_context_t *gcpp_context, char *job_url, uint32_t skip_pages, char *mime_types, gcpp_recv_job_header_t recv_job_header_func, gcpp_recv_job_data_t recv_job_data_func, void *context);

#ifdef DEBUG
void gcpp_hex_dump( char * buffer, uint32_t length );
#endif

#ifdef __cplusplus
}
#endif

#endif // GCPP_HELPER_H
