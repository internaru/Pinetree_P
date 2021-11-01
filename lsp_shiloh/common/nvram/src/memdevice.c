/**
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file memdevice.c
 *
 *  \brief Memory Device Driver Manager
 *
 *  This file allows a single point if entry for accessing one of several 
 *  memory device drivers (I2C EEPROM, SPI Flash, NAND, etc.)
 **/

#include <string.h>
#include <stdint.h>
#include "error_types.h"
#include "debug.h"
#include "dprintf.h"
#include "logger.h"
#include "utils.h"
#include "lassert.h"
#include "memAPI.h"
#include "deviceid.h"
#include "memdevice.h"
#include "regmemdev.h"
#include "nvram_logger.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static const char *memdevice_cmd_test_desc;
static const char *memdevice_cmd_test_usage;
static const char *memdevice_cmd_test_notes;
static int memdevice_cmd_test_cb(int argc, char *argv[]);
#endif

/*
 * Local variables
 */
static ATLISTENTRY memdevice_list;  ///< list of registered memory devices

/*
 * Local function prototypes
 */
static MEMDEV_HANDLE* findMemDev(MEMORY_DEVICE DeviceID);



/* FUNCTION NAME: InitMemDeviceAPI 
 *
 *  See memdevice.h for function documentation.
 */
void InitMemDeviceAPI(void)
{
#ifdef HAVE_CMD
    int cmd_res;
#endif

    ATInitList(&memdevice_list);

    /* Register all the available memory devices */
    RegisterMemDrv();

#ifdef HAVE_CMD
    cmd_res = cmd_register_cmd( "memdev",
                                NULL,
                                "memory device commands",
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "memdev",
                                   "test",
                                   memdevice_cmd_test_desc,
                                   memdevice_cmd_test_usage,
                                   memdevice_cmd_test_notes,
                                   memdevice_cmd_test_cb );
    ASSERT( CMD_OK == cmd_res );
#endif
}


/* FUNCTION NAME: RegisterMemoryDevice 
 *
 *  See memdevice.h for function documentation.
 */
error_type_t RegisterMemoryDevice(MEMDEV_HANDLE *Handle)
{
    MEMDEV_HANDLE  *TempStruct;
    uint32_t        allocSize;
    
    if (Handle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Make sure this device isn't already on the registered device list. If
     * so just ignore it.
     */  
    if (findMemDev(Handle->DeviceID) != NULL)
    {
        return OK;
    }
            
    /* We can't keep the memory pointed to by memDev (may be on the stack), so
     * attempt to allocate memory for a new instance structure to keep on our
     * registered device list.
     */
    TempStruct = (MEMDEV_HANDLE *)MEM_MALLOC(sizeof(MEMDEV_HANDLE));
    if (TempStruct == NULL)
    {
        return FAIL;
    }
    memset((char *)TempStruct, 0, sizeof(MEMDEV_HANDLE));
    
    /* Fill out the new structure and add it to the tail of the list */
    *TempStruct = *Handle;
    ATInsertTailList(&memdevice_list, TempStruct->node);

    return OK;
}


/* FUNCTION NAME: memDevInstalled 
 *
 *  See memdevice.h for function documentation.
 */
bool memDevInstalled(MEMORY_DEVICE DeviceID)
{
    bool         retCode = true;

    if (findMemDev(DeviceID) == NULL)
    {
        retCode = false;
    }
    
    return retCode;
}


/* FUNCTION NAME: devOpen 
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devOpen(MEMORY_DEVICE DeviceID, MEMDEV_HANDLE **Handle)
{
    MEMDEV_HANDLE  *TempStruct;
    MEMDEV_HANDLE  *devHandle;
    error_type_t    retStatus = FAIL;

    if (Handle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }

    /* Assume things will go badly */
    *Handle = NULL;
           
    /* Try to find the device in our registered device list */
    TempStruct = findMemDev(DeviceID);

    if (TempStruct != NULL)
    {
        /* We found the device and need to fill out the instance: this
         * interface will allocate an instance structure if not supplied
         */       
        devHandle = (MEMDEV_HANDLE *)MEM_MALLOC(sizeof(MEMDEV_HANDLE));
        if (devHandle == NULL)
        {    
            DBG_PRINTF(LOG_ERR|DBG_MEMDEVICE, "MEMDEVICE(%d): Memory alloc failed\n", __LINE__);
            return FAIL;
        }
                    
        /* Start out with a clean structure */ 
        memset(devHandle, 0, sizeof(MEMDEV_HANDLE));
 
        /* Fill in our device driver info */
        *devHandle = *TempStruct;
        devHandle->MemHandle = NULL;  /* device driver will allocate memory */

        /* Try and open the device driver */
        retStatus = devHandle->Open(DeviceID, (void *)(&(devHandle->MemHandle)));

        if (retStatus != OK)
        {
            MEM_FREE_AND_NULL(devHandle);
        }
        else
        {
            /* We made it, set return handle */
            *Handle = devHandle;
        }
    }
    else
    {
        DBG_PRINTF(LOG_INFO|DBG_MEMDEVICE, "MEMDEVICE(%d):devOpen - %s not registered\n", __LINE__, getDevIdStr(DeviceID));
    }
    
    return retStatus;
}


/* FUNCTION NAME: devClose 
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devClose(MEMDEV_HANDLE *Handle)
{
    if (Handle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }

    if (Handle->Close(Handle->MemHandle) == OK)
    {
        MEM_FREE_AND_NULL(Handle);
        return OK;
    }
    
    /* The device close failed (resources still allocated) */
    return FAIL;
}


/* FUNCTION NAME: devReport 
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devReport(MEMDEV_HANDLE *Handle, DRIVER_CONFIG_t *devData)
{
    if ((Handle == NULL) || (devData == NULL))
    {
        ASSERT(false);
        return FAIL;
    }
    
    return Handle->Report(Handle->MemHandle, devData);  
}


/* FUNCTION NAME: devRead 
 *
 *  See memdevice.h for function documentation.
 */
int32_t devRead(MEMDEV_HANDLE *Handle, int8_t *Dest, uint64_t Address, uint32_t Length)
{
    if ((Handle == NULL) || (Dest == NULL) || (Length == 0))
    {
        ASSERT(false);
        return 0;
    }

    return Handle->Read(Handle->MemHandle, Dest, Address, Length);
}

/* FUNCTION NAME: devWrite 
 *
 *  See memdevice.h for function documentation.
 */
int32_t devWrite(MEMDEV_HANDLE *Handle, int8_t *Src, uint64_t Address, uint32_t Length)
{    
    if ((Handle == NULL) || (Src == NULL) || (Length == 0))
    {
        ASSERT(false);
        return 0;
    }

    return Handle->Write(Handle->MemHandle, Src, Address, Length);
}


/* FUNCTION NAME: devErase 
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devErase(MEMDEV_HANDLE *Handle, uint32_t Block)
{
    if (Handle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }

    return Handle->Erase(Handle->MemHandle, Block);
}

/* FUNCTION NAME: devFormat
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devFormat(MEMDEV_HANDLE *Handle)
{
    ASSERT(Handle != NULL);

    // this is an optionally supported routine.  Func ptr may be NULL.
    if (Handle == NULL || Handle->Format == NULL)
    {
        return FAIL;
    }

    return Handle->Format(Handle);
}

/* FUNCTION NAME: devVerifyNvFormat
 *
 *  See memdevice.h for function documentation.
 */
error_type_t devVerifyNvFormat(MEMDEV_HANDLE *Handle, uint8_t* ctbuffer)
{
    ASSERT(Handle != NULL && ctbuffer != NULL);

    if (Handle == NULL || ctbuffer == NULL)
    {
        return FAIL;
    }

    // this is an optionally supported routine.  Func ptr may be NULL.
    if(Handle->VerifyFormat == NULL)
    {
        // just return success for devices that don't support format verification
        return OK;
    }

    return Handle->VerifyFormat(ctbuffer);
}

/**
 * \brief Attempt to find device in the device list
 *
 * \param DeviceID A string that specifies the device.
 *
 * \retval handle The handle used to access the device; NULL if not found
 *
 **/
static MEMDEV_HANDLE* findMemDev(MEMORY_DEVICE DeviceID)
{
    ATLISTENTRY* node;
    MEMDEV_HANDLE  *TempStruct = NULL;

    node = ATListHead(&memdevice_list);
    while(node != NULL)
    {
        TempStruct = CONTAINING_RECORD(node, MEMDEV_HANDLE, node);
        if(TempStruct->DeviceID == DeviceID)
        {
            /* found the device, bail out */
            break;
        }
        node = ATListNext(&memdevice_list, node);
    }
    return TempStruct;
}



#ifdef HAVE_CMD
static const char *memdevice_cmd_test_desc = "test the MEM_I2C memory device";
static const char *memdevice_cmd_test_usage = NULL;
static const char *memdevice_cmd_test_notes = NULL;
static int memdevice_cmd_test_cb( int argc, char *argv[] )
{
    MEMDEV_HANDLE *handle = NULL;
    int8_t   Src[512];
    int8_t   Dest[512];
    uint32_t i;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    memset(Src, 0x97, 512);
    memset(Dest, 0x0, 512);
    if (devOpen(MEM_I2C, &handle) == OK)
    {
        cmd_printf("I2C Driver Opened!\n");

        i = devWrite(handle, Src, 1024, 256);
        cmd_printf("I2C Driver write %lu bytes!\n", i);

        i = devRead(handle, Dest, 1024, 256);
        cmd_printf("I2C Driver read %lu bytes!\n", i);

        devClose(handle);
        cmd_printf("I2C Driver Closed\n");
    }
    else
    {
        cmd_printf("unable to open MEM_I2C device\n");
        return CMD_ERROR;
    }
    
    return CMD_OK;
}

#endif
