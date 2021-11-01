 /****************************************************************************** 
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef HTTP_METHODS_SUPPORT_H
#define HTTP_METHODS_SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif


/* message format */
#define HTTP_NET_PARSER_MQ_MSG_BUF_SIZE   2 //TX_2_ULONG

/* message queue buffer */
#define HTTP_NET_PARSER_MQ_MSG_BUF_BYTES  ( HTTP_NET_PARSER_MQ_MSG_BUF_SIZE * sizeof( unsigned long int ) )
typedef enum
{
    NET_HTTP_NETWORK_DELAY_SUBSCRITION   

} http_net_parser_msg_type_t;

typedef struct
{
    http_net_parser_msg_type_t type;
    void *data; 
} http_net_parser_msg_t;

typedef union
{
    http_net_parser_msg_t msg;
    uint8_t buf [ HTTP_NET_PARSER_MQ_MSG_BUF_BYTES ];
} http_net_parser_msg_buf_t;


// **********************************************************
//  
// These functions are required by http.c in order to 
// fulfill HTTP responses.  Each of these functions allows
// for customization for protocols running underneath the
// HTTP transactions.
// 
// **********************************************************

/** 
 * \brief Initialize protocol specific code, called once, only, during system
 * initialization
 *
 **/
void http_methods_initialize( void );

/** 
 * \brief Validates if provided url is password protected
 *
 * \return true  ==> Password Protected
 *         false ==> No password required
 *
 **/
bool http_methods_is_password_protected( char *request, char *url );

/** 
 * \brief Gets correct password for the url
 * 
 * \param[in] url - Url that was requested
 * 
 * \param[out] password - correct password for url, memory is created by caller
 * 
 * \param[in] password_len - the length of the password string buffer in bytes
 * 
 * \return OK upon success, otherwise FAIL
 *
 **/
error_type_t http_methods_get_current_password( char * url, char * password, uint32_t password_len );

/** 
 * \brief Parse and HTTP GET request and either supply requested
 * data or indicate an error
 * 
 * \param context - context with data 
 *  
 * \param is_included_file - Is this file being included from 
 *        within another file
 * 
 * \return bool - success / failure
 * 
 * Given a context that has recieved an HTTP GET request,
 * examine the filename requested and either send the file if it
 * is in our system - or return a 404 file not found error
 * 
 **/
bool parseHttpGet( HttpPageContext *context, bool is_included_file );

/** 
 * \brief parse and HTTP POST request
 * 
 * \param context - context to initialize
 * 
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP POST request, parse the data to
 * see if it contains correct formatting and a file that we know
 * about.  If successful, data is saved in the system, otherwise
 * the appropriate HTTP error is added to the context
 * 
 **/
bool parseHttpPost( HttpPageContext *context );

/** 
 * \brief Parse an HTTP PUT request
 * 
 * \param context - current context, contains HTTP request
 * 
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP PUT request, parse the data to
 * see if it contains correct formatting and a file that we know
 * about.  If successful, data is saved in the system, otherwise
 * the appropriate HTTP error is added to the context
 * 
 **/
bool parseHttpPut( HttpPageContext *context );

/** 
 * \brief Parse and handle and HTTP DELETE request
 * 
 * \param context - current context, contains request
 * 
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP DELETE request, parse the data
 * to see if it contains correct formatting and a file that we
 * know about.  If successful, the data is deleted from the
 * system, otherwise the appropriate HTTP error is added to the
 * context
 * 
 **/
bool parseHttpDelete( HttpPageContext *context );

/** 
 * \brief Parse and handle and HTTP HEAD request
 * 
 * \param context - current context, contains request
 * 
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP HEAD request, check if the file
 * exists in the system and if so return the HTTP header for
 * this file - no data.
 * 
 **/
bool parseHttpHead( HttpPageContext *context );


#ifdef __cplusplus
}
#endif

#endif // HTTP_METHODS_SUPPORT_H

