/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef httpParse_h
#define httpParse_h

#include <stdbool.h>
#include <stdint.h>

#include "pthread.h"
#include "oid_api.h"
#include "html.h"
#include "logger.h"
#include "error_types.h"
#include "http_api.h"
#include "http_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// General debug trace commands - DBG_PRFX defined in individual file
#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_WARN(...) DBG_PRINTF(LOG_WARNING, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_MSG2(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__) // not an error but high importance
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)

#define HTTP_SUBMOD_HTTP    LOGGER_SUBMODULE_BIT( 26 ) 
#define HTTP_SUBMOD_METHODS LOGGER_SUBMODULE_BIT( 25 )
#define HTTP_SUBMOD_SSI     LOGGER_SUBMODULE_BIT( 24 )
#define HTTP_SUBMOD_OID     LOGGER_SUBMODULE_BIT( 23 )
#define HTTP_SUBMOD_USB     LOGGER_SUBMODULE_BIT( 22 )
#define HTTP_SUBMOD_NET     LOGGER_SUBMODULE_BIT( 21 )

/* error_type_t definitions for HTTP module */

/* Negative status codes */
#define HTTP_PARSING_INCOMPLETE (error_type_t)-100
#define HTTP_PARSING_JUNK       (error_type_t)-101
#define HTTP_MEMORY_OUT         (error_type_t)-102
#define HTTP_CONTEXT_FOUND      (error_type_t)-103
#define HTTP_NO_CONTEXT         (error_type_t)-104

/* Positive status codes */
#define HTTP_DATA_TO_WRITE      (error_type_t)200

/* Use this as the index of an oid if the http context handle should be passed to the oid */
#define HTTP_USE_REQUEST_HANDLE_AS_INDEX 65535  

/* Port used for SSL connection */
#define HTTPS_SERVER_PORT 443

/* Special index used in xml parsing that indicates we want to 
   dynamically select the index based off last oid processed.  
   If the last oid in the list matches the current oid, then take
   the last index and add one to it for the current oid.  
   The value was chosen randomly to be a high value that will not be used
   as a valid index to an oid.
*/
#define DYNAMIC_INDEX_SELECTION 0xFFFFFEDD

/* Special oid value used to indicate that the current XML value
   Should be added to a previously created oid link.
*/
#define UPDATE_OID_LINK         0xFFFFFEEE

#define GET_REQUEST     "GET"
#define POST_REQUEST    "POST"
#define PUT_REQUEST     "PUT"
#define DELETE_REQUEST  "DELETE"
#define HEAD_REQUEST    "HEAD"

#define HTTP_FINAL_CHUNK "\r\n0000\r\n\r\n"
#define HTTP_STATUS_100 "HTTP/1.1 100 Continue\r\n"
#define HTTP_STATUS_100_COMPLETE HTTP_STATUS_100"\r\n"
#define HTTP_STATUS_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_STATUS_201 "HTTP/1.1 201 Created\r\n"
#define HTTP_STATUS_202 "HTTP/1.1 202 Accepted\r\n"
#define HTTP_STATUS_204 "HTTP/1.1 204 No Content\r\n"
#define HTTP_STATUS_301 "HTTP/1.1 301 Moved Permanently\r\n"
#define HTTP_STATUS_304 "HTTP/1.1 304 Not Modified\r\n"
#define HTTP_STATUS_307 "HTTP/1.1 307 Temporary Redirect\r\n"
#define HTTP_STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define HTTP_STATUS_401 "HTTP/1.1 401 Authorization Required\r\n"
#define HTTP_STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define HTTP_STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_STATUS_405 "HTTP/1.1 405 Method Not Allowed\r\n"
#define HTTP_STATUS_409 "HTTP/1.1 409 Conflict\r\n"
#define HTTP_STATUS_426 "HTTP/1.1 426 Upgrade Required\r\n"
#define HTTP_STATUS_500 "HTTP/1.1 500 Internal Server Error\r\n"
#define HTTP_STATUS_503 "HTTP/1.1 503 Service Unavailable\r\n"

#define HTTP_CONNECTION_CLOSE   "Connection: close"
#define HTTP_CONTENT_TYPE       "Content-Type: "
#define HTTP_CONTENT_LENGTH     "Content-Length:"
#define HTTP_REQUIRED_FIELDS    "Server: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\nConnection: keep-alive\r\n"
#define HTTP_TRANSFER_CHUNKED   "Transfer-Encoding: chunked\r\n"
#define HTTP_TRANSFER_ENCODING  "Transfer-Encoding:"

#define HTTP_X_HTTP_METHOD_OVERRIDE "X-HTTP-METHOD-OVERRIDE"
#define HTTP_OK_1               HTTP_STATUS_200"Content-Type: "
#define HTTP_OK_2               "\r\n"HTTP_REQUIRED_FIELDS""HTTP_TRANSFER_CHUNKED
#define HTTP_OK_FILE_LENGTH_1   HTTP_STATUS_200"Content-Length: "
#define HTTP_OK_FILE_LENGTH_2   "\r\nExpires: Thu, 01 Jan 1970 00:00:00 GMT\r\nContent-Type: "
#define HTTP_OK_FILE_LENGTH_3   "\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_OK_SIMPLE          HTTP_STATUS_200"Content-Length:0\r\nExpires: Thu, 01 Jan 1970 00:00:00 GMT\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_CREATED            HTTP_STATUS_201"Content-Length:0\r\nExpires: Thu, 01 Jan 1970 00:00:00 GMT\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\nLocation: http://"
#define HTTP_ACCEPTED           HTTP_STATUS_202"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_NO_CONTENT         HTTP_STATUS_204"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_MOVED_PERM_1       HTTP_STATUS_301"Content-Length:0\r\nLocation: "
#define HTTP_MOVED_PERM_2       "\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_NOT_MODIFIED       HTTP_STATUS_304"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_MOVED_TEMP_1       HTTP_STATUS_307"Content-Length:0\r\nLocation: "
#define HTTP_MOVED_TEMP_2       "\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nCache-Control: no-cache\r\n\r\n"
#define HTTP_BAD_REQUEST        HTTP_STATUS_400"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_UNAUTHORIZED_1     HTTP_STATUS_401"WWW-Authenticate: Basic realm=\""
#define HTTP_UNAUTHORIZED_2     "\"\r\nContent-Length: 0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_FORBIDDEN          HTTP_STATUS_403"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_NOT_FOUND          HTTP_STATUS_404"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_METHOD_NOT_ALLOWED HTTP_STATUS_405"Server: "HTTP_SERVER_DESCRIPTION"\r\nContent-Length:0\r\nAllow: "
#define HTTP_CONFLICT           HTTP_STATUS_409"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_UPGRADE            HTTP_STATUS_426"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\nUpgrade: TLS/1.0, HTTP/1.1\r\nConnection: Upgrade\r\n\r\n"
#define HTTP_INTRNL_ERROR       HTTP_STATUS_500"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"
#define HTTP_SERVICE_UNAVAIL    HTTP_STATUS_503"Content-Length:0\r\nServer: "HTTP_SERVER_DESCRIPTION"\r\n\r\n"

#define ENDOFLINE           "\r\n"
#define ENDOFHEADER         "\r\n\r\n"
#define IPV4_INDEX 3

#define LENGTH_OF_NULL          1
#define LENGTH_OF_char          3
#define LENGTH_OF_uint16_t      5
#define LENGTH_OF_uint32_t      10

/**
 *
 * \warning The order here must match the enumeration of
 * content types in http_api.h
 *
 */
static const char * const HttpdContentTypes[ ] =
{
    "text/html",
    "text/xml",
    "text/plain",
    "image/jpeg",
    "image/gif",
    "image/png",
    "application/soap+xml",
    "application/octet-stream",
    "text/css",
    "text/javascript",
    "application/ipp",
    "application/octet-stream", // Unknown case - map to binary data
};
#define NUM_HTTP_CONTENT_TYPES ( sizeof( HttpdContentTypes ) / sizeof( const char * ) )

typedef enum
{
    e_http_connection_network,
    e_http_connection_usb,
} http_ioconn_t;

typedef struct oidLink_s
{
    uint32_t      oid;            // Oid to save
    uint32_t    index;          // Index in oid
    char        *oid_val_str;   // String value for oid
    uint32_t    oid_val_int;    // Int value for oid
    struct oidLink_s* next; // Next oid for this request
} oidLink;

typedef struct PageDataLink_s
{
    char *     data;        // Start of the data
    uint32_t   atByte;      // Index of next byte to send
    uint32_t   size;        // Number of bytes in this block
    uint32_t   buff_size;   // Size of buffer
    struct PageDataLink_s *next; // Next link in chain
} PageDataLink;


typedef enum
{
    e_http_state_initialized,
    e_http_state_header_received,
    e_http_state_body_received,
    e_http_state_status_code_added,
    e_http_state_type_added,
    e_http_state_header_complete,
    e_http_state_body_complete,
    e_http_state_header_sent,
    e_http_state_body_sent,
    e_http_state_done,
    e_http_state_resetting,
} http_state_t;


typedef struct http_handle_s HttpPageContext;


typedef struct
{
    uint16_t network_port;      // Which network port to simulate (0 for "all ports")
} http_usb_device_info_t;


/**
 *
 * \brief A callback for a blocking send.
 *
 * The HTTP Module may wish at times to do a blocking send over the pipe
 * the current context is on.  This callback will be passed into
 * the http_context_create() function so that the blocking send may occur
 * at any time.
 *
 */
typedef error_type_t (*http_blocking_send_cb_t)( HttpPageContext *context,
                                                 char *buffer,
                                                 uint32_t size,
                                                 bool free_buffer );

/**
 *
 * \brief A callback for a non-blocking recv.
 *
 * The HTTP Module may wish at times to do a blocking recv over the 
 * pipe the current context is on.  This callback will be passed into 
 * the http_context_create() function so that the blocking recv may 
 * occur at any time. 
 *
 */
typedef int32_t (*http_blocking_recv_cb_t)( HttpPageContext *context,
                                            char *buffer,
                                            uint32_t size,
                                            uint32_t *timeout );



struct http_handle_s 
{
    http_request_t              id;                    // This is a unique ID per context
    http_state_t                state;                 // Overall state of connection
    http_resource_event_cb_t    request_cb;            // Is this request handled by an outside module?  If so, add callback
    http_resource_event_flag_cb_t event_cb;            // This request handled by an outside module via read/write calls
    http_blocking_send_cb_t     blocking_send_cb;      // A callback for doing a blocking send
    http_blocking_recv_cb_t     blocking_recv_cb;      // A callback for doing a blocking recv
    uint8_t                     num_buffers;           // Number of buffers send to external module that need to be freed before request is terminated
    char *                      pagePtr;               // Pointer to page data that we are currently viewing
    uint32_t                    pageSize;              // Size of page we are currently viewing
                                                       // Also used for incoming chunked request to store current chunk size
    uint32_t                    atByte;                // How many bytes we've already read
                                                       // Also used for incoming chunked requests to store how much of current chunk has been read
    bool                        parseSSI;              // If true, this page will be searched for SSI tags. (HTML and XML files)
    uint32_t                    outputEnabled;         // SSI can disable sections of the file.
    char                        *request_header;       // The header for the request being serviced
    uint32_t                    request_header_length; // Length of the request header, not including irrelevant characters
    http_request_cgi_args_t     *cgi_args;             // Parsed cgi arguments from URL and request body
    char                        *request_body;         // The body for the request being serviced
    uint32_t                    request_body_length;   // Length of the request body, not including irrelevant characters
    char                        *requested_uri;        // Requested URI - extracted from header for ease of use
    uint32_t                    content_length;        // Length of request -- if length =CHUNKED_ENCODING(0xFFFFFFFF) indicates chunked encoding
    uint32_t                    request_data_read;     // Number of bytes of the incoming request that have already been read 
    char                        *xmlTag;               // Flattened view of current place in xml file ie: <doc><test><Hello> --> doc|test|Hello
    oidLink                     *oidsToSave;           // Linked list of oid values to set, upon successful parsing of entire POST/PUT request
    http_ioconn_t               connType;              // Type of connection
    void                        *deviceInfo;           // Extra info to help us get this data to the right place (socket file descriptor for network)
    char                        *loc_of_content_length;// Ptr to location where content length needs to be inserted when it is known
    PageDataLink                *dataChain;            // The actual data waiting to be sent
    sem_t                       data_chain_semaphore;  // a Semaphore used to indicate that a dataChain is being altered
    sem_t                       resource_done_semaphore;  // a Semaphore used to indicate that a resource_complete is done 
    uint32_t                    lastAccess;            // Timestamp of last time connnection was accessed
    http_response_type_t        contextTypeIndex;      // The file type to return to the client
    void *                      tmpData;               // Void pointer to store any temporary data during parsing of requests
    HttpPageContext             *nextContext;          // Used to build linked lists of contexts
    int                         port;                  // Save off port used for current request
    uint8_t                     *partial_chunk;        // A partial set of chunked encoded data
    uint32_t                    partial_chunk_length;  // The length of the partial_chunk buffer
    uint32_t                    response_content_length;// Length of response -- if length = CHUNKED_ENCODING indicates chunked encoding
    uint32_t                    response_length;       // Number of bytes of response body sent thus far
};

typedef enum
{
    XML_OIDS,
    XML_SUBSCRIPTIONS,
} XML_HANDLER;


/**
 *
 * \brief Creates and adds an HTTP context to the HTTP internal modules.
 * The new context is returned via the pointer in the parameter list for use
 * with other HTTP API functions.
 *
 * \param new_context A pointer to a memory location large enough to hold
 * a pointer to the created context data structure.
 *
 * \param request_buffer A buffer containing the request to associate with
 * the new context.  The buffer does NOT have to be a full request.  Verify
 * that the request is complete by checking the return error_type_t
 *
 * \param request_buffer_len_in_bytes The size of the request_buffer in bytes
 *
 * \param actual_request_bytes_parsed The number of valid bytes in the buffer
 * passed.  This value should be useful to track pipe I/O reads and determining
 * if the pipe reads have started reading another request
 *
 * \param io_type The IO type, one of the HTTP_IO_TYPE enums
 *
 * \param device_info A caller-specific data.  Users of the HTTP module may 
 * use this value to save data and refer to it later when processing http
 * data.
 *  
 * \param blocking_send_cb Callback for blocking send call 
 *  
 * \param blocking_recv_cb Callback for blocking recv call
 *  
 * \return
 * - OK upon successful parsing of the request buffer
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The request buffer contains junk data and should be
 * destroyed using http_context_destroy()
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 *
 */
error_type_t http_context_create( 
    HttpPageContext ** new_context,
    char * request_buffer,
    uint32_t request_buffer_len_in_bytes,
    uint32_t * actual_request_bytes_parsed,
    http_ioconn_t io_type,
    void * device_info,
    http_blocking_send_cb_t blocking_send_cb,
    http_blocking_recv_cb_t blocking_recv_cb
    );

/**
 *
 * \brief Destroys a current context, freeing all associated memory
 *
 * \param context A valid http context 
 *  
 * \param reset_socket On network connection this will force a 
 *        reset on the socket instead of the normal close
 *
 * \warning This function MUST be called after error or completion of a call to
 * http_context_create() in order to prevent resource leaks.
 *
 */
void http_context_destroy( HttpPageContext * context, bool reset_socket );


/**
 *
 * \brief Adds data to an existing HTTP context.
 *
 * \param request_buffer A buffer containing more of the request to associate
 * with the new context.  The buffer does NOT have to be a full request.  Verify
 * that the request is complete by checking the return error_type_t
 *
 * \param request_buffer_len_in_bytes The size of the request_buffer in bytes
 *
 * \param actual_request_bytes_parsed The number of valid bytes in the buffer
 * passed.  This value should be useful to track pipe I/O reads and determining
 * if the pipe reads have started reading another request
 *
 * \return
 * - OK upon successful parsing of the request buffer
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The request buffer contains junk data and should be
 * destroyed using http_context_destroy()
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 *
 */
error_type_t http_context_add_data( HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_request_bytes_parsed );


/**
 *
 * \brief Processes an HTTP request.  All the data must be added to the context
 *
 * \param context A valid http context
 *
 * \return
 * - OK upon successful parsing of the context, also indicates this is a 
 *   registered resource.
 *   If this is a registered resource you MUST call http_context_wait_for_done_state()
 *   or otherwise check for done and then write the response buffer data chain
 *   out before the context is destroyed.
 *
 * - HTTP_DATA_TO_WRITE indicates the data chain is valid and has
 *   data that needs to be written out the IO
 *
 * - FAIL if this is not a registered resource, and there is no 
 *   data to write then this is an error condition.
 *   ALL HTTP transactions must have data to write as a response
 *
 */
error_type_t http_context_process( HttpPageContext * context );


/** 
 *
 * \brief Block waiting for the resource to complete any blocking reads/writes
 * it will need to do.  This function should be called when http_context_process()
 * returns OK to wait for a registered resource to complete.
 *
 * \param context A valid http context
 *
 * \return 
 * - OK upon success when there is no further data to write
 *  
 * - HTTP_DATA_TO_WRITE indicates there is data to write in the datachain
 *
 */
error_type_t http_context_wait_for_done_state( HttpPageContext * context );


/**
 *
 * \brief Check for the resource to be done completing any blocking
 * reads/writes. When done the IO parser can destroy the context.
 * This is a non-blocking call.
 *
 * \param context A valid http context
 *
 * \return OK if the context is complete, FAIL otherwise
 *
 */
error_type_t http_context_check_for_done_state( HttpPageContext * context );


/** 
 * \brief Get pointer to the first context of a specific type 
 * 
 * \param connection_type The type of connection's context to get
 *
 * \param context A memory location to be filled with the first context of
 * the type specified
 *
 * \return error_type_t:
 * OK upon success, context will be filled with a valid pointer
 *
 * HTTP_NO_CONTEXT if no context of the type specified was found
 *
 * This function is to be used as the first step in iterating over a list of
 * contexts for a specific type.
 * 
 * 
 **/
error_type_t http_context_get_first( http_ioconn_t connection_type, HttpPageContext ** context );


/** 
 * \brief Get pointer to the next context of a specific type 
 * 
 * \param context The current context caller is iterating over
 * This must be a context of the same type and returned through a call to
 * http_context_get_first() or this same function
 *
 * \param connection_type The type of connection's context to get
 *
 * \param next_context A memory location to be filled with the first context of
 * the type specified
 *
 * \return error_type_t:
 * OK upon success, context will be filled with a valid pointer
 *
 * HTTP_NO_CONTEXT if no more contexts of the type specified was found
 *
 * This function is to be used as the iteration step in iterating over a list of
 * contexts for a specific type.  First call http_context_get_first() in order
 * to get the initial iterator context.
 * 
 * 
 **/
error_type_t http_context_get_next( HttpPageContext * context,
        http_ioconn_t connection_type,
        HttpPageContext ** next_context );


/** 
 *
 * \brief Fill context with data to send
 *
 * \param context - current context, contains pointers to file
 * to send
 * 
 * \param add_last_chunk - When finish adding data to context, 
 * do you want the last chunk header added to context 
 *  
 * \param included_file - determines if the final content length 
 *        shoudl be added when finished or not
 *
 * \return Returns 1 if context complete, 0 if more data to send
 *
 * Given an active web context, fills the context with data that
 * should be sent back to the requester.  Limits itself to
 * prevent too much memory from being allocated across the
 * system.
 * 
 **/
int32_t fillContextWithData( HttpPageContext *context, bool add_last_chunk, bool included_file );


error_type_t http_add_request_to_context( HttpPageContext *context, char *buffer, uint32_t bytes_read );
void resetContextForRead( HttpPageContext *context );
void http_reset_connections( http_ioconn_t connection_type );

char* decode_base_64_string( char *str );
void addDataToHttpContext( char *data, uint32_t size, bool needsToBeFreed, HttpPageContext *context, bool addChunkHeader );
void addMethodNotAllowed( HttpPageContext* context );
char* skipWhiteSpace( char* str );
bool getContextInfoForFilename( char *filename, HttpPageContext *context, bool includedFile );
bool setPageValues( char *filename, HttpPageContext *context, bool includedFile );
http_response_type_t getContextTypeIndex( char* filename );
bool compare_tag_with_optional_namespace( char *cur_loc_user_tag, char *cur_loc_comparison_tag );
bool saveOidValues( HttpPageContext* context );
error_type_t handleEwsPost( HttpPageContext* context );
HttpPageContext *getHttpContextFromId( http_request_t id );
void deleteFirstDataElement ( HttpPageContext *context );
char *xml_encode(char *str);
void urlDecode( char* original, char* final, uint32_t bufSize );

// In http_oid_lut.c
void ConvertXmlTagtoOid(char * tagName, char* value, HttpPageContext* oidsToSave);
void checkXmlTagtoCreateOids(char * tagName, HttpPageContext* context);
void checkXmlTagsWithoutValues(char * tagName, HttpPageContext* context);
oidLink *http_oid_lut_get_last_oid( HttpPageContext *context );
bool addOidLinkToContext( HttpPageContext* context, oid_t oid, uint32_t index, char *val );

void lock_contexts( void );
void unlock_contexts( void );

#ifdef __cplusplus
}
#endif

#endif // httpParse_h

