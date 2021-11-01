/*
 * ============================================================================
 * Copyright (c) 2014   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file axi_bus_api.c
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
 **/
  
#include <stddef.h>
#include "axi_bus_api.h"
#include "asic.h"
#include "lassert.h"

#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include "map_mem.h"
#endif

#define AXI_BUS_QOS_MAX 15

/// \brief Map AXI bus block address appropriately for the OS and return.
/// \note Unmap must be called for all mapped blocks to release the mapping.
static void* axi_bus_map_block_addr(axi_bus_block_t bus_block, uint32_t* block_size)
{
    void* block_addr = NULL;
    *block_size = 0;

    switch (bus_block)
    {
        case AXI_BUS_BLOCK_MVDO:
            block_addr = (void*)BUS_MVDO_S_BASE;
            *block_size = sizeof(MVDO_S_REGS_t);
            break;
        case AXI_BUS_BLOCK_PIC:
            block_addr = (void*)BUS_PIC_S_BASE;
            *block_size = sizeof(PIC_S_REGS_t);
            break;
        case AXI_BUS_BLOCK_CISX:
            block_addr = (void*)BUS_CISX_S_BASE;
            *block_size = sizeof(CISX_S_REGS_t);
            break;
        case AXI_BUS_BLOCK_JBIG:
            block_addr = (void*)BUS_JBIG_S_BASE;
            *block_size = sizeof(JBIG_S_REGS_t);
            break;
        case AXI_BUS_BLOCK_PCIE:
            block_addr = (void*)BUS_PCIE_S_BASE;
            *block_size = sizeof(PCIE_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_M2M:
            block_addr = (void*)BUS_M2M_S_BASE;
            *block_size = sizeof(M2M_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_GLAN:
            block_addr = (void*)BUS_GLAN_S_BASE;
            *block_size = sizeof(GLAN_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_LCD:
            block_addr = (void*)BUS_LCD_S_BASE;
            *block_size = sizeof(LCD_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_PIP:
            block_addr = (void*)BUS_PIP_S_BASE;
            *block_size = sizeof(PIP_S_REGS_t);
            break;
        case AXI_BUS_BLOCK_JPEGR:
            block_addr = (void*)BUS_JPEGR_S_BASE;
            *block_size = sizeof(JPEGR_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_JPEGW:
            block_addr = (void*)BUS_JPEGW_S_BASE;
            *block_size = sizeof(JPEGW_S_REGS_t);
            break;  
        case AXI_BUS_BLOCK_UPC:
            block_addr = (void*)BUS_UPC_S_BASE;
            *block_size = sizeof(UPC_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_CDMA:
            block_addr = (void*)BUS_CDMA_S_BASE;
            *block_size = sizeof(CDMA_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_USB:
            block_addr = (void*)BUS_USB_S_BASE;
            *block_size = sizeof(USB_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_SDMMC:
            block_addr = (void*)BUS_SDMMC_S_BASE;
            *block_size = sizeof(SDMMC_S_REGS_t);
            break;        
        case AXI_BUS_BLOCK_ZX:
            block_addr = (void*)BUS_ZX_S_BASE;
            *block_size = sizeof(ZX_S_REGS_t);
            break;        
        default:
            break;
    }

    // if valid block, map address
#ifdef __linux__
    if (block_addr)
    {
        int fd_mem = -1;
        void *baseaddr;

        fd_mem = open("/dev/mem", (O_RDWR | O_SYNC));
        XASSERT(fd_mem >= 0, fd_mem);
        block_addr = mapMem((off_t)block_addr, *block_size, &baseaddr, fd_mem);
        close(fd_mem);
    }
#endif

    return block_addr;
}


static void axi_bus_unmap_block_addr(void* block_addr, uint32_t length)
{
#ifdef __linux__
    unMapMem(block_addr, length);
#endif
}
 
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
error_type_t axi_bus_set_qos(axi_bus_block_t bus_block, uint32_t read_qos, uint32_t write_qos)
{
    uint32_t block_size;
    void* block_addr;

    if (read_qos > AXI_BUS_QOS_MAX || write_qos > AXI_BUS_QOS_MAX)
    {
        return FAIL;
    }

    block_addr = axi_bus_map_block_addr(bus_block, &block_size);

    if (block_addr != 0)
    {
        switch (bus_block)
        {
            case AXI_BUS_BLOCK_MVDO:
                ((MVDO_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((MVDO_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;
            case AXI_BUS_BLOCK_PIC:
                ((PIC_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((PIC_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;
            case AXI_BUS_BLOCK_CISX:
                ((CISX_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((CISX_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;
            case AXI_BUS_BLOCK_JBIG:
                ((JBIG_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((JBIG_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;
            case AXI_BUS_BLOCK_PCIE:
                ((PCIE_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((PCIE_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_M2M:
                ((M2M_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((M2M_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_GLAN:
                ((GLAN_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((GLAN_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_LCD:
                ((LCD_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((LCD_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_PIP:
                ((PIP_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((PIP_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;
            case AXI_BUS_BLOCK_JPEGR:
                ((JPEGR_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((JPEGR_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;      
            case AXI_BUS_BLOCK_JPEGW:
                ((JPEGW_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((JPEGW_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;                     
            case AXI_BUS_BLOCK_UPC:
                ((UPC_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((UPC_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_CDMA:
                ((CDMA_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((CDMA_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_USB:
                ((USB_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((USB_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_SDMMC:
                ((SDMMC_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((SDMMC_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            case AXI_BUS_BLOCK_ZX:
                ((ZX_S_REGS_t*)block_addr)->read_qos = read_qos;
                ((ZX_S_REGS_t*)block_addr)->write_qos = write_qos;
                break;        
            default:
                ASSERT(0);
                break;
        }
        ASSERT(block_size > 0);
        axi_bus_unmap_block_addr(block_addr, block_size);
        return OK;
    }
    return FAIL;
}

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
error_type_t axi_bus_get_qos(axi_bus_block_t bus_block, uint32_t* read_qos, uint32_t* write_qos)
{
    uint32_t block_size;
    void* block_addr;
    uint32_t local_read_qos = 0;
    uint32_t local_write_qos = 0;

    block_addr = axi_bus_map_block_addr(bus_block, &block_size);

    if (block_addr != 0)
    {
        switch (bus_block)
        {
            case AXI_BUS_BLOCK_MVDO:
                local_read_qos = ((MVDO_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((MVDO_S_REGS_t*)block_addr)->write_qos;
                break;
            case AXI_BUS_BLOCK_PIC:
                local_read_qos = ((PIC_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((PIC_S_REGS_t*)block_addr)->write_qos;
                break;
            case AXI_BUS_BLOCK_CISX:
                local_read_qos = ((CISX_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((CISX_S_REGS_t*)block_addr)->write_qos;
                break;
            case AXI_BUS_BLOCK_JBIG:
                local_read_qos = ((JBIG_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((JBIG_S_REGS_t*)block_addr)->write_qos;
                break;
            case AXI_BUS_BLOCK_PCIE:
                local_read_qos = ((PCIE_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((PCIE_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_M2M:
                local_read_qos = ((M2M_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((M2M_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_GLAN:
                local_read_qos = ((GLAN_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((GLAN_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_LCD:
                local_read_qos = ((LCD_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((LCD_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_PIP:
                local_read_qos = ((PIP_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((PIP_S_REGS_t*)block_addr)->write_qos;
                break;
            case AXI_BUS_BLOCK_JPEGR:
                local_read_qos = ((JPEGR_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((JPEGR_S_REGS_t*)block_addr)->write_qos;
                break;   
            case AXI_BUS_BLOCK_JPEGW:
                local_read_qos = ((JPEGW_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((JPEGW_S_REGS_t*)block_addr)->write_qos;
                break;                      
            case AXI_BUS_BLOCK_UPC:
                local_read_qos = ((UPC_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((UPC_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_CDMA:
                local_read_qos = ((CDMA_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((CDMA_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_USB:
                local_read_qos = ((USB_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((USB_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_SDMMC:
                local_read_qos = ((SDMMC_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((SDMMC_S_REGS_t*)block_addr)->write_qos;
                break;        
            case AXI_BUS_BLOCK_ZX:
                local_read_qos = ((ZX_S_REGS_t*)block_addr)->read_qos;
                local_write_qos = ((ZX_S_REGS_t*)block_addr)->write_qos;
                break;        
            default:
                ASSERT(0);
                break;
        }
        ASSERT(block_size > 0);
        axi_bus_unmap_block_addr(block_addr, block_size);

        if (read_qos)
        {
            *read_qos = local_read_qos;
        }
        if (write_qos)
        {
            *write_qos = local_write_qos;
        }
        return OK;
    }
    return FAIL;
}

