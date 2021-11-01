/****************************************************************************** 
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef _INC_HTTP_NETWORK_PARSER_H
#define _INC_HTTP_NETWORK_PARSER_H

#include <pthread.h>
#include <posix_ostools.h>
#include "http_config.h"
#include "error_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * \brief The HTTP Module's wired network parser initialization
 * routine.  This function should only be used internally by the HTTP
 * module and only called once during system initialization. 
 *
 */
void http_network_parser_init( void );

/**
 * \brief Add a port to the list of active listening sockets
 * 
 * \param[in] port Value for socket to listen on
 * \param[in] type Type of socket (tcp or udp)
 * 
 * \retVal error_type_t OK on success, else FAIL 
 *  
 * This function adds a new port number/type to the list of 
 * active listening sockets.  Before adding the new socket, it 
 * will first scan the open sockets to determine if it is 
 * already open. 
 *  
 */
error_type_t http_network_add_port( uint32_t port, http_net_socket_protocol_t type );

/**
 * \brief Remove port from active listening socket list
 * 
 * \param[in] port Value that socket is listening on
 * \param[in] type Type of socket (udp or tcp)
 * 
 * \retVal error_type_t OK on success, else FAIL 
 *  
 * This function will find the socket associated with a port 
 * nubmer/type and then close it since it is not longer 
 * requested.  If the socket is listed in the HTTP config 
 * sockets (default sockets for the web server) then it will 
 * not be closed since it is required for other requests. 
 *  
 */
error_type_t http_network_remove_port( uint32_t port, http_net_socket_protocol_t type );

// **********************************************************
// **********************************************************
// 
// The following functions must exist in the product or protocol
// specific code.  These functions allow custom processing at 
// various points in the web server code
// 
// **********************************************************
// **********************************************************

/** 
 * \brief A UDP packet has been received, handle any processing
 * based off this data
 * 
 * \param udp_response - This is the data recieved in the UDP
 * packet
 * 
 * \param destAddr - pointer to socket address structure that
 * recieved the UDP packet
 * 
 * This function allows for custom handling of UDP packet data.
 * When a UDP packet is recieved, this function will be called
 * from the Web Server - and custom processessing can be done
 * 
 **/
void process_udp_response( char* udp_response, struct sockaddr_storage *destAddr );


/** 
 * \brief Regularly called function to allow custom processing
 * based for the web server
 * 
 * This function is called at the beginning of the select loop
 * of the web server ( currently the delay is 1 second ).  This
 * allows for checking of data, messages... in the context of
 * the webserver thread.
 * 
 **/
void http_custom_web_server_processing( );

/** 
 * \brief Get a handle to the UDP server socket 
 *  
 * \param http_net_socket_protocol_t - tcp or udp 
 *  
 * \param port -transfort port 
 *  
 * \return int- handle to the scoket used as server
 * */ 
int get_server_handle(http_net_socket_protocol_t type, uint32_t port);


#ifdef HAVE_HTTPS
/**
 * \brief Clean up SSL information for a context
 *  
 * \param device_fd Socket handle for HTTPS
 */
void ssl_free_context( int32_t device_fd );
#endif // HAVE_HTTPS


#ifdef WIRED_NETWORK
#ifdef DEBUG
/**
 *
 * \brief Get the thread ID of the http parser; used for debug 
 *        validation checks only.
 *
 */
pthread_t http_network_parser_get_tid( void );
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif

