/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_LINK_CONFIG_H
#define NET_LINK_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "net_api.h"

#include <stdio.h>
#include <unistd.h> // read
#include <sys/ioctl.h> // ioctl
#include <sys/mman.h> // mmap

// revisit -- treck type names
#ifndef _TRECK_TYPES_ALREADY_DEFINED_
#define _TRECK_TYPES_ALREADY_DEFINED_ /* see also <os_network.h> */
typedef void*                   ttUserInterface;
typedef ttUserInterface*        ttUserInterfacePtr;
typedef void*                   ttUserBuffer;
typedef ttUserBuffer*           ttUserBufferPtr;
typedef void*                   ttUserLinkLayer;
typedef struct
        {
          unsigned int devIndex;
          void* user_data;
        }                       ttDeviceEntry;
typedef ttDeviceEntry*          ttDeviceEntryPtr;
typedef void*                   ttDevRecvOffloadStruct;
typedef ttDevRecvOffloadStruct* ttDevRecvOffloadPtr;
typedef void           VOID;
typedef unsigned long  ULONG;

// revisit 
typedef int (*ttDevOpenCloseFuncPtr)(ttUserInterface);
typedef int (*ttDevSendFuncPtr)(ttUserInterface, char*, int, int);
typedef int (*ttDevRecvFuncPtr)(ttUserInterface, char**, int*, ttUserBuffer*, ttDevRecvOffloadPtr /*offload_ptr?*/);
typedef int (*ttDevIoctlFuncPtr) (ttUserInterface, int, void*, int);
typedef int (*ttDevGetPhysAddrFuncPtr)(void*, char*);
#endif

//=========================================================================================================================
// Enumerated network link config (i.e. per link instance)
//=========================================================================================================================

typedef net_rcode_t (*net_link_instance_oem_init_func_t)(net_link_api_funcs_t *api_funcs);

typedef struct
{
    net_link_instance_oem_init_func_t init;
} net_link_instance_oem_func_table_t;


/**
  * 
 * @param priority Link priority relative to other registered links sharing the same 
 *                 interface. When multiple registered links are "up" (i.e. have acquired
 *                 link status) on a shared network interface, only the highest priority
 *                 link will be bound to the overlying interface.  Every link attached to
 *                 an interface must be assigned a link_priority_t value that is unique
 *                 for that interface.NET_IFACE_LINK_PRIORITY_EXCLUSIVE
 * 
 * @param[in] link_name Pointer to a null-terminated character string representing the 
 *                 name of the link (e.g. "Ethernet"). User must use a name that is
 *                 unique across all registered links. Upon return the caller may free the
 *                 memory used to store the string.
 * 
 * Overlying network interfaces are created automatically during link enumeration. 
 * Enumeration occurs during initialization of the net_iface module. All links must be
 * registered prior to enumeration. The number of interfaces created cannot exceed 
 * NET_MAX_INTERFACES. 
 *  
 * Network links referencing the same iface_name during registration will share an 
 * interface following enumeration.  In this case, when more than one link is up at the 
 * same time only the highest priority link will be bound to the overlying network 
 * interface. If a link is registered with flag NET_LINK_FLAG_SHARED_EXCLUSIVE then the 
 * overlying network interface layer will notify the link layer whenever a higher priority 
 * link comes up so that its own link can be disabled. An exclusive link will receive 
 * IOCTLs NET_IFACE_IOCTL_LINK_ENABLE and NET_IFACE_IOCTL_LINK_DISABLE via its registered 
 * IOCTL handler to signal the link when to enable and disable itself, respectively. 
 */
typedef struct
{
    net_link_instance_oem_func_table_t *func_table;
    char                    name[NET_LINK_NAME_SIZE];
    link_priority_t         priority;
    net_link_flags_t        flags;

    // this enum must match a corresponding network interface enum to which the logical link will be attached
    uint8_t                 logical_link_enum;
} net_link_instance_oem_config_t;


//=========================================================================================================================
// General link config
//=========================================================================================================================

typedef net_rcode_t (*net_link_oem_init_func_t)(void);
typedef net_rcode_t (*net_link_instance_get_oem_config_func_t)(uint8_t link_enum, net_link_instance_oem_config_t *instance_oem_config);

typedef struct
{
    net_link_oem_init_func_t init;
    net_link_instance_get_oem_config_func_t instance_get_oem_config;
} net_link_oem_func_table_t;

struct net_link_s;
typedef struct
{
    net_link_oem_func_table_t *func_table;
    uint8_t link_cnt; // physical link count
    net_link_instance_oem_config_t *links; // physical link array
} net_link_oem_config_t;


//=========================================================================================================================
// Public OEM config API
//=========================================================================================================================

net_rcode_t net_link_get_oem_config(net_link_oem_config_t *oem_config);

#endif // NET_LINK_CONFIG_H

