/*
 * ============================================================================
 * Copyright (c) 2014   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file axi_bus_api.h
 * 
 * \brief AXI Bus Driver
 * 
 * This driver allows setting Quality-of-Service values for blocks on the AXI Bus.
 * 
 * The default QOS value for all blocks is 0 when the hardware is reset.
 * Valid QOS values are 0-15.
 * 
 * This API does not support all possible blocks or tuning parameters, only those
 * deemed most likely to be of use.  Others may be added as needed.
 *  
 * It is recommended that the user read and understand the ASIC Programmer's Guide 
 * before modifying bus priorities and that modification of bus priorities only 
 * be done to solve known system issues.  Modification of bus priority may cause 
 * bus starvation to lower priority blocks. 
 *  
 * Modification of bus priority is most likely to be of use on blocks that have strict 
 * latency requirements and that are paced by hardware so that they do not 
 * completely take over the bus.  Some examples of blocks where modifcation of 
 * bus priority may be useful are CISX, PIC, and MVDO. 
 *  
 * Typically the axi bus settings are configured only once during system initialization 
 * and are not modified thereafter.  This config is usually done from hwConfigInit 
 * routine in hwconfig.c. 
 *
 **/
  
#ifndef _AXI_BUS_API_H
#define _AXI_BUS_API_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief AXI Bus Block enumeration
 **/

typedef enum
{
    AXI_BUS_BLOCK_MVDO,
    AXI_BUS_BLOCK_PIC,
    AXI_BUS_BLOCK_CISX,
    AXI_BUS_BLOCK_JBIG,
    AXI_BUS_BLOCK_PCIE,
    AXI_BUS_BLOCK_M2M,
    AXI_BUS_BLOCK_GLAN,
    AXI_BUS_BLOCK_LCD,
    AXI_BUS_BLOCK_PIP,
    AXI_BUS_BLOCK_JPEGR,
    AXI_BUS_BLOCK_JPEGW,
    AXI_BUS_BLOCK_UPC,
    AXI_BUS_BLOCK_CDMA,
    AXI_BUS_BLOCK_USB,
    AXI_BUS_BLOCK_SDMMC,
    AXI_BUS_BLOCK_ZX
}axi_bus_block_t;

 
/** 
 * \brief Set the read and write Quality-of-Servce for the specified block on the AXI bus
 * 
 * \param bus_block - enum specifying which block to set QOS params for 
 *  
 * \param read_qos - read Quality-of-Service to set.  Valid values are 0-15.
 *  
 * \param write_qos - write Quality-of-Service to set.  Valid values are 0-15.
 *  
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t axi_bus_set_qos(axi_bus_block_t bus_block, uint32_t read_qos, uint32_t write_qos);

/** 
 * \brief Get the read and write Quality-of-Servce for the specified block on the AXI bus
 * 
 * \param bus_block - enum specifying which block to set QOS params for 
 *  
 * \param read_qos - read Quality-of-Service to get. Set to NULL if value not desired.
 *  
 * \param write_qos - write Quality-of-Service to get.  Set to NULL if value not desired.
 *  
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t axi_bus_get_qos(axi_bus_block_t bus_block, uint32_t* read_qos, uint32_t* write_qos);


#ifdef __cplusplus
}
#endif

#endif //_AXI_BUS_API_H
