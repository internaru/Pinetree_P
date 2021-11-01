/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_slp_config.h
 *
 * \brief This file is the public configuration API for the SLP
 * network application.
 * 
 **/

#ifndef INC_NET_SLP_CONFIG_H
#define INC_NET_SLP_CONFIG_H

#include "slp.h"
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include "net_iface_api.h"
#include "net_iface_vars.h"
#endif
//#include <trsocket.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum slp_version_e
{
    SLP_VERSION_1 = 1,
    SLP_VERSION_2 = 2,
} slp_version_t;

/**
 *
 * \brief A slp configuration structure definition
 * used to configure slp
 * 
 */
typedef struct
{
    uint8_t                 ttl;
    uint32_t                v1_address;
    uint32_t                v2_ipv4_address;
//    struct sockaddr_storage v2_ipv6_address;
    struct in6_addr         v2_ipv6_address;
    uint32_t                port_number;
    uint32_t                max_attr_list_length;
    bool                    auto_announce;
} net_slp_config_t;

void net_slp_config( net_slp_config_t *config );

/**
 * \brief Build up the attribute list for the SLP response
 *
 * \param SLP_ATTR_REPLY structure where response should be
 * placed
 *
 * \return error_type_t OK upon success, FAIL otherwise
 *
 */
error_type_t net_slp_add_attr_list( char *reply, slp_version_t version, /*struct sockaddr_storage**/ struct in6_addr* sockaddr );


/**
 * \brief Check if SLP request matches our search criteria
 *
 * \param char* pointer to incoming data packet
 *
 * \return 0 = no match else = match
 *
 */
uint8_t net_slp_match_url ( char *data );

#ifdef __cplusplus
}
#endif

#endif

