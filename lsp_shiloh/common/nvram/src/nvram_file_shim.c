/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_file_shim.c
 *
 *  \brief NVRAM file shim
 *
 *  This file implements a file interface to the NVRAM API layer.
 *
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/stat.h>
#include "error_types.h"
#include "memAPI.h"
#include "list.h"
#include "lassert.h"
#include "checksum.h"
#include "deviceid.h"
#include "nvram_api.h"
#include "partitionMgrAPI.h"
#include "platform.h"
#include "nvram_dev_shim.h"
#include "nvram_config.h"
#include "nvram_logger.h"
#include "ioutils.h"


/*
 *  Local (static) variables
 */
#define NVRAM_DATA_PATH     "./nvram"
static char *g_nvram_data_path = NVRAM_DATA_PATH;


/*
 * FUNCTION NAME: InitNvramDevShims
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t InitNvramDevShim(void)
{
    struct stat st;

    /* check to see if the nvram path is overwritten by the environment variable */
    char *env_nvram = getenv("NVRAM");
    if (env_nvram != (char *)0)
        g_nvram_data_path = env_nvram;
    
    /* Make sure NVRAM directory exists */
    if (stat(g_nvram_data_path, &st) != 0)
    {
        /* Make sure NVRAM directory exists */
        mkdir(g_nvram_data_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    
    return OK;
} 


/*
 * FUNCTION NAME: LocateVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t LocateVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    struct stat st;
    
    if (nHandle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }
    
    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    /* deterine if file exists, set MaxLength if it does */
    if (stat(filepath, &st) == 0)
    {
        /* set size based on existing file */
        nHandle->MaxLength = st.st_size;
        
        /* attempt to open existing file */
        nHandle->fd = open(filepath, O_RDWR | O_CLOEXEC);
        if (nHandle->fd != -1)
        {
            ret = OK;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to open existing file %s, errno = 0x%x\n", __LINE__, filepath, errno);
        }
    }
       
    return ret;
}


/*
 * FUNCTION NAME: RegisterVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t RegisterVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    
    /* Validate input parameters */
    if ( (nHandle == NULL) || (nHandle->BlockLabel == NULL) || 
         (nHandle->MaxLength == 0) )
    {
        return FAIL;
    }

    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    /* open file, create if does not exist */
    nHandle->fd = open(filepath, O_RDWR | O_CREAT | O_CLOEXEC, 0666);
    if (nHandle->fd != -1)
    {
        // seek to new EOF-1
        lseek(nHandle->fd, nHandle->MaxLength-1, SEEK_SET);
        
        // something needs to be written at end for file to have new size, 
        // so just write an empty string which results in a '\0'.
        write(nHandle->fd, "", 1);
        fsync(nHandle->fd);
        ret = OK;
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to open file %s, errno = 0x%x\n", __LINE__, filepath, errno);
    }
       
    return ret;
}


/*
 * FUNCTION NAME: ReleaseVarBlock
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t ReleaseVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    
    /* Validate input parameters */
    if ( (nHandle == NULL) || (nHandle->BlockLabel == NULL) || 
         (nHandle->MaxLength == 0) )
    {
        return FAIL;
    }
    
    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    // delete file
    if (remove(filepath) == 0)
    {
        ret = OK;
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to remove file %s, errno = 0x%x\n", __LINE__, filepath, errno);
    }
       
    return ret;
}

/*
 * FUNCTION NAME: VarBlockRead
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockRead(nvram_handle_t *nHandle, int8_t *blockPtr, uint64_t offset, uint32_t size)
{
    uint32_t bytes_read = 0;
    
    if ((nHandle == NULL) || (nHandle->fd == -1) || (blockPtr == NULL) || (size == 0))
    {
        return 0;
    }

    // seek to offset
    if (lseek(nHandle->fd, offset, SEEK_SET) >= 0)
    {
        int bytes;

        // read bytes from file
        bytes = read(nHandle->fd, blockPtr, size);
        if (bytes > 0)
        {
            bytes_read = (uint32_t)bytes;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable read block of %d bytes, errno = 0x%x\n", __LINE__, size, errno);
        }
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to r-seek to offset %d, errno = 0x%x\n", __LINE__, offset, errno);
    }
       
    return bytes_read;
}


/*
 * FUNCTION NAME: VarBlockWrite
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockWrite(nvram_handle_t *nHandle, int8_t *blockPtr)
{
    uint32_t bytes_written = 0;
    
    if ((nHandle == NULL) || (nHandle->fd == -1) || (nHandle->MaxLength == 0)  || (blockPtr == NULL))
    {
        return 0;
    }

    // seek to SOF
    if (lseek(nHandle->fd, 0, SEEK_SET) >= 0)
    {
        int bytes;
        
        // write bytes to file
        bytes = write(nHandle->fd, blockPtr, nHandle->MaxLength);
        if (bytes > 0)
        {
            // make sure any buffers are flushed to storage device
            fsync(nHandle->fd);
            bytes_written = (uint32_t)bytes;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable write block of %d bytes\n, errno = 0x%x", __LINE__, nHandle->MaxLength, errno);
        }
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to w-seek to SOF, errno = 0x%x\n", __LINE__, errno);
    }
       
    return bytes_written;
}


/*
 * FUNCTION NAME: VarBlockCheckCRC
 *
 *   See nvram_dev_shim.h for function documentation.
 */
bool VarBlockCheckCRC(char * label, VARBLOCK_HEADER_t *varBlock, uint32_t erased_data_val)
{
    // no need to call this when HAVE_NVRAM_FILE defined
    ASSERT(false);
    return false;
}

