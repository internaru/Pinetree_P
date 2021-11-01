/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_slp_config.c
 *
 * \brief This file implements the public configuration API for
 * the SLP network application.
 * 
 **/

#include <stdlib.h>
#include <platform_api.h>
#include <lassert.h>
#include <string.h>
 
#ifdef __linux__
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <rpc/types.h>
#else
#include <net_iface_api.h>
#include <net_iface_vars.h>
#endif 
//#include <net_iface_api.h>
//#include <net_iface_vars.h>
#include "dprintf.h"
#include "logger.h"
#include "memAPI.h"
#include "ioutils.h"
#include "slp.h"
#include "net_slp_config.h"
#include "net_api.h"

#define DBG_PRFX "SLP_CFG: "
#define DBG_ON
#define DBG_ERR_ON
#define DBG_VERBOSE_ON

#ifdef DBG_ON
#define SLP_DBG(...) DPRINTF((DBG_SOFT|DBG_OUTPUT),(DBG_PRFX __VA_ARGS__));
#else // !DBG_ON
#define SLP_DBG(...)
#endif // DBG_ON
#ifdef DBG_ERR_ON
#define SLP_DBG_ERR(...) DPRINTF((DBG_ERROR|DBG_OUTPUT),(DBG_PRFX __VA_ARGS__));
#else // !DBG_ERR_ON
#define SLP_DBG_ERR(...)
#endif // DBG_ERR_ON
#ifdef DBG_VERBOSE_ON
#define SLP_DBG_VERBOSE(...) DPRINTF((DBG_LOUD|DBG_OUTPUT),(DBG_PRFX __VA_ARGS__));
#else // !DBG_VERBOSE_ON
#define SLP_DBG_VERBOSE(...)
#endif // DBG_VERBOSE_ON

//#define HAVE_SLP_ANNOUNCE 1

// Service name takes the following form: service:printer:raw-tcp://<hostname>.<domainname>:<port>
// or service:printer:lpr:<hostname>.<domainname>/<queuename>
//#define MAX_SERVICE_NAME_LENGTH     ( 60 + NET_IFACE_DOMAINNAME_SIZE + NET_IFACE_HOSTNAME_SIZE )   
#define MAX_DOMAINNAME_SIZE       64 ///< RFC 1034/1035 max domain label size of 63 plus terminating null char.  
#define MAX_SERVICE_NAME_LENGTH     ( 60 + MAX_DOMAINNAME_SIZE + MAXHOSTNAMELEN ) 

// TODO add support for element printer-xri-supported - which is required when using the service:printer service name
#define SLP_TEMPLATE_ATTR_STRING   "(printer-name=%s),(ieee-1284-device-id=%s),(x-mac-addr=%02X%02X%02X%02X%02X%02X),(x-ip-addr=%s)"

#define RAW_DEST_PORT 9100

#define DEF_SLP_TTL     4
#define SLPV1_ADDR      "224.0.1.22"
#define SLPV2_IPV6_ADDR "FF02::116"
#define SLPV2_IPV4_ADDR "239.255.255.253"
#define SLP_PORT_NUM    427
#define SLP_ANNOUNCE    false

void net_slp_config( net_slp_config_t *config )
{
    config->ttl                 = DEF_SLP_TTL;
    config->v1_address          = inet_addr( SLPV1_ADDR );
    config->v2_ipv4_address     = inet_addr( SLPV2_IPV4_ADDR );
    #ifdef HAVE_IPV6
//    inet_pton ( AF_INET6, SLPV2_IPV6_ADDR, ( void* ) &config->v2_ipv6_address.addr.ipv6.sin6_addr.ip6Addr );
    inet_pton ( AF_INET6, SLPV2_IPV6_ADDR, ( void* ) &config->v2_ipv6_address );
    #endif // HAVE_IPV6
    config->port_number         = SLP_PORT_NUM;
    config->max_attr_list_length= 1024;
    config->auto_announce       = SLP_ANNOUNCE;
}


// This function looks up the 1284 string and replaces any
// of the following characters (),\!<=>~
// Must free the returned string
char *net_slp_escape_ieee_1284_string( void )
{
    char *ieee_1284_string;
    char *ieee_1284_string_escaped;
    uint32_t ieee_1284_string_length;
    uint32_t i;
    uint32_t j;

    platvars_get_1284str( &ieee_1284_string );
    ieee_1284_string += 2;  //  skip lenghth stuff
    ieee_1284_string_length = strlen( ieee_1284_string );
    for ( i = 0; i < strlen( ieee_1284_string ); i++ )
    {
        if ( ( ieee_1284_string[ i ] == '(' ) ||
             ( ieee_1284_string[ i ] == ')' ) ||
             ( ieee_1284_string[ i ] == ',' ) ||
             ( ieee_1284_string[ i ] == '\\' ) ||
             ( ieee_1284_string[ i ] == '!' ) ||
             ( ieee_1284_string[ i ] == '<' ) ||
             ( ieee_1284_string[ i ] == '=' ) ||
             ( ieee_1284_string[ i ] == '>' ) ||
             ( ieee_1284_string[ i ] == '~' ) )
        {
            ieee_1284_string_length += 2;
        }
    }

    ieee_1284_string_length++; // For null character

    ieee_1284_string_escaped = ( char * )MEM_MALLOC( ieee_1284_string_length );
    ASSERT( ieee_1284_string_escaped );
    memset( ieee_1284_string_escaped, 0x00, ieee_1284_string_length );
    // i is for ieee_1284_string, j is for the escaped equivalent
    for ( i = 0, j = 0; i < strlen( ieee_1284_string ); i++ )
    {
        if ( ieee_1284_string[ i ] == '(' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '2';
            ieee_1284_string_escaped[ j++ ] = '8';
        }
        else if ( ieee_1284_string[ i ] == ')' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '2';
            ieee_1284_string_escaped[ j++ ] = '9';
        }
        else if ( ieee_1284_string[ i ] == ',' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '2';
            ieee_1284_string_escaped[ j++ ] = 'C';
        }
        else if ( ieee_1284_string[ i ] == '\\' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '5';
            ieee_1284_string_escaped[ j++ ] = 'C';
        }
        else if ( ieee_1284_string[ i ] == '!' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '2';
            ieee_1284_string_escaped[ j++ ] = '1';
        }
        else if ( ieee_1284_string[ i ] == '<' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '3';
            ieee_1284_string_escaped[ j++ ] = 'C';
        }
        else if ( ieee_1284_string[ i ] == '=' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '3';
            ieee_1284_string_escaped[ j++ ] = 'D';
        }
        else if ( ieee_1284_string[ i ] == '>' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '3';
            ieee_1284_string_escaped[ j++ ] = 'E';
        }
        else if ( ieee_1284_string[ i ] == '~' )
        {
            ieee_1284_string_escaped[ j++ ] = '\\';
            ieee_1284_string_escaped[ j++ ] = '7';
            ieee_1284_string_escaped[ j++ ] = 'E';
        }
        else
        {
            ieee_1284_string_escaped[ j++ ] = ieee_1284_string[ i ];
        }
    }
    return ieee_1284_string_escaped;
}

/**
 * \brief Build up the attribute list for the SLP response
 *
 * \param SLP_ATTR_REPLY structure where response should be
 * placed
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t net_slp_add_attr_list( char *attr_list, slp_version_t version, /*struct sockaddr_storage**/ struct in6_addr* sockaddr )
{
  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  /* get IP address thats attached to "eth0" */
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

  /*get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;

 /* display result */
// printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    #ifdef HAVE_IPV6
    char ip_address_string[INET6_ADDRSTRLEN];
//    net_iface_ipv6_addr_t ipv6_addr;
    struct in6_addr ipv6_addr;
    #else
    char ip_address_string[INET_ADDRSTRLEN];
    #endif // HAVE_IPV6t
//    uint8_t phys_addr[TM_ETHERNET_PHY_ADDR_LEN];
    uint8_t phys_addr[IFHWADDRLEN];
//    char hostname[ NET_IFACE_HOSTNAME_SIZE ];
    char hostname[ MAXHOSTNAMELEN ];
    uint32_t ip;
    char *ieee_1284_string;

    // TODO must account for multiple simultaneous interfaces
//    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    if ( version == SLP_VERSION_1 )
    {
//        if ( net_iface_get_var_ipv4_addr( if_index, &ip ) != OK )
        if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
        {
            ip = 0;
        }
        // inet_ntop( AF_INET, (void *)&ip, ip_address_string, INET_ADDRSTRLEN );
        inet_ntop( AF_INET, (void *)&ifr.ifr_addr, ip_address_string, INET_ADDRSTRLEN );
        //inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)
    }
    else if ( version == SLP_VERSION_2 )
    {

        #ifdef HAVE_IPV6
        //if ( sockaddr->addr.ipv6.sin6_addr.ip6Addr.ip6U32[ 0 ] == 0 )
        if ( sockaddr->s6_addr32[ 0 ] == 0 )
        {
//            if ( net_iface_get_var_ipv4_addr( if_index, &ip ) != OK )
            if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
            {
                ip = 0;
            }
            //inet_ntop( AF_INET, (void *)&ip, ip_address_string, INET_ADDRSTRLEN );
            inet_ntop( AF_INET, (void *)&ifr.ifr_addr, ip_address_string, INET_ADDRSTRLEN );
        }
        else
        {
            /* get an IPv6 IP address */
            ifr.ifr_addr.sa_family = AF_INET6;
    //        if ( net_iface_get_var_ipv6_link_local_addr(if_index, &ipv6_addr, NULL) != OK )
            if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
            {
     //           memset( &ipv6_addr, 0x00, sizeof( net_iface_ipv6_addr_t ) );
                memset ( &ipv6_addr, 0x00, sizeof( /*struct in6_addr*/ ipv6_addr ));
            }
            //inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), ip_address_string, INET6_ADDRSTRLEN);
            inet_ntop( AF_INET6, (void *)&ifr.ifr_addr, ip_address_string, INET6_ADDRSTRLEN );
        }
        #else
//        if ( net_iface_get_var_ipv4_addr( if_index, &ip ) != OK )
        if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
        {
            ip = 0;
        }
 //       inet_ntop( AF_INET, (void *)&ip, ip_address_string, INET_ADDRSTRLEN );
        inet_ntop( AF_INET, (void *)&ifr.ifr_addr, ip_address_string, INET_ADDRSTRLEN );
        #endif
    }
    else
    {
        XASSERT( 0, version );
    }

//    struct sockaddr_in sa;
    // if ( net_iface_get_var_hostname( if_index, hostname, NET_IFACE_HOSTNAME_SIZE ) != OK )
    if (getnameinfo(&ifr.ifr_addr, sizeof(ifr.ifr_addr), hostname, sizeof(hostname),
                       NULL, 0, NI_NAMEREQD) != 0)
    {
        hostname[ 0 ] = 0;
    }

//    net_iface_get_var_phys_addr(phys_addr, TM_ETHERNET_PHY_ADDR_LEN);
    memcpy(phys_addr, ifr.ifr_hwaddr.sa_data, IFHWADDRLEN);


    ieee_1284_string = net_slp_escape_ieee_1284_string( );

    minSprintf( attr_list, SLP_TEMPLATE_ATTR_STRING, 
                hostname, 
                ieee_1284_string,
                phys_addr[0], phys_addr[1], phys_addr[2], phys_addr[3], phys_addr[4], phys_addr[5], 
                ip_address_string );

    MEM_FREE_AND_NULL( ieee_1284_string );
    close(fd);

    return OK;
}



/**
 * \brief Check if SLP request matches our search criteria
 *
 * \param char* pointer to incoming data packet
 *
 * \return 0 = no match else = match
 *
 */
uint8_t net_slp_match_url ( char *data )
{
    uint16_t length;
    char *tmp_data;
    char *service_name;
    char *host_name;
    char *domain_name;
    uint8_t match = 0;
    uint32_t reset_location = 0;


    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

   /* get IP address thats attached to "eth0" */
   strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

   /*get an IPv4 IP address */
   ifr.ifr_addr.sa_family = AF_INET;

    #ifdef HAVE_IPV6
    char ip_address_string_v6[INET6_ADDRSTRLEN];
//    net_iface_ipv6_addr_t ipv6_addr;
    struct in6_addr ipv6_addr;
    #endif // HAVE_IPV6
    char ip_address_string_v4[INET_ADDRSTRLEN];
    uint32_t ip;

    service_name = ( char * )MEM_MALLOC( MAX_SERVICE_NAME_LENGTH );
    ASSERT( service_name );

//    host_name = ( char * )MEM_MALLOC( NET_IFACE_HOSTNAME_SIZE );
    host_name = ( char * )MEM_MALLOC( MAXHOSTNAMELEN );
    ASSERT( host_name );

    domain_name = ( char * )MEM_MALLOC( MAX_DOMAINNAME_SIZE );
    ASSERT( domain_name );

    // TODO must account for multiple simultaneous interfaces
//    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

//   if ( net_iface_get_var_ipv4_addr( if_index, &ip ) != OK )
    if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
    {
        ip = 0;
    }
 //   inet_ntop( AF_INET, (void *)&ip, ip_address_string_v4, INET_ADDRSTRLEN );
    inet_ntop( AF_INET, (void *)&ifr.ifr_addr, ip_address_string_v4, INET_ADDRSTRLEN );

    #ifdef HAVE_IPV6
//    if ( net_iface_get_var_ipv6_link_local_addr(if_index, &ipv6_addr, NULL) != OK )
    if(ioctl(fd, SIOCGIFADDR, &ifr) == -1)
    {
        memset( &ipv6_addr, 0x00, sizeof( /*net_iface_ipv6_addr_t*/ ipv6_addr) );
    }
    //inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), ip_address_string_v6, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, (void *)(&ifr.ifr_addr), ip_address_string_v6, INET6_ADDRSTRLEN);
    #endif // HAVE_IPV6

    tmp_data = data; 
    
    // exclusion list length
    length = cpu_to_be16( *((uint16_t *) tmp_data) );
    tmp_data += (length + 2); // skip exclusion list and length bytes

    // URL length
    length = cpu_to_be16( *((uint16_t *) tmp_data) );
    tmp_data += 2;

    // Is the URL one of the ones that we support?
    strcpy( service_name, "service:" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

    strcat( service_name, "printer" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

    // Check for raw-tcp printers
    reset_location = strlen( service_name );
    strcat( service_name, ":raw-tcp" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

//    if ( net_iface_get_var_hostname( if_index, host_name, NET_IFACE_HOSTNAME_SIZE ) == OK )
    if (getnameinfo(&ifr.ifr_addr, sizeof(ifr.ifr_addr), host_name, sizeof(host_name),
                    NULL, 0, NI_NAMEREQD) == 0)
    {
        minSprintf( service_name, "%s://%s:%d", service_name, host_name, RAW_DEST_PORT );
        if ( strcasecmp( service_name, tmp_data ) == 0 )
        {
            match = 1;
        }

//        if ( net_iface_get_var_domain( if_index, domain_name, NET_IFACE_DOMAINNAME_SIZE ) == OK )
        if( getdomainname(domain_name, MAX_DOMAINNAME_SIZE) == 0)
        {
            minSprintf( service_name, "service:printer:raw-tcp://%s.%s:%d", host_name, domain_name, RAW_DEST_PORT );
            if ( strcasecmp( service_name, tmp_data ) == 0 )
            {
                match = 1;
            }
        }
    }

    minSprintf( service_name, "service:printer:raw-tcp://%s:%d", ip_address_string_v4, RAW_DEST_PORT );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

    #ifdef HAVE_IPV6
    minSprintf( service_name, "service:printer:raw-tcp://%s:%d", ip_address_string_v6, RAW_DEST_PORT );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }
    #endif // HAVE_IPV6

    #ifdef HAVE_LPR
    // check for lpr printers
    minSprintf( service_name, "service:printer:lpr" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

//    if ( net_iface_get_var_hostname( if_index, host_name, NET_IFACE_HOSTNAME_SIZE ) == OK )
    if (getnameinfo(&ifr.ifr_addr, sizeof(ifr.ifr_addr), host_name, sizeof(host_name),
                    NULL, 0, NI_NAMEREQD) == OK)
    {
        minSprintf( service_name, "%s://%s", service_name, host_name );
        if ( strcasecmp( service_name, tmp_data ) == 0 )
        {
            match = 1;
        }
        /* TODO add support for LPR queue names
        strcat( service_name, "/auto" );
        if ( strcasecmp( service_name, tmp_data ) == 0 )
        {
            match = 1;
        }
        */

//        if ( net_iface_get_var_domain( if_index, domain_name, NET_IFACE_DOMAINNAME_SIZE ) == OK )
          if( getdomainname(domain_name, MAX_DOMAINNAME_SIZE) == OK)
        {
            minSprintf( service_name, "service:printer:lpr://%s.%s", host_name, domain_name );
            if ( strcasecmp( service_name, tmp_data ) == 0 )
            {
                match = 1;
            }
            /* TODO add support for LPR queue names
            strcat( service_name, "/auto" );
            if ( strcasecmp( service_name, tmp_data ) == 0 )
            {
                match = 1;
            }
            */
        }
    }

    minSprintf( service_name, "service:printer:lpr://%s", ip_address_string_v4 );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

    /* TODO add support for LPR queue names
    strcat( service_name, "/auto" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }
    */

    #ifdef HAVE_IPV6
    minSprintf( service_name, "service:printer:lpr://%s", ip_address_string_v6 );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }

    /* TODO add support for LPR queue names
    strcat( service_name, "/auto" );
    if ( strcasecmp( service_name, tmp_data ) == 0 )
    {
        match = 1;
    }
    */
    #endif // HAVE_IPV6
    #endif // HAVE_LPR

    MEM_FREE_AND_NULL( service_name );
    MEM_FREE_AND_NULL( host_name );
    MEM_FREE_AND_NULL(domain_name );

    return match;
}

