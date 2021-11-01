/*
 * ============================================================================
 * Copyright (c) 2009-2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *
 * \file http_api.h
 *
 * \brief This file is the main API file for the HTTP module.
 *
 * HTTP RESOURCE API:
 * The HTTP Resource specific API allows a module to register for
 * notification and assume the responsibility of responding to specific 
 * resource requests. 
 *
 * The registering module will receive a series of callbacks when the 
 * resource is requested, and then the module will send back the
 * appropriate response.
 *
 * For example a scan module could register the "/dev/scan"
 * resource (URI) and be notified when a request is received from a 
 * client. This could allow the scan module to perform a scan upon 
 * demand from an HTTP client and return the scan data through the HTTP 
 * pipe. 
 *
 * In addition this API supports registering for a request on a specific 
 * port or all ports. 
 *  
 * There are two options on how to register with the HTTP module: 
 *  
 * 1. http_resource_register - This method will provide a series of 
 * callbacks (http_resource_event_t) to indicate data has been received. 
 * As part of these callbacks, data buffers are provided that contain 
 * request header and body data.  A blocking send call 
 * (http_resource_send_data or http_resource_send_data_no_free ) is used 
 * to send response body data. 
 *  
 * The following API functions are exclusive to this method: 
 * http_resource_register, http_resource_buffer_free, 
 * http_resource_send_data, http_resource_send_data_no_free 
 *  
 *  
 * 2. http_resource_register_event - This method provides a single event
 * notification (HTTP_RESOURCE_HEADER_DATA_AVAILABLE) indicating that a 
 * registered resource has been recieved.  The module can then call 
 * http_resource_read to get the body data of the request, filling in 
 * a buffer provided by the calling module.  This method will eliminate 
 * at least one memory copy and should be used when large amounts of 
 * incoming data are expected.  Response body data is sent using the 
 * http_resource_write API, which is a non-blocking send. 
 *  
 * The following API functions are exclusive to this method: 
 * http_resource_register_event, http_resource_read, http_resource_write
 *  
 * All other API functions can be called from either method.  Below is a sample 
 * interaction using both methods. 
 *  
 * Method 1 - http_register_resource(): 
 *  
 * Receive callback for HTTP_RESOURCE_HEADER_DATA 
 * Call http_resource_get_method
 * Call http_resource_get_content_length 
 * Receive callback for HTTP_RESOURCE_BODY_DATA 
 * Receive callback for HTTP_RESOURCE_BODY_DATA 
 * Receive callback for HTTP_RESOURCE_BODY_DATA 
 * Receive callback for HTTP_RESOURCE_BODY_DATA_END 
 * Call http_resource_set_status_code 
 * Call http_resource_set_content_type 
 * Call http_resource_header_complete 
 * Call http_resource_send_data reapeatedly until all data has been sent
 * Call http_resource_buffer_free for each buffer received in the 
 * callbacks 
 * Call http_resource_send_complete 
 *  
 * Method 2 - http_resource_register_event(): 
 *  
 * Receive callback for HTTP_RESOURCE_HEADER_DATA_AVAILABLE 
 * Call http_resource_get_header_length 
 * Call http_resource_get_header 
 * Call http_resource_read until all data has been read
 * Call http_resource_set_status_code 
 * Call http_resource_set_content_type 
 * Call http_resource_header_complete 
 * Call http_resource_write until all data has been written 
 * Call http_resource_send_complete
  
 *  
 */

#ifndef _INC_HTTP_API_H
#define _INC_HTTP_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "error_types.h"
#ifdef HAVE_NETWORK
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#else
#include "trsocket.h"
#endif
#endif // HAVE_NETWORK
#include <pthread.h>
#include <posix_ostools.h>

/**
 * \brief This error code indicates that the requested information is
 *        not available. This could be because it was not sent in the
 *        current request.
*/
#define HTTP_ERROR_NO_RESULTS (error_type_t)-11
/**
 * \brief This error code indicates that the buffer is too small
 */
#define HTTP_ERROR_BUFFER_TOO_SMALL (error_type_t)-12


/**
 * \brief This value is used to signal that the HTTP request is chunk 
 *        encoded and no content length is available.
 */
#define CHUNKED_ENCODING    0xFFFFFFFF


/** \brief Supported HTTP response status codes */
typedef enum
{
    HTTP_100_CONTINUE,
    HTTP_200_OK,
    HTTP_201_CREATED,
    HTTP_202_ACCEPTED,
    HTTP_204_NO_CONTENT,
    HTTP_301_MOVED,
    HTTP_304_NOT_MODIFIED,
    HTTP_400_BAD_REQUEST,
    HTTP_401_UNAUTHORIZED,
    HTTP_403_FORBIDDEN,
    HTTP_404_NOT_FOUND,
    HTTP_405_METHOD_NOT_ALLOWED,
    HTTP_409_CONFLICT,
    HTTP_500_SERVER_ERROR,
    HTTP_503_SERVICE_UNAVAILABLE,

} http_status_code_t;


/** 
 *  \brief Arguments passed as part of HTTP request either
 *  after requested resource or in POST data
 **/
typedef struct
{
    char **     names;
    char **     values;
    uint32_t    cnt;
} http_request_cgi_args_t;


/**
 * \brief Supported HTTP content types
 **/
typedef enum http_response_type_e
{
    HTTP_TYPE_HTML = 0,
    HTTP_TYPE_XML,
    HTTP_TYPE_PLAIN,
    HTTP_TYPE_JPEG,
    HTTP_TYPE_GIF,
    HTTP_TYPE_PNG,
    HTTP_TYPE_SOAP_XML,
    HTTP_TYPE_OCTET,
    HTTP_TYPE_CSS,
    HTTP_TYPE_JAVASCRIPT,
    HTTP_TYPE_IPP,
    HTTP_TYPE_UNKNOWN,
} http_response_type_t;


/**
 *
 * \brief HTTP Methods
 *
 */
typedef enum http_request_method_e
{
    HTTP_METHOD_GET,
    HTTP_METHOD_PUT,
    HTTP_METHOD_POST,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_HEAD
} http_request_method_t;


/** \brief A unique identifier for each http request */
typedef uint32_t http_request_t;


/**
 *
 * \brief Initialize the HTTP Module.
 *
 * This function should be called at system startup after the network
 * stack and USB device initialization has been completed.
 *
 **/
void http_init( void );


/******************************************************* 
                    Method 1 API functions
********************************************************/
/**
 * \brief This is the set of events that will be returned as part of the
 *        callback when a resource that was registered is requested.
 **/
typedef enum 
{
    HTTP_RESOURCE_HEADER_DATA,
    HTTP_RESOURCE_BODY_DATA,
    HTTP_RESOURCE_BODY_DATA_END,
    HTTP_RESOURCE_CLOSED
} http_resource_event_t;


/**
 * 
 * \brief Definition for resource event callback function.  
 * Do not send http data from within callback - this will lock out any
 * other HTTP connections
 *
 * \param request A handle to identify unique requests as they 
 *       come
 *
 * \param event The resource-specific event that has occured for the
 * unique resuest
 *
 * \param buffer A data buffer that contains either header or body data
 *
 * \param size The size of the data buffer in bytes, if applicable
 * 
 * \note The module who recieves the resource event callback MUST call 
 * the http_resource_buffer_free() function once done processing the 
 * buffer 
 *
 */
typedef void (*http_resource_event_cb_t)( http_request_t request,
                                          http_resource_event_t event,
                                          char *buffer,
                                          uint32_t buffer_size_in_bytes );


/**
 *
 * \brief Register a resource (URI) so that the caller will be notified 
 * when a request comes in for the resource 
 *  
 * \param[in] resource A string representing the resource to 
 * receive notification about, for example: "/dev/scan" 
 *  
 * \param[in] port Integer port number to match against, 0 
 *        indicates any port
 *  
 * \param[in] event_cb A function in the form of the http 
 * resource event callback that will be called by the HTTP 
 * module when a matching request for the specific resource / 
 * port combination is received from a client 
 * 
 * \retval OK upon success, FAIL otherwise
 *  
 * This function registers a specific resource/port combination 
 * to be used to match against incoming HTTP requests.  If a 
 * match is found, the callback funciton will be notified with 
 * specific events. 
 *  
 * The resource can end with '*' as a wildcard, meaning that the
 * source modules wants all requests that match up to the '*'. 
 * The '*' is only valid as the last character in the resource. 
 *  
 * If the port is 0, the resource will be compared against 
 * requests on all ports.  If the resource is NULL, all requests 
 * on the provided port will be forwarded.  If both the port and 
 * resource are set, then both must match for the callback to be 
 * made. 
 *  
 * If a non 0 port is specified and a socket for listening on 
 * this port has not already been created as part of this call. 
 *  
 * The memory for the resource must not be freed until after the 
 * deregister function has been called.  
 *  
 */
error_type_t http_resource_register( char *resource, 
                                     int32_t port,
                                     http_resource_event_cb_t event_cb );


/**
 * \brief Free memory that was received in the http resource
 * registered callback function 
 *  
 * \param[in] request Handle to request that received the buffer
 *  
 * \param[in] buffer Pointer to memory to be freed (buffer 
 * should have been received as part of the 
 * http_resource_event_cb_t) 
 * 
 * \retval error_type_t OK upon success, FAIL otherwise
 *  
 * The HTTP server will provide the registering module 
 * with data buffers as part of the callback routines. After the
 * module has processed that information, they need to be freed 
 * via this API call.  If there are extra buffers when the HTTP 
 * server receives the http_resource_send_complete call, the 
 * code will ASSERT.  
 */
error_type_t http_resource_buffer_free( http_request_t request, char *buffer );


/**
 *
 * \brief This function is to be called when there is data for the body 
 * of the HTTP request 
 *
 * \note If retVal is OK then buffer gets freed as part of this call
 *
 * \note this will make a blocking send call
 * 
 * \param[in] request Handle to current http request 
 *  
 * \param[in] buffer Data to send, will be freed
 *  
 * \param[in] size Length in bytes of buffer 
 *  
 * \retval OK upon success, FAIL otherwise
 *
 */
error_type_t http_resource_send_data( http_request_t request,
                                      char *buffer,
                                      uint32_t size );


/**
 *
 * \brief This function is to be called when there is data for the body 
 * of the HTTP request 
 *
 * \note buffer will NOT be freed as part of this call
 *
 * \note this will make a blocking send call
 * 
 * \param[in] request Handle to current http request 
 *  
 * \param[in] buffer Data to send, will not be freed
 *  
 * \param[in] size Length in bytes of buffer 
 *  
 * \retval OK upon success, FAIL otherwise
 *
 */
error_type_t http_resource_send_data_no_free( http_request_t request, 
                                              char *buffer, uint32_t size );


/******************************************************* 
                    Method 2 API functions
********************************************************/

/**
 *
 * \brief This is the set of events that will be returned as 
 * part of the callback when a resource that was registered is 
 * requested. The receiving module will then need to call 
 * http_resource_read and http_resource_write to retrieve and 
 * send data. 
 *  
 */
typedef enum 
{
    HTTP_RESOURCE_HEADER_DATA_AVAILABLE,
    HTTP_RESOURCE_CONNECTION_ABORTED
} http_resource_event_flag_t;


/**
 * 
 * \brief Definition for resource event callback function.  
 * Keep processing to a minimun inside the callback as it is running 
 * on the HTTP server threads 
 *
 * \param[in] request A handle to identify unique requests as they 
 *       come
 *  
 * \param[in] event The resource-specific event that has occured for 
 * the unique resuest 
 *  
 * \param[in] usr_ctxt Value provided by module during registration 
 *       for a resource.
 *  
 */
typedef void (*http_resource_event_flag_cb_t)( http_request_t handle,
                                               http_resource_event_flag_t event,
                                               void *usr_ctxt );


/**
 *
 * \brief Register a resource (URI) so that the caller will be notified
 * when a request comes in for the resource
 *  
 * \param[in] resource A string representing the resource to receive 
 * notification about, for example: "/dev/scan".  A NULL resource
 * indicates any request on the specified port.
 *  
 * \param[in] port Integer port number to match against, 0 indicates any
 *       port
 *  
 * \param[in] flags Currently unused - for future expansion
 * 
 * \param[in] event_cb A function pointer that will be called by the
 * HTTP module when a matching request for the specific resource / port
 * combination is received from a client
 * 
 * \param[in] usr_ctxt User data that will be passed back with each
 *       notification
 * 
\retval OK upon success, FAIL otherwise
 *  
 * This function registers a specific resource/port combination to be
 * used to match against incoming HTTP requests.  If a match is found,
 * the callback funciton will be notified.
 *  
 * The resource can end with '*' as a wildcard, meaning that the source 
 * modules wants all requests that match up to the '*'. The '*' is only
 * valid as the last character in the resource.
 *  
 * If the port is 0, the resource will be compared against requests on
 * all ports.  If the resource is NULL, all requests on the provided
 * port will be forwarded.  If both the port and resource are set, then
 * both must match for the callback to be made.
 *  
 * If a non 0 port is specified and a socket for listening on this port
 * is not already open, it will created and opened as part of this call.
 *  
 * The memory for the resource must not be freed until after the
 * deregister function has been called.
 *  
 */
error_type_t http_resource_register_event( char *resource, 
                                           int32_t port,
                                           uint32_t flags,
                                           http_resource_event_flag_cb_t event_cb,
                                           void *usr_ctxt );


/**
 * After the HTTP_RESOURCE_HEADER_DATA_AVAILABLE event has been 
 * received a module will call this function to read data for 
 * an HTTP request. 
 * 
 * @param[in] request Handle to http resource
 * @param[out] buffer Memory buffer allocated by caller that 
 *       will be filled with data
 * @param[in] len Size of buffer in bytes
 * @param[in/out] timeout Duration of time in milliseconds to 
 *       wait for len bytes to be recieved.  Reset to 0 if
 *       timeout occurs
 * 
 * @return int32_t Standard BSD socket values
 * @retval >0 --> Number of bytes actually read 
 * @retval 0  --> EOF 
 * @retval <0 --> Error or timeout with no bytes read, if 
 *         return value of timeout is zero, then a a timeout
 *         occurred otherwise an error has occurred
 *  
 *  This is a blocking recv call until all of the requested
 *  bytes have been received or the timeout expires.  If
 *  timeout expires and len bytes have not been read, any
 *  bytes that have been read will be sent back and the timeout
 *  parameter will be reset to 0
 *  
 */
int32_t http_resource_read( http_request_t request, 
                            char *buffer, 
                            uint32_t len, 
                            uint32_t *timeout );


/**
 * After all body data has been read, the HTTP response header 
 * API's have been called then a module will call this function
 * to write any response data for an HTTP request.  Once 
 * finished sending response data, then 
 * http_resource_send_complete needs to be called to terminate 
 * the HTTP request. 
 * 
 * @param[in] request Handle to http resource
 * @param[in] buffer Malloced buffer of data that will be sent 
 *       as body data for current response
 * @param[in] len Size of buffer in bytes
 * 
 * @return int32_t 
 *      >=0 --> Number of bytes actually queued,
 *      <0  --> Error
 *  
 *  This call is non-blocking.  Data will be added to a linked
 *  list and queued to send whenever the socket is available.
 *  The memory will be freed after it is sent
 *  
 */
int32_t http_resource_write( http_request_t request, char *buffer, uint32_t len );


/******************************************************* 
    Helper API functions - Common to both Methods
********************************************************/

/**
 *
 * \brief De-Register a resource (URI) that was previously
 * registered
 *
 * \param[in] resource A string representing the resource to 
 * receive notification about, for example: "/dev/scan" 
 *  
 * \param[in] port The numerical port used in registration. 
 *  
 * \retval OK upon success, FAIL otherwise
 *
 * If a non zero port has been specified, the socket associated 
 * with the port number / type will be closed - provided it is 
 * not being used by any other registered resources. 
 *
 */
error_type_t http_resource_deregister( char *resource, int32_t port );


/**
 * Request the length of the HTTP header for a specific request
 * 
 * @param[in] request Handle to HTTP request
 * @param[out] len Length of request in bytes
 * 
 * @return error_type_t OK on success, else FAIL
 */
error_type_t http_resource_get_header_length( http_request_t request, 
                                              uint32_t *len );


/**
 * Get a copy of the entire HTTP header for a specific request
 * 
 * @param[in] request Handle to HTTP request
 * @param[out] buffer Buffer provided by caller where HTTP header will 
 *       be copied
 * @param[in] buffer_len Length in bytes of buffer
 * 
 * @return error_type_t OK on success, else FAIL
 */
error_type_t http_resource_get_header( http_request_t request,
                                       char *buffer, 
                                       uint32_t buffer_len );


/**
 * Get the HTTP method for the current request
 * 
 * @param[in] request Handle to HTTP request
 * @param[out] method HTTP method for current request
 * 
 * @return error_type_t 
 */
error_type_t http_resource_get_method( http_request_t request, 
                                       http_request_method_t *method );


/**
 *
 * \brief Allows a module to fetch the requested resource without
 * having to parse the HTTP header
 *
 * \param[in] request Handle to current http request
 * 
 * \param[out] resource URI requested.  This memory is owned and will
 * be freed by the http server - Do not modify this string. 
 *  
 * \retval OK upon success, FAIL otherwise
 * 
 */
error_type_t http_resource_get_requested_resource( http_request_t request, 
                                                   char **resource );


/** 
 * \brief Allows a module to retrieve the URL parameters that were
 * sent.  These will be in name/value pairs.  The value of
 * params will be NULL if no parameters were received.
 * 
 * \param[in] request Handle to current http request 
 *  
 * \param[out] params - place to save URL parameters, memory for 
 *       this data will be freed by the HTTP server when current
 *       response has been sent
 * 
 * \retval OK on success, HTTP_ERROR_NO_RESULTS no params in 
 *         current request, else FAIL
 * 
 **/
error_type_t http_resource_get_resource_params( http_request_t request,
                                                http_request_cgi_args_t **params);


/**
 * Get the length of the body data for this request
 * 
 * @param[in] request Handle to HTTP request
 * @param[out] len Length of the body data in bytes.  If request is 
 *       chunk encoded, the content length is unknown and the value
 *       CHUNKED_ENCODING will be returned
 * 
 * @return error_type_t 
 */
error_type_t http_resource_get_content_length( http_request_t request, 
                                               uint32_t *len );


/**
 * Get the content type of the body data for this request
 * 
 * @param[in] request Handle to HTTP request
 * @param[out] type Type of the body data.  If type is
 *       HTTP_TYPE_UNKNOWN then the request has a type  that is not
 *       well known to the HTTP module.  Use the
 *       http_resource_get_header_value to get the string
 *       representation of the type field for more information.
 * 
 * @return error_type_t 
 */
error_type_t http_resource_get_content_type( http_request_t request, 
                                             http_response_type_t *type );


/**
 *  
 * Get an HTTP header value based off header name from a specific request 
 * 
 * @param[in] request - Handle to HTTP request
 * @param[in] header_name - String value of header field to look for
 * @param[out] header_value - Buffer provided by caller that will store 
 *       the header value
 * @param[in] header_value_len - Length of header_value buffer in bytes
 * 
 * @return error_type_t 
 * @retval OK - Header field found and result in header_value 
 * @retval HTTP_ERROR_NO_RESULTS - Did not find requested header name 
 *         in current request
 * @retval HTTP_ERROR_BUFFER_TOO_SMALL - Buffer is too small to fit 
 *         header value
 * @retval FAIL - Other failure 
 */
error_type_t http_resource_get_header_value( http_request_t request, 
                                             char *header_name, 
                                             char *header_value, 
                                             uint32_t header_value_len );


/**
 * \brief Send an HTTP 100 Continue response. 
 *  
 * \param[in] request Handle to current http request 
 *  
 * \retval error_type_t OK on success, else FAIL 
 *  
 * This will send a complete HTTP transaction that only contains
 * the 100 Continue line.  This allows to send a full HTTP 
 * response later, indicates to other side of connection that we 
 * have received and not yet rejected the request. 
 *  
 */
error_type_t http_resource_send_100_continue( http_request_t request );


/**
 *
 * \brief Send a response to the request
 * 
 * \param[in] request Handle to current request
 * 
 * \param[in] code HTTP status code value to send in HTTP
 *
  \retval OK upon success, FAIL otherwise
 * 
 * It is Required that this function be called when the event
 * callback is called with the HTTP_RESOURCE_HEADER_DATA event
 * parameter
 * 
 */
error_type_t http_resource_set_status_code( http_request_t request,
                                            http_status_code_t code );


/**
 * 
 * \brief This function can be called to set the content type for a 
 * specific request 
 *  
 * \note It is recommended that this be called when the event callback 
 * is called with the HTTP_RESOURCE_HEADER_DATA event parameter 
 *  
 * \param[in] request Handle to current request 
 *  
 * \param[in] type HTTP type value of data being sent in HTTP 
 *       response
 *  
 * \retval OK upon success, FAIL otherwise
 *
 */
error_type_t http_resource_set_content_type( http_request_t request,
                                             http_response_type_t type );


/**
 *  
 * \brief This function can be called to set the content length 
 *        for a specific request
 *  
 * \param[in] request Handle to current http request 
 *  
 * \param[in] length The number of bytes that will be sent out 
 *        with the HTTP response
 *  
 * \retval OK upon success, FAIL otherwise 
 *  
 * This function is optional.  If this function is omitted, the
 * HTTP response will be sent out chunk encoded.
 * 
 */
error_type_t http_resource_set_content_length( http_request_t request, 
                                               uint32_t length );


/**
 *
 * \brief This function allows the module who registered for requests 
 * to specific resources to add resource-specific header fields. 
 *  
 * \param[in] request Handle to current http request 
 *  
 * \param[in] name String to be used as header name field 
 *  
 * \param[in] name_size length in bytes of param name 
 *  
 * \param[in] value Value associated with the name 
 *       parameter
 *  
 * \param[in] value_size length in bytes of param value
 *  
 * The http server adds the following headers:
 * \code
 * Transfer-Encoding: chunked\r\n  -- If content length not 
 * specified 
 * Server: "HTTP_SERVER_DESCRIPTION"\r\n  -- defined in 
 * http_config.h Expires: Thu, 01 Jan 1970 00:00:00 GMT\r\n 
 * Cache-Control: no-cache\r\n 
 * \endcode
 * 
 * \retval OK upon success, FAIL otherwise
 *  
 * This is optional if there are custom header fields that 
 * should be added for the given request. The buffers that are
 * passed to this function are NOT freed - it is the callers
 * reponsibility to free them if necessary
 * 
 */
error_type_t http_resource_add_header_field( http_request_t request,
                                             char *name,
                                             uint32_t name_size,
                                             char *value,
                                             uint32_t value_size );


/**
 *
 * \brief Signal to the HTTP module that the response header is ready to 
 * be sent to the client 
 *
 * \note It is Required that this function be called when the event 
 * callback is called with the HTTP_RESOURCE_HEADER_DATA event parameter 
 *  
 * \param[in] request Handle to current http request 
 *  
 * \retval OK upon success, FAIL otherwise
 *
 */
error_type_t http_resource_header_complete( http_request_t request );


/**
 *
 * \brief signal to the HTTP module that the data response is ready to 
 * be sent to the client 
 *
 * \note It is Required that this function be called when all the data 
 * has been sent using the http_send_data() function 
 * 
 * \param[in] request Handle to current http request 
 *  
 * \retval OK upon success, FAIL otherwise
 *
 */
error_type_t http_resource_send_complete( http_request_t request );


#ifdef HAVE_NETWORK
/** 
 * \brief Retrieves the address of the server accepting this request
 * HTTP connection
 * 
 * \param request - handle to request information
 * 
 * \param addr - buffer for this server address
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to gather the server address information for an
 * HTTP request.  The buffer for addr must be allocated by the
 * calling function.
 * 
 **/
error_type_t http_resource_get_server_addr( http_request_t request,
                                            struct sockaddr_storage *addr );


/**
 *
 * \brief This request will populate the sockaddr_storage with the peer
 * address information.  The calling function must allocate
 * memory for the sockaddr_storage.
 *
 * \param[in] request Handle to current http request 
 *  
 * \param[out] addr Data structure populated with socket info 
 *  
 * \retval OK upon success, FAIL otherwise
 * 
 */
error_type_t http_resource_get_peer_addr( http_request_t request, 
                                          struct sockaddr_storage *addr );


/**
 *
 * \brief This request will populate the sockaddr_storage with 
 * the sock address information.  The calling function must 
 * allocate memory for the sockaddr_storage. 
 *
 * \param[in] request Handle to current http request 
 *  
 * \param[out] addr Data structure populated with socket info 
 *  
 * \retval OK upon success, FAIL otherwise
 * 
 */
error_type_t http_resource_get_sock_addr( http_request_t request, 
                                          struct sockaddr_storage *addr );

#endif //HAVE_NETWORK

/**
 *
 * \brief Allows a module to save off a void* with the request - which
 * it can request at a later time
 *
 * \param[in] request Handle to current http request 
 *  
 * \param[in] data Handle to data to save with the current HTTP 
 *       request 
 *  
 * \retval OK upon success, FAIL otherwise 
 * 
 * This field MUST be set to NULL prior to calling 
 * http_resource_send_complete or in the callback for an error 
 * condition.  If the value is not NULL when these conditions occur 
 * then it is assumed to be a memory leak and the code will ASSERT 
 * 
 */
error_type_t http_resource_set_data_ptr( http_request_t request, void *data );


/**
 *
 * \brief Allows the module to retrieve a previously saved void*
 * pointer
 *
 * \param[in] request Handle to current http request 
 *  
 * \param[out] data Handle to data saved with call to 
 *       http_resource_set_data_ptr
 *  
 * \retval OK upon success, FAIL otherwise
 * 
 */
error_type_t http_resource_get_data_ptr( http_request_t request, void **data );

/**
 * 
 * \brief Allows modules to forcefully close an HTTP connection.  No 
 *        further HTTP API calls will be allowed using this request
 *        handle.
 * 
 * \param[in] request Handle to http request to reset
 * 
 * \retval OK upon success, FAIL otherwise
 */
error_type_t http_resource_reset_connection( http_request_t request );

// manipulate TCP buffer sizes on-the-fly
// e.g. increase recv window size before streaming bulk rx data for an IPP PrintJob operation
// NOTE: treck does not support *decreasing* the TCP recv window size on-the-fly 
error_type_t http_get_tcp_recv_buf_size(http_request_t request, uint32_t *size);
error_type_t http_set_tcp_recv_buf_size(http_request_t request, uint32_t size);
error_type_t http_get_tcp_send_buf_size(http_request_t request, uint32_t *size);
error_type_t http_set_tcp_send_buf_size(http_request_t request, uint32_t size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INC_HTTP_API_H
