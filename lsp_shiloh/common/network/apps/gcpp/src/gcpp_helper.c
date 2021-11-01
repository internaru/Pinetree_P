 /*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// public includes
#include "dprintf.h"
#include "lassert.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "platform_api.h"
#include "json_api.h"

#include "os_network.h"
#include "net_iface_api.h"
//#include "net_app_api.h"
#include "net_iface_vars.h"
#include "net_gcpp_vars.h"

// private includes
#include "gcpp.h"
#include "gcpp_helper.h"

#ifdef DEBUG
#include "debug.h"
#endif // DEBUG

#define GCPP_PRINT_DATA_HEADER            GCPP_ACCEPT_HEADER "\r\n" GCPP_PROXY_HEADER


char* gcpp_strndup(char* string, int length)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
	
	if (string == NULL)
		return NULL;

	if (length == 0)
		return NULL;

	char *newStr = (char*)MEM_MALLOC(length+1);
	if (newStr)
	{
        memset(newStr, 0, length+1);
		strncpy(newStr, string, length);
	}

	DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
	return newStr;
}

char* gcpp_strdup(char* string)
{
    if (string == NULL) 
      return NULL;

    return gcpp_strndup(string, strlen(string));
}

gcpp_error_t gcpp_map_response_code(long code)
{
    gcpp_error_t ret = NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE;
    
    switch(code)
    {
        case 200:
            ret = NET_GCPP_SUCCESS;
            break;
            
        case 400:
            ret = NET_GCPP_HTTP_BAD_REQUEST;
            break;
            
        case 403:
            ret = NET_GCPP_HTTP_FORBIDDEN;
            break;
            
        case 404:
            ret = NET_GCPP_HTTP_FILE_NOT_FOUND;
            break;
            
        case 415:
        case 502:
            ret = NET_GCPP_HTTP_FILE_CONVERSION_FAILED;
            break;
            
        default:
            ret = NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE;
            break;
    }
    
    return ret;
}

#ifdef DEBUG
#define GCPP_HEX_DUMP_CHAR_WIDTH 32
void gcpp_hex_dump( char * buffer, uint32_t length )
{
    uint32_t i;
    uint32_t j;
    uint32_t temp_length;
    ASSERT( NULL != buffer );
        
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    for ( i = 0; i < length; i++ )
    {
        if ( ( i != 0 ) && ( ( i % GCPP_HEX_DUMP_CHAR_WIDTH ) == 0 ) )
        {
            for ( j = ( i - GCPP_HEX_DUMP_CHAR_WIDTH ); j < i ; j++ )
            {
                if ( ( buffer[ j ] > 31 ) && ( buffer[ j ] < 127 ) )
                {
                    DBG_DUMP( "%c", (char)( buffer[ j ] ) );
                }
                else
                {
                    DBG_DUMP( ".");
                }
            }

            DBG_DUMP( "\n" );
        }
        if ( ( i % GCPP_HEX_DUMP_CHAR_WIDTH ) == 0 )
        {
            DBG_DUMP( "BYTE %05d: ", i );
        }

        DBG_DUMP( "%02x ", buffer[ i ] );
    }

    if ( ( length % GCPP_HEX_DUMP_CHAR_WIDTH ) != 0 )
    {
        /* print last nibble */
        for ( j = 0; j < ( GCPP_HEX_DUMP_CHAR_WIDTH - ( length % GCPP_HEX_DUMP_CHAR_WIDTH ) ); j++ ) 
        {
            DBG_DUMP( "   " );
        }
    }

    temp_length = ( length - ( length % GCPP_HEX_DUMP_CHAR_WIDTH ) );

    if ( ( length % GCPP_HEX_DUMP_CHAR_WIDTH ) == 0 )
    {
        temp_length = length - GCPP_HEX_DUMP_CHAR_WIDTH;
    }

    for ( j = temp_length; j < length ; j++ )
    {
        if ( ( buffer[ j ] > 31 ) && ( buffer[ j ] < 127 ) )
        {
            DBG_DUMP( "%c", (char)( buffer[ j ] ) );
        }
        else
        {
            DBG_DUMP( ".");
        }
    }

    DBG_DUMP( "\n" );
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}
#endif

static size_t gcpp_fetch_data_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
    struct gcppDataStruct *jobData = (struct gcppDataStruct *)data;
    size_t count = (size * nmemb);

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (ptr == NULL)
        return 0;
        
    if (jobData == NULL)
        return 0;

    DBG_VERBOSE("gcpp_fetch_data_callback (%d) bytes\n", count);

    if (jobData->data)
    {
        jobData->data = MEM_REALLOC(jobData->data, jobData->size + count + 1);
    }
    else
    {
        jobData->data = MEM_MALLOC(count + 1);
    }
    
    if (jobData->data)
    {
        memcpy(&(jobData->data[jobData->size]), ptr, count);
        jobData->size += count;
        jobData->data[jobData->size] = 0;
    }
    else
    {
        DBG_ERR("gcpp_fetch_data_callback - unable to allocate memory for data buffer!\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return count;
}


static size_t gcpp_job_data_callback(void *ptr, size_t size, size_t nmemb, void *context)
{
    struct gcppJobRecvStruct *jobRecv = (struct gcppJobRecvStruct *)context;
    size_t count = (size * nmemb);
    size_t ret = 0;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    ASSERT(jobRecv);

    if (ptr == NULL)
        return 0;

    DBG_VERBOSE("gcpp_job_data_callback (%d) bytes\n", count);

    if (jobRecv->recv_data_func)
    {
        ret = (size_t)jobRecv->recv_data_func(ptr, count, jobRecv->context);
    }
    else
    {
        DBG_MSG("gcpp_job_data_callback: recv_data_func is NULL\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

static size_t gcpp_job_header_callback(void *ptr, size_t size, size_t nmemb, void *context)
{
    struct gcppJobRecvStruct *jobRecv = (struct gcppJobRecvStruct *)context;
    size_t count = (size * nmemb);
    size_t ret = 0;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    ASSERT(jobRecv);

    if (ptr == NULL)
        return 0;

    DBG_VERBOSE("gcpp_job_header_callback (%d) bytes\n", count);

    if (jobRecv->recv_header_func)
    {
        ret = (size_t)jobRecv->recv_header_func(ptr, count, jobRecv->context);
    }
    else
    {
        DBG_MSG("gcpp_job_header_callback: recv_header_func is NULL\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_parse_printer_id(net_gcpp_context_t *gcpp_context, void *ptr, size_t size, char **printer_id)
{
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (ptr == NULL)
    {
        DBG_ERR("ptr cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    DBG_VERBOSE("gcpp_parse_printer_id (%d) bytes:\n", size);

    JSON *json = json_parseInput(ptr);
    if (json)
    {
        JSON *printers = json_getObject(json, "printers");
        if (printers && json_getArrayCount(printers) > 0)
        {
            int i, count;
            
            count = json_getArrayCount(printers);
            DBG_VERBOSE("Number of printers found = %d\n", count);
            
            // Note: In order to support certain third party hardware and virtual printer solutions, 
            // the Google Cloud Proxy service allows a printer proxy to register more than one 
            // physical printer. In those circumstances, the printer list may be more than one item 
            // long. On the other hand, a Cloud Ready printer acting on its own behalf should always 
            // use a unique proxy ID, and therefore we always return the first (and only) printer id
            // associated to the proxy ID.
            for (i=0;i<count;i++)
            {
                JSON *printer = json_getArray(printers, i);
                if (printer)
                {
                    error_type_t e_res = json_getObjectString(printer, "id", printer_id);
                    if (e_res == OK && *printer_id)
                    {
                        DBG_MSG("Identified printer id = %s\n", *printer_id);
                        ret = NET_GCPP_SUCCESS;
                        break;
                    }
                }
            }
        }
        else
        {
            DBG_VERBOSE("No printers found!\n");
        }
    }
    else
    {
        DBG_ERR("\nJSON Parse Error:\n%s\n", ptr);
        
        /* indicate fatal error */
        ret = NET_GCPP_FATAL_ERROR;
    }

    json_free(json);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_parse_jobs(net_gcpp_context_t *gcpp_context, void *ptr, size_t size, gcpp_job_t ***jobs)
{
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (ptr == NULL)
    {
        DBG_ERR("ptr cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (jobs == NULL)
    {
        DBG_ERR("jobs cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    DBG_VERBOSE("gcpp_parse_jobs (%d) bytes:\n", size);

    JSON *json = json_parseInput(ptr);
    if (json)
    {
        ret = NET_GCPP_SUCCESS; 
        *jobs = NULL;
        
        JSON *json_request = json_getObject(json, "request");
        JSON *json_jobs = json_getObject(json, "jobs");
        
        if (json_jobs && json_getArrayCount(json_jobs) > 0)
        {
            int index = 0;
            int count = json_getArrayCount(json_jobs);
            
            DBG_VERBOSE("Number of jobs found = %d\n", count);
            
            int jobs_size = ( (count+1) * sizeof(gcpp_job_t*) );
            *jobs = (gcpp_job_t **)MEM_MALLOC( jobs_size );
            
            if (*jobs == NULL)
            {
                DBG_ERR("Low memory error!\n", ptr);
                json_free(json);
                return NET_GCPP_LOW_MEM;
            }
                
            memset(*jobs, 0, jobs_size);
            
            while (index < count)
            {
                JSON *json_job = json_getArray(json_jobs, index);
                if (json_job)
                {
                    gcpp_job_t *gcpp_job = gcpp_printer_new_job();
                    if (gcpp_job)
                    {
                        json_getObjectString(json_job, "id", &gcpp_job->id);
                        json_getObjectString(json_job, "printer_id", &gcpp_job->printer_id);
                        json_getObjectString(json_job, "title", &gcpp_job->title);
                        json_getObjectString(json_job, "fileUrl", &gcpp_job->file_url);
                        json_getObjectString(json_job, "ticketUrl", &gcpp_job->ticket_url);
                        gcpp_job->tags = NULL;

                        JSON *json_tags = json_getObject(json_job, "tags");
                        if (json_tags && json_getArrayCount(json_tags) > 0)
                        {
                            int ind = 0;
                            int cnt = json_getArrayCount(json_tags);
                            DBG_MSG ("Number of Tags = %d\n", cnt);
                            gcpp_job->tags = (char**)MEM_MALLOC (sizeof(char*) * (cnt + 1));
                            while (ind < cnt)
                            {
                                json_getArrayString(json_tags, ind, &gcpp_job->tags[ind]);
                                DBG_MSG ("Tag [%d] = %s\n", ind, gcpp_job->tags[ind]);
                                ind++;
                            }         
                            gcpp_job->tags [ind] = NULL;  
                        }
                        
                        if (json_request)
                        {
                            json_getObjectString(json_request, "user", &gcpp_job->user);
                        }
        
                        DBG_VERBOSE("Adding job, id = %s\n", gcpp_job->id);
                        (*jobs)[index++] = gcpp_job;
                    }
                }
            }
        }
    }
    else
    {
        DBG_ERR("\nJSON Parse Error:\n%s\n", ptr);
        ret = NET_GCPP_FATAL_ERROR;
    }

    json_free(json);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_parse_for_success(net_gcpp_context_t *gcpp_context, gcpp_event_context_header_t *event_hdr, void *ptr, size_t size)
{
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (ptr == NULL)
    {
        DBG_ERR("ptr cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    DBG_VERBOSE("gcpp_parse_for_success (%d) bytes:\n", size);

    JSON *json = json_parseInput(ptr);
    if (json)
    {
        JSON *successObjFound = json_getObject(json, "success");
        if (successObjFound)
        {
            bool success;
            uint32_t error_code;

            json_getObjectBool(json, "success", &success);
            error_code = json_getObjectNumber(json, "errorCode", &error_code);
            
            ret = NET_GCPP_SUCCESS;
            
            if (event_hdr != NULL)
            {
                event_hdr->success = success;
                event_hdr->error_code = (long)error_code;
            }
                
#ifdef DEBUG
            if (!success)
            {
                char *message;
                error_type_t e_res = json_getObjectString(json, "message", &message);
                if (e_res == OK && message)
                {
                    DBG_ERR("Error:  %s\n", message);
                    MEM_FREE_AND_NULL(message);
                }
                else
                {
                    DBG_ERR("Request Failed!\n");
                }
            }
#endif // DEBUG
            
            char *xsrf_token;
            error_type_t e_res = json_getObjectString(json, "xsrf_token", &xsrf_token);
            if (e_res == OK && xsrf_token)
            {
                if (gcpp_context->xsrf_token)
                {
                    MEM_FREE_AND_NULL(gcpp_context->xsrf_token);
                }
                
                gcpp_context->xsrf_token = xsrf_token;
                DBG_VERBOSE("Updated XSRF token.\n");
    
                ASSERT(gcpp_context->oem_func_table);
                if(gcpp_context->oem_func_table->event_notification)
                {
                    gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_XSRF_TOKEN_UPDATED, xsrf_token, NULL);
                    if(rcode != NET_GCPP_SUCCESS)
                    {
                        XASSERT(0, rcode);
                    }
                }
            }
        }
        else
        {
            DBG_ERR("\nJSON Parse Error: No success token\n");
        }
    }
    else
    {
        DBG_ERR("\nJSON Parse Error:\n%s\n", ptr);
        
        /* indicate fatal error */
        ret = NET_GCPP_FATAL_ERROR;
    }

    json_free(json);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}


gcpp_error_t gcpp_curl_standard_request(
                             net_gcpp_context_t *gcpp_context,
                             gcpp_event_context_header_t *event_hdr,
                             char *url, 
                             void *headerFunction, 
                             void *headerFunction_data, 
                             void *writeFunction, 
                             void *writeFunction_data, 
                             struct curl_httppost *post, 
                             char *postFields,
                             struct curl_slist *headers
                            )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    CURL *curl;
    CURLcode res;    
    long info=0;
    bool http_get = true;
	
   
    if (gcpp_context == NULL)
    {
        DBG_ERR("ERROR: gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (post != NULL)
    {
	    http_get = false;
    }

    if (event_hdr != NULL)
    {
        event_hdr->result = 0;
        event_hdr->response_code = 0;
        event_hdr->success = false;
        event_hdr->error_code = 0;
    }
    
    curl = curl_easy_init();
    if (curl == NULL)
    {
        DBG_ERR("Failed to init CURL!\n" );
        return NET_GCPP_FATAL_ERROR;
    }
    
    res = curl_easy_setopt(curl, CURLOPT_HTTPGET, (long)http_get);
    res = curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    
    if (url != NULL)
    {
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_setopt(curl, CURLOPT_USE_SSL, 1);
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    
    if (writeFunction)
    {
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        if (writeFunction_data)
        {
            res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, writeFunction_data);
        }
    }
    else
    {
        res = curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }
    
    if (headerFunction)
    {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerFunction);
        if (headerFunction_data)
        {
            res = curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerFunction_data);
        }
    }
    
    if (gcpp_context->http_proxy_enabled)
    {
        res = curl_easy_setopt(curl, CURLOPT_PROXY, gcpp_context->http_proxy_server);
        res = curl_easy_setopt(curl, CURLOPT_PROXYPORT, gcpp_context->http_proxy_port);
        
        if (gcpp_context->http_proxy_auth_req)
        {
            res = curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
            res = curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, gcpp_context->http_proxy_username);
            res = curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, gcpp_context->http_proxy_password);
        }
    }

    if (post != NULL)
    {
        res = curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    }
    
    if (postFields != NULL)
    {
        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);
    }
    
    if (headers != NULL)
    {
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
#ifdef DEBUG    
    /* enable verbose for easier tracing */
    res = curl_easy_setopt(curl, CURLOPT_VERBOSE, gcpp_context->debug_curl_level);
#endif

    res = curl_easy_perform(curl);
    if (res == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info);
        
        ret = gcpp_map_response_code(info);
        if (ret == NET_GCPP_SUCCESS)
        {
            // success
        }
        else
        {
            DBG_ERR("Response code check failed, response code = %03ld!\n", info);
        }
    }
    else
    {
        DBG_ERR("Call to curl_easy_perform failed, res = %d!\n", res);
        ret = NET_GCPP_FATAL_ERROR;
    }

    
    if (event_hdr != NULL)
    {
        event_hdr->result = ret;
        event_hdr->response_code = info;
    }
    
    /* cleanup */
    curl_easy_cleanup(curl);
    
    return ret;
}

gcpp_error_t gcpp_curl_cloudprint_request(
                             net_gcpp_context_t *gcpp_context,
                             gcpp_event_context_header_t *event_hdr,
                             char *url, 
                             void *writeFunction, 
                             struct gcppDataStruct *jobData, 
                             struct curl_httppost *post, 
                             char *postFields
                            )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    struct curl_slist *headers = NULL;
    bool http_get = true;
	
    if (gcpp_context == NULL)
    {
        DBG_ERR("ERROR: gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (post != NULL)
    {
	    http_get = false;
    }
    
    char *httpProxyHeader = MEM_MALLOC(strlen(GCPP_PROXY_HEADER)+strlen(gcpp_context->proxy));
    if (httpProxyHeader)
    {
        sprintf(httpProxyHeader, GCPP_PROXY_HEADER, gcpp_context->proxy);
        headers = curl_slist_append(headers, httpProxyHeader);
        MEM_FREE_AND_NULL(httpProxyHeader);
    }
    
    ret = gcpp_curl_standard_request (gcpp_context, event_hdr, url, NULL, NULL, writeFunction, jobData, post, postFields, headers);
    if (ret == NET_GCPP_SUCCESS)
    {
        if (jobData != NULL)
        {
#ifdef DEBUG
            if (gcpp_context->debug_on && jobData->size > 0)
            {
                DBG_VERBOSE("\nData Dump:\n");
                gcpp_hex_dump(jobData->data, jobData->size);
            }
#endif
            ret = gcpp_parse_for_success(gcpp_context, event_hdr, jobData->data, jobData->size);
        }
    }
    
    /* cleanup */
    if (headers)
        curl_slist_free_all(headers);    
    
    return ret;
}

gcpp_error_t gcpp_initiate_anonymous_registration(net_gcpp_context_t *gcpp_context, gcpp_registration_anonymous_context_t *anon_reg_context, void *callback_context)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;
    char *capabilities = NULL;
    char *defaults = NULL;
    char *tags = NULL;
    char *status = NULL;
    char *description = NULL;
    char *capshash = NULL;
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;

    if (gcpp_context->printer_name)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PRINTER_NAME_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->printer_name, 
                   CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);
    }
    
    if (gcpp_context->proxy)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PROXY_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->proxy,
                   CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capabilities */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capabilities)
    {
        capabilities = gcpp_context->oem_func_table->get_printer_capabilities();
    }
    if (capabilities != NULL)
    {
        int capabilities_size = (capabilities == NULL) ? 0 : strlen(capabilities);
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nCapabilities Dump:\n");
            gcpp_hex_dump(capabilities, capabilities_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITIES_STR,
                   CURLFORM_PTRCONTENTS, capabilities,
                   CURLFORM_CONTENTSLENGTH, capabilities_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_defaults */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_defaults)
    {
        defaults = gcpp_context->oem_func_table->get_printer_defaults();
    }
    if (defaults != NULL)
    {
        int defaults_size = (defaults == NULL) ? 0 : strlen(defaults);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(defaults, defaults_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_DEFAULTS_STR,
                   CURLFORM_PTRCONTENTS, defaults,
                   CURLFORM_CONTENTSLENGTH, defaults_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_tags */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_tags)
    {
        tags = gcpp_context->oem_func_table->get_printer_tags();
    }
    if (tags != NULL)
    {
        int tags_size = (tags == NULL) ? 0 : strlen(tags);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(tags, tags_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_TAGS_STR,
                   CURLFORM_PTRCONTENTS, tags,
                   CURLFORM_CONTENTSLENGTH, tags_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_status */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_status)
    {
        status = gcpp_context->oem_func_table->get_printer_status();
    }
    if (status != NULL)
    {
        int status_size = (status == NULL) ? 0 : strlen(status);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(status, status_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_STR,
                   CURLFORM_PTRCONTENTS, status,
                   CURLFORM_CONTENTSLENGTH, status_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_description */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_description)
    {
        description = gcpp_context->oem_func_table->get_printer_description();
    }
    if (description != NULL)
    {
        int description_size = (description == NULL) ? 0 : strlen(description);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(description, description_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_DESCRIPTION_STR,
                   CURLFORM_PTRCONTENTS, description,
                   CURLFORM_CONTENTSLENGTH, description_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capshash */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capshash)
    {
        capshash = gcpp_context->oem_func_table->get_printer_capshash();
    }
    if (capshash != NULL)
    {
        int capshash_size = (capshash == NULL) ? 0 : strlen(capshash);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(capshash, capshash_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITES_HASH_STR,
                   CURLFORM_PTRCONTENTS, capshash,
                   CURLFORM_CONTENTSLENGTH, capshash_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }

    sprintf(tmpURL, "%s/register", gcpp_context->oem_config->cloudprint_url); 
        
    ret = gcpp_curl_cloudprint_request (gcpp_context, (gcpp_event_context_header_t*)anon_reg_context, tmpURL, gcpp_fetch_data_callback, &jobData, post, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE, jobData.data, jobData.size, callback_context);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        if (anon_reg_context)
        {
            JSON *json = json_parseInput(jobData.data);
            if (json)
            {
                json_getObjectString(json, "registration_token", &anon_reg_context->registration_token);
                json_getObjectString(json, "token_duration", &anon_reg_context->token_duration);
                json_getObjectString(json, "invite_url", &anon_reg_context->invite_url);
                json_getObjectString(json, "complete_invite_url", &anon_reg_context->complete_invite_url);
                json_getObjectString(json, "invite_page_url", &anon_reg_context->invite_page_url);
                json_getObjectString(json, "polling_url", &anon_reg_context->polling_url);

                JSON *printers = json_getObject(json, "printers");
                if (printers && json_getArrayCount(printers) > 0)
                {
                    JSON *printer = json_getArray(printers, 0);
                    if (printer)
                    {
                         json_getObjectString(printer, "id", &anon_reg_context->printer_id);
                    }
                }
                
            }
            else
            {
                DBG_ERR("\nJSON Parse Error:\n%s\n", jobData.data);
                
                /* indicate fatal error */
                ret = NET_GCPP_FATAL_ERROR;
            }

            json_free(json);
        }
    }
    else
    {
        DBG_ERR("Anonymous registration request failed!\n");
    }
    
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->event_notification)
    {
        gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE, anon_reg_context, callback_context);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    /* cleanup */
    if (post)
        curl_formfree(post);

    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    if (capabilities != NULL)
    {
        MEM_FREE_AND_NULL(capabilities);
    }
    
    if (defaults != NULL)
    {
        MEM_FREE_AND_NULL(defaults);
    }
    
    if (tags != NULL)
    {
        MEM_FREE_AND_NULL(tags);
    }
    
    if (status != NULL)
    {
        MEM_FREE_AND_NULL(status);
    }
    
    if (description != NULL)
    {
        MEM_FREE_AND_NULL(description);
    }
    
    if (capshash != NULL)
    {
        MEM_FREE_AND_NULL(capshash);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_claim_printer(net_gcpp_context_t *gcpp_context, char *polling_url, gcpp_registration_claim_printer_context_t *reg_completion_context, void *callback_context)
{
    struct gcppDataStruct jobData;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (polling_url == NULL)
    {
        DBG_ERR("polling_url cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
                   
    sprintf(tmpURL, "%s%s", polling_url, gcpp_context->oem_config->oauth_client_id);

    ret = gcpp_curl_cloudprint_request (gcpp_context, (gcpp_event_context_header_t*)reg_completion_context, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret ==  NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE, jobData.data, jobData.size, callback_context);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        if (reg_completion_context)
        {
            JSON *json = json_parseInput(jobData.data);
            if (json)
            {
                json_getObjectString(json, "xmpp_jid", &reg_completion_context->xmpp_jid);
                json_getObjectString(json, "authorization_code", &reg_completion_context->authorization_code);
                json_getObjectString(json, "user_email", &reg_completion_context->user_email);
                json_getObjectString(json, "confirmation_page_url", &reg_completion_context->confirmation_page_url);
                json_getObjectString(json, "printerid", &reg_completion_context->printerid);
            }
            else
            {
                DBG_ERR("\nJSON Parse Error:\n%s\n", jobData.data);
                
                /* indicate fatal error */
                ret = NET_GCPP_FATAL_ERROR;
            }

            json_free(json);
        }
    }
    else
    {
        DBG_ERR("Registration Completion request failed!\n");
    }
    
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->event_notification)
    {
        gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE, reg_completion_context, callback_context);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    /* cleanup */
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_get_authorization_tokens(net_gcpp_context_t *gcpp_context, char *auth_code, gcpp_authorization_tokens_context_t *auth_tokens_context, void *callback_context)
{
    struct gcppDataStruct jobData;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    char tmpPostData[GCPP_MAX_URL_LENGTH];
    gcpp_error_t ret = NET_GCPP_FAIL;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (auth_code == NULL)
    {
        DBG_ERR("auth_code cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->oem_config == NULL)
    {
        DBG_ERR("gcpp_context oem_config cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s/token", gcpp_context->oem_config->oauth2_url); 

    sprintf(tmpPostData, "code=%s&client_id=%s&client_secret=%s&redirect_uri=oob&grant_type=authorization_code", 
        auth_code, gcpp_context->oem_config->oauth_client_id, gcpp_context->oem_config->oauth_client_secret);
    
    ret = gcpp_curl_standard_request (gcpp_context, (gcpp_event_context_header_t*)auth_tokens_context, tmpURL, NULL, NULL, gcpp_fetch_data_callback, &jobData, NULL, tmpPostData, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_AUTH_TOKENS, jobData.data, jobData.size, callback_context);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        if (auth_tokens_context)
        {
            JSON *json = json_parseInput(jobData.data);
            if (json)
            {
                json_getObjectString(json, "access_token", &auth_tokens_context->access_token);
                json_getObjectString(json, "refresh_token", &auth_tokens_context->refresh_token);
                json_getObjectNumber(json, "expires_in", &auth_tokens_context->expires_in);
            }
            else
            {
                DBG_ERR("\nJSON Parse Error:\n%s\n", jobData.data);
                
                /* indicate fatal error */
                ret = NET_GCPP_FATAL_ERROR;
            }

            json_free(json);
        }
    }
    else
    {
        DBG_ERR("Auth tokens request failed!\n");
        ret = NET_GCPP_FATAL_ERROR;
    }
            
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->event_notification)
    {
        gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_AUTH_TOKENS, auth_tokens_context, callback_context);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
        
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_update_authorization_tokens(net_gcpp_context_t *gcpp_context, char *refresh_token, gcpp_authorization_tokens_context_t *auth_token_context, void *callback_context)
{
    struct gcppDataStruct jobData;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    char tmpPostData[GCPP_MAX_URL_LENGTH];
    gcpp_error_t ret = NET_GCPP_FAIL;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (refresh_token == NULL)
    {
        DBG_ERR("refresh_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->oem_config == NULL)
    {
        DBG_ERR("gcpp_context oem_config cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s/token", gcpp_context->oem_config->oauth2_url);

    sprintf(tmpPostData, "&client_id=%s&client_secret=%s&refresh_token=%s&grant_type=refresh_token", 
        gcpp_context->oem_config->oauth_client_id, gcpp_context->oem_config->oauth_client_secret, refresh_token);
    
    ret = gcpp_curl_standard_request (gcpp_context, (gcpp_event_context_header_t*)auth_token_context, tmpURL, NULL, NULL, gcpp_fetch_data_callback, &jobData, NULL, tmpPostData, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_AUTH_TOKENS, jobData.data, jobData.size, callback_context);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        if (auth_token_context)
        {
            JSON *json = json_parseInput(jobData.data);
            if (json)
            {
                json_getObjectString(json, "access_token", &auth_token_context->access_token);
                json_getObjectString(json, "refresh_token", &auth_token_context->refresh_token);
                json_getObjectNumber(json, "expires_in", &auth_token_context->expires_in);
            }
            else
            {
                DBG_ERR("\nJSON Parse Error:\n%s\n", jobData.data);
                
                /* indicate fatal error */
                ret = NET_GCPP_FATAL_ERROR;
            }

            json_free(json);
        }
    }
    else
    {
        DBG_ERR("Update auth tokens failed!\n");
        ret = NET_GCPP_FATAL_ERROR;
    }
    
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->event_notification)
    {
        gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_AUTH_TOKENS, auth_token_context, callback_context);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
        
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_update_xsrf_token(net_gcpp_context_t *gcpp_context)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s/xsrf?oauth_token=%s", gcpp_context->oem_config->cloudprint_url, gcpp_context->auth_tokens->access_token);
    
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret != NET_GCPP_SUCCESS)
    {
        DBG_ERR("Failed to update GCPP XSRF token!\n");
    }
    else
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_XSRF_TOKEN_UPDATED, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
    }

    /* cleanup */
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_add_printer(net_gcpp_context_t *gcpp_context, char **printer_id)
{
    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    char *capabilities = NULL;
    char *defaults = NULL;
    char *tags = NULL;
    char *status = NULL;
    char *description = NULL;
    char *capshash = NULL;
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->xsrf_token == NULL)
    {
        DBG_ERR("xsrf_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
    
    if (gcpp_context->printer_name)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PRINTER_NAME_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->printer_name, 
                   CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);
    }

    if (gcpp_context->proxy)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PROXY_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->proxy,
                   CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capabilities */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capabilities)
    {
        capabilities = gcpp_context->oem_func_table->get_printer_capabilities();
    }
    if (capabilities != NULL)
    {
        int capabilities_size = (capabilities == NULL) ? 0 : strlen(capabilities);
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nCapabilities Dump:\n");
            gcpp_hex_dump(capabilities, capabilities_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITIES_STR,
                   CURLFORM_PTRCONTENTS, capabilities,
                   CURLFORM_CONTENTSLENGTH, capabilities_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_defaults */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_defaults)
    {
        defaults = gcpp_context->oem_func_table->get_printer_defaults();
    }
    if (defaults != NULL)
    {
        int defaults_size = (defaults == NULL) ? 0 : strlen(defaults);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(defaults, defaults_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_DEFAULTS_STR,
                   CURLFORM_PTRCONTENTS, defaults,
                   CURLFORM_CONTENTSLENGTH, defaults_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_tags */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_tags)
    {
        tags = gcpp_context->oem_func_table->get_printer_tags();
    }
    if (tags != NULL)
    {
        int tags_size = (tags == NULL) ? 0 : strlen(tags);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(tags, tags_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_TAGS_STR,
                   CURLFORM_PTRCONTENTS, tags,
                   CURLFORM_CONTENTSLENGTH, tags_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_status */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_status)
    {
        status = gcpp_context->oem_func_table->get_printer_status();
    }
    if (status != NULL)
    {
        int status_size = (status == NULL) ? 0 : strlen(status);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(status, status_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_STR,
                   CURLFORM_PTRCONTENTS, status,
                   CURLFORM_CONTENTSLENGTH, status_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_description */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_description)
    {
        description = gcpp_context->oem_func_table->get_printer_description();
    }
    if (description != NULL)
    {
        int description_size = (description == NULL) ? 0 : strlen(description);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(description, description_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_DESCRIPTION_STR,
                   CURLFORM_PTRCONTENTS, description,
                   CURLFORM_CONTENTSLENGTH, description_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capshash */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capshash)
    {
        capshash = gcpp_context->oem_func_table->get_printer_capshash();
    }
    if (capshash != NULL)
    {
        int capshash_size = (capshash == NULL) ? 0 : strlen(capshash);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(capshash, capshash_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITES_HASH_STR,
                   CURLFORM_PTRCONTENTS, capshash,
                   CURLFORM_CONTENTSLENGTH, capshash_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    sprintf(tmpURL, "%s/register?oauth_token=%s&xsrf=%s", gcpp_context->oem_config->cloudprint_url,
        gcpp_context->auth_tokens->access_token, gcpp_context->xsrf_token);
        
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, post, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_PRINTER_ADDED, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        ret = gcpp_parse_printer_id(gcpp_context, jobData.data, jobData.size, printer_id);
        if (ret == NET_GCPP_SUCCESS)
        {
            DBG_VERBOSE("Register printer request completed:\n");
            DBG_VERBOSE("   printer id = %s\n", *printer_id);
            
            /* execute OEM-specific event PRINTER_ADDED */
            ASSERT(gcpp_context->oem_func_table);
            if(gcpp_context->oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_ADDED, *printer_id, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
        }
    }
    else
    {
        DBG_ERR("Register printer failed!\n");
    }

    /* cleanup */
    if (post)
        curl_formfree(post);
        
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    if (capabilities != NULL)
    {
        MEM_FREE_AND_NULL(capabilities);
    }
    
    if (defaults != NULL)
    {
        MEM_FREE_AND_NULL(defaults);
    }
    
    if (tags != NULL)
    {
        MEM_FREE_AND_NULL(tags);
    }
    
    if (status != NULL)
    {
        MEM_FREE_AND_NULL(status);
    }
    
    if (description != NULL)
    {
        MEM_FREE_AND_NULL(description);
    }
    
    if (capshash != NULL)
    {
        MEM_FREE_AND_NULL(capshash);
    }
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_get_printer_id(net_gcpp_context_t *gcpp_context, char **printer_id)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->proxy == NULL)
    {
        DBG_ERR("proxy cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;

    sprintf(tmpURL, "%s/list?proxy=%s&oauth_token=%s", gcpp_context->oem_config->cloudprint_url,
        gcpp_context->proxy, gcpp_context->auth_tokens->access_token);
    
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr,tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_GET_PRINTER_ID, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        ret = gcpp_parse_printer_id(gcpp_context, jobData.data, jobData.size, printer_id);
        if (ret == NET_GCPP_SUCCESS)
        {
            DBG_VERBOSE("Get Printer completed:\n");
            DBG_VERBOSE("   printer id = %s\n", *printer_id);
            
            /* execute OEM-specific event PRINTER_ID */
            ASSERT(gcpp_context->oem_func_table);
            if(gcpp_context->oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_GET_PRINTER_ID, *printer_id, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
        }
    }
    else
    {
        DBG_ERR("Get Printer request failed\n");
    }

    /* cleanup */
    if (jobData.data)
    {
        MEM_FREE_AND_NULL(jobData.data);
    }
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_update_printer(net_gcpp_context_t *gcpp_context, char *printer_id)
{
    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    char *capabilities = NULL;
    char *defaults = NULL;
    char *tags = NULL;
    char *status = NULL;
    char *description = NULL;
    char *capshash = NULL;
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->xsrf_token == NULL)
    {
        DBG_ERR("xsrf_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
    
    curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PRINTERID_STR,
               CURLFORM_PTRCONTENTS, printer_id, 
               CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);
    
    if (gcpp_context->printer_name)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PRINTER_NAME_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->printer_name, 
                   CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);
    }

    if (gcpp_context->display_name)
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_DISPLAY_NAME_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->display_name,
                   CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);

    if (gcpp_context->proxy)
    {
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_PROXY_STR,
                   CURLFORM_PTRCONTENTS, gcpp_context->proxy,
                   CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capabilities */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capabilities)
    {
        capabilities = gcpp_context->oem_func_table->get_printer_capabilities();
    }
    if (capabilities != NULL)
    {
        int capabilities_size = (capabilities == NULL) ? 0 : strlen(capabilities);
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nCapabilities Dump:\n");
            gcpp_hex_dump(capabilities, capabilities_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITIES_STR,
                   CURLFORM_PTRCONTENTS, capabilities,
                   CURLFORM_CONTENTSLENGTH, capabilities_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_defaults */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_defaults)
    {
        defaults = gcpp_context->oem_func_table->get_printer_defaults();
    }
    if (defaults != NULL)
    {
        int defaults_size = (defaults == NULL) ? 0 : strlen(defaults);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(defaults, defaults_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_DEFAULTS_STR,
                   CURLFORM_PTRCONTENTS, defaults,
                   CURLFORM_CONTENTSLENGTH, defaults_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_tags */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_tags)
    {
        tags = gcpp_context->oem_func_table->get_printer_tags();
    }
    if (tags != NULL)
    {
        int tags_size = (tags == NULL) ? 0 : strlen(tags);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(tags, tags_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_TAGS_STR,
                   CURLFORM_PTRCONTENTS, tags,
                   CURLFORM_CONTENTSLENGTH, tags_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_status */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_status)
    {
        status = gcpp_context->oem_func_table->get_printer_status();
    }
    if (status != NULL)
    {
        int status_size = (status == NULL) ? 0 : strlen(status);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(status, status_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_STR,
                   CURLFORM_PTRCONTENTS, status,
                   CURLFORM_CONTENTSLENGTH, status_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_description */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_description)
    {
        description = gcpp_context->oem_func_table->get_printer_description();
    }
    if (description != NULL)
    {
        int description_size = (description == NULL) ? 0 : strlen(description);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(description, description_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_STATUS_DESCRIPTION_STR,
                   CURLFORM_PTRCONTENTS, description,
                   CURLFORM_CONTENTSLENGTH, description_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    /* execute OEM-specific get_printer_capshash */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->get_printer_capshash)
    {
        capshash = gcpp_context->oem_func_table->get_printer_capshash();
    }
    if (capshash != NULL)
    {
        int capshash_size = (capshash == NULL) ? 0 : strlen(capshash);;
#ifdef DEBUG    
        if (gcpp_context->debug_on)
        {
            DBG_VERBOSE("\nDefaults Dump:\n");
            gcpp_hex_dump(capshash, capshash_size);
        }
#endif        
        curl_formadd(&post, &last, CURLFORM_COPYNAME, GCPP_PARAM_CAPABILITES_HASH_STR,
                   CURLFORM_PTRCONTENTS, capshash,
                   CURLFORM_CONTENTSLENGTH, capshash_size,
                   CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
    }
    
    sprintf(tmpURL, "%s/update?oauth_token=%s&xsrf=%s", gcpp_context->oem_config->cloudprint_url,
        gcpp_context->auth_tokens->access_token, gcpp_context->xsrf_token);

    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, post, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_PRINTER_UPDATED, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        /* execute OEM-specific event PRINTER_UPDATED */
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_notification)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_UPDATED, printer_id, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
    }
    else
    {
        DBG_ERR("Update printer failed!\n");
    }

    /* cleanup */
    if (post)
        curl_formfree(post);
        
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    if (capabilities != NULL)
    {
        MEM_FREE_AND_NULL(capabilities);
    }
    
    if (defaults != NULL)
    {
        MEM_FREE_AND_NULL(defaults);
    }
    
    if (tags != NULL)
    {
        MEM_FREE_AND_NULL(tags);
    }
    
    if (status != NULL)
    {
        MEM_FREE_AND_NULL(status);
    }
    
    if (description != NULL)
    {
        MEM_FREE_AND_NULL(description);
    }
    
    if (capshash != NULL)
    {
        MEM_FREE_AND_NULL(capshash);
    }
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return (ret);
}

gcpp_error_t gcpp_remove_printer(net_gcpp_context_t *gcpp_context, char *printer_id)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    gcpp_error_t ret = NET_GCPP_FAIL;
    struct gcppDataStruct jobData;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->xsrf_token == NULL)
    {
        DBG_ERR("xsrf_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s/delete?printerid=%s&oauth_token=%s&xsrf=%s", gcpp_context->oem_config->cloudprint_url,
        printer_id, gcpp_context->auth_tokens->access_token, gcpp_context->xsrf_token);
    
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        DBG_VERBOSE("Printer %s was successfully removed\n", printer_id);
        
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_PRINTER_REMOVED, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        /* execute OEM-specific event PRINTER_REMOVED */
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_notification)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_REMOVED, printer_id, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
    }
    else
    {
        DBG_ERR("Remove GCPP Printer failed\n");
    }

    /* cleanup */
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_remove_printers(net_gcpp_context_t *gcpp_context)
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->proxy == NULL)
    {
        DBG_ERR("proxy cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->xsrf_token == NULL)
    {
        DBG_ERR("xsrf_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    jobData.data = NULL;
    jobData.size = 0;

    sprintf(tmpURL, "%s/list?proxy=%s&oauth_token=%s&xsrf=%s", gcpp_context->oem_config->cloudprint_url,
        gcpp_context->proxy, gcpp_context->auth_tokens->access_token, gcpp_context->xsrf_token);
    
    gcpp_event_context_header_t hdr;
    gcpp_error_t result = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (result == NET_GCPP_SUCCESS)
    {
        if (hdr.success)
        {
            ret = NET_GCPP_SUCCESS;
            
            JSON *json = json_parseInput(jobData.data);
            if (json)
            {
                JSON *printers = json_getObject(json, "printers");
                if (printers && json_getArrayCount(printers) > 0)
                {
                    int i, count;
                    
                    count = json_getArrayCount(printers);
                    DBG_VERBOSE("Number of printers found = %d\n", count);
                    for (i=0;i<count;i++)
                    {
                        JSON *printer = json_getArray(printers, i);
                        if (printer)
                        {
                            char *printer_id;
                            error_type_t e_res = json_getObjectString(printer, "id", &printer_id);
                            if (e_res == OK && printer_id)
                            {
                                DBG_MSG("Removing printer id = %s\n", printer_id);
                                
                                gcpp_remove_printer(gcpp_context, printer_id);
                                MEM_FREE_AND_NULL(printer_id);
                            }
                            else
                            {
                                DBG_ERR("Unable to parse printer id from JSON data\n");
                            }
                        }
                    }
                }
            }
            else
            {
                DBG_ERR("\nJSON Parse Error:\n%s\n", jobData.data);
            }

            json_free(json);
        }
    }
    else
    {
        DBG_ERR("Get Printer request failed\n");
    }

    /* cleanup */
    if (jobData.data)
    {
        MEM_FREE_AND_NULL(jobData.data);
    }
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_error_t gcpp_printer_get_jobs(net_gcpp_context_t *gcpp_context, char *printer_id, gcpp_job_t ***jobs)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (jobs == NULL)
    {
        DBG_ERR("jobs cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (printer_id == NULL)
    {
        DBG_ERR("printer_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    *jobs = NULL;
    
    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s/fetch?printerid=%s&oauth_token=%s", gcpp_context->oem_config->cloudprint_url,
        printer_id, gcpp_context->auth_tokens->access_token);
    
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_cloudprint_request (gcpp_context, &hdr, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret == NET_GCPP_SUCCESS)
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_PRINTER_JOBS, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        ret = gcpp_parse_jobs(gcpp_context, jobData.data, jobData.size, jobs);
        if (ret == NET_GCPP_SUCCESS)
        {
            DBG_VERBOSE("Retrieved jobs list.\n");
        }
        else
        {
            DBG_ERR("Failed to retrieve jobs list!\n");
        }
        
        /* execute OEM-specific event PRINTER_REMOVED */
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_notification)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_JOBS, *jobs, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
    }
    else
    {
        DBG_ERR("Fetch GCPP Jobs failed\n");
    }

    /* cleanup */
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_job_t *gcpp_printer_new_job()
{
    gcpp_job_t *new_job = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    new_job = (gcpp_job_t *)MEM_MALLOC(sizeof(gcpp_job_t));
    if (new_job)
    {
        memset(new_job, 0, sizeof(gcpp_job_t));
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return new_job;
}

gcpp_job_t *gcpp_printer_copy_job(gcpp_job_t *source)
{
    gcpp_job_t *new_job = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    new_job = (gcpp_job_t *)MEM_MALLOC(sizeof(gcpp_job_t));
    if (new_job)
    {
        memcpy(new_job, source, sizeof(gcpp_job_t));
        
        new_job->id = gcpp_strdup(source->id);
        new_job->printer_id = gcpp_strdup(source->printer_id);
        new_job->title = gcpp_strdup(source->title);
        new_job->user = gcpp_strdup(source->user);
        new_job->file_url = gcpp_strdup(source->file_url);
        new_job->ticket_url = gcpp_strdup(source->ticket_url);
        if (source->tags)
        { 
           int ind=0, cnt=0;
           while (source->tags [cnt] != NULL) cnt++;
            
           new_job->tags = (char**)MEM_MALLOC (sizeof(char*) * (cnt + 1));
           while (ind < cnt)
           {
               new_job->tags [ind] = gcpp_strdup(source->tags[ind]);
               ind++;
           }         
           new_job->tags [ind] = NULL;   
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return new_job;
}

void gcpp_printer_delete_job(gcpp_job_t *job)
{


    if (job == NULL)
    {
        DBG_ERR("job cannot be NULL.\n" );
        return;
    }

    DBG_VERBOSE( "%s enter %p id %d \n", __FUNCTION__, job, job->id );    

    if (job->id)
        MEM_FREE_AND_NULL(job->id);

    if (job->printer_id)
        MEM_FREE_AND_NULL(job->printer_id);
        
    if (job->title)
        MEM_FREE_AND_NULL(job->title);
        
    if (job->user)
        MEM_FREE_AND_NULL(job->user);
        
    if (job->file_url)
        MEM_FREE_AND_NULL(job->file_url);
        
    if (job->ticket_url)
        MEM_FREE_AND_NULL(job->ticket_url);
        
    if (job->tags)
    { 
       int ind=0;
       while (job->tags [ind] != NULL)
       {
          MEM_FREE_AND_NULL(job->tags [ind]);
          ind++;
       }
       MEM_FREE_AND_NULL(job->tags);
    }
        
    MEM_FREE_AND_NULL(job);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

void gcpp_printer_delete_jobs(gcpp_job_t **jobs)
{
    gcpp_job_t *job = NULL;
    int index = 0;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (jobs == NULL)
    {
        DBG_ERR("jobs cannot be NULL.\n" );
        return;
    }
    
    do
    {
        job = jobs[index++];
        if (job)
        {
            gcpp_printer_delete_job(job);
        }
    }
    while (job);
    
    MEM_FREE_AND_NULL(jobs);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

gcpp_error_t gcpp_printer_get_job_ticket(net_gcpp_context_t *gcpp_context, char *job_ticket_url, gcpp_recv_job_header_t recv_job_header_func, char **job_ticket_data, uint32_t *job_ticket_size, void *context)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    struct curl_slist *headers = NULL;
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (job_ticket_url == NULL)
    {
        DBG_ERR("job_ticket_url cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (job_ticket_data == NULL)
    {
        DBG_ERR("job_ticket_data cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    *job_ticket_data = NULL;
    *job_ticket_size = 0;        
    
    jobData.data = NULL;
    jobData.size = 0;
    
    sprintf(tmpURL, "%s&oauth_token=%s", job_ticket_url, gcpp_context->auth_tokens->access_token);
    
    char *httpProxyHeader = MEM_MALLOC(strlen(GCPP_PROXY_HEADER)+strlen(gcpp_context->proxy));
    if (httpProxyHeader)
    {
        sprintf(httpProxyHeader, GCPP_PROXY_HEADER, gcpp_context->proxy);
        headers = curl_slist_append(headers, httpProxyHeader);
        MEM_FREE_AND_NULL(httpProxyHeader);
    }
    
    struct gcppJobRecvStruct jobRecv;
    jobRecv.recv_header_func = recv_job_header_func;
    jobRecv.recv_data_func = NULL;
    jobRecv.context = context;

    void *header_callback = (recv_job_header_func == NULL) ? NULL : gcpp_job_header_callback;

    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_standard_request (gcpp_context, &hdr, tmpURL, header_callback, &jobRecv, gcpp_fetch_data_callback, &jobData, NULL, NULL, headers);
    if (ret == NET_GCPP_SUCCESS)
    {
        *job_ticket_data = MEM_MALLOC(jobData.size);
        if (*job_ticket_data)
        {
            memcpy(*job_ticket_data, jobData.data, jobData.size);
            *job_ticket_size = jobData.size;
        }
        else
        {
            ret = NET_GCPP_LOW_MEM;
        }
    }
    else
    {
        DBG_ERR("Fetch GCPP SM Job Ticket request failed\n");
    }

    /* cleanup */
    if (headers)
        curl_slist_free_all(headers);    
        
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}


gcpp_error_t gcpp_printer_get_job(net_gcpp_context_t *gcpp_context, char *job_url, uint32_t skip_pages, char *mime_types, gcpp_recv_job_header_t recv_job_header_func, gcpp_recv_job_data_t recv_job_data_func, void *context)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    gcpp_error_t ret = NET_GCPP_FAIL;
    struct curl_slist *headers = NULL;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (job_url == NULL)
    {
        DBG_ERR("job_url cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    /* append GCPP access token, and skip_pages if available */
    if (skip_pages)
    {
        sprintf(tmpURL, "%s&oauth_token=%s&skippages=%d", job_url, gcpp_context->auth_tokens->access_token, skip_pages);
    }
    else
    {
        sprintf(tmpURL, "%s&oauth_token=%s", job_url, gcpp_context->auth_tokens->access_token);
    }
 
    /* additional GCPP headers to send with file URL */
    char *mime = (mime_types == NULL) ? GCPP_MIMETYPE_IMAGE : mime_types;    
    int httpProxyHeaderLength = strlen(GCPP_PRINT_DATA_HEADER)+strlen(gcpp_context->proxy)+strlen(mime);
    char *httpProxyHeader = MEM_MALLOC(httpProxyHeaderLength);
    if (httpProxyHeader)
    {
        sprintf(httpProxyHeader, GCPP_PRINT_DATA_HEADER, mime, gcpp_context->proxy);
        headers = curl_slist_append(headers, httpProxyHeader);
        MEM_FREE_AND_NULL(httpProxyHeader);
    }

    struct gcppJobRecvStruct jobRecv;
    jobRecv.recv_header_func = recv_job_header_func;
    jobRecv.recv_data_func = recv_job_data_func;
    jobRecv.context = context;

    void *header_callback = (recv_job_header_func == NULL) ? NULL : gcpp_job_header_callback;
    void *data_callback = (recv_job_data_func == NULL) ? NULL : gcpp_job_data_callback;
    gcpp_event_context_header_t hdr;
    ret = gcpp_curl_standard_request (gcpp_context, &hdr, tmpURL, header_callback, &jobRecv, data_callback, &jobRecv, NULL, NULL, headers);
    if (ret == NET_GCPP_SUCCESS)
    {
	// success
    }
    else
    {
        DBG_ERR("Fetch GCPP SM Job Data request failed\n");
    }
    
    /* cleanup */
    if (headers)
        curl_slist_free_all(headers);    
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}

gcpp_error_t gcpp_job_update_status(net_gcpp_context_t *gcpp_context, char *job_id, char *status, char *code, char *message)
{
    char tmpURL[GCPP_MAX_URL_LENGTH];
    struct gcppDataStruct jobData;
    gcpp_error_t ret = NET_GCPP_FAIL;
    gcpp_printer_status_context_t printer_status;

    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (gcpp_context == NULL)
    {
        DBG_ERR("gcpp_context cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    if (job_id == NULL)
    {
        DBG_ERR("job_id cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->auth_tokens == NULL || gcpp_context->auth_tokens->access_token == NULL)
    {
        DBG_ERR("access_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (gcpp_context->xsrf_token == NULL)
    {
        DBG_ERR("xsrf_token cannot be NULL.\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (status == NULL)
    {
        status = "DONE";
    }

    DBG_MSG("Updating GCPP Job Status - ID = %s, to status = %s\n", job_id, status );
        
    jobData.data = NULL;
    jobData.size = 0;
    
    memset(&printer_status, 0, sizeof(gcpp_printer_status_context_t));
    
    sprintf(tmpURL, "%s/control?jobid=%s&status=%s&oauth_token=%s&xsrf=%s", gcpp_context->oem_config->cloudprint_url,
        job_id, status, gcpp_context->auth_tokens->access_token, gcpp_context->xsrf_token);
    
    ret = gcpp_curl_cloudprint_request (gcpp_context, (gcpp_event_context_header_t*)&printer_status, tmpURL, gcpp_fetch_data_callback, &jobData, NULL, NULL);
    if (ret != NET_GCPP_SUCCESS)
    {
        DBG_ERR("Update GCPP Job failed\n");
    }
    else
    {
        ASSERT(gcpp_context->oem_func_table);
        if(gcpp_context->oem_func_table->event_data)
        {
            gcpp_error_t rcode = gcpp_context->oem_func_table->event_data(NET_GCPP_EVENT_PRINTER_STATUS_UPDATED, jobData.data, jobData.size, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
    }
        
    /* execute OEM-specific event PRINTER_STATUS_UPDATED */
    ASSERT(gcpp_context->oem_func_table);
    if(gcpp_context->oem_func_table->event_notification)
    {
        printer_status.job_id = job_id;
        printer_status.status = status;
        
        gcpp_error_t rcode = gcpp_context->oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_STATUS_UPDATED, &printer_status, NULL);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    /* cleanup */
    if (jobData.data)
        MEM_FREE_AND_NULL(jobData.data);
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ret;
}
