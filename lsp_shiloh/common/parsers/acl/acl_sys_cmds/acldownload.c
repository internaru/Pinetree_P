/*
 *
 * ============================================================================
 * Copyright (c) 2006-2009   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file acldownload.c
 *
 * \brief Code to handle the acl download command.
 *
 */
#include <string.h>

#include "posix_ostools.h"
#include "lassert.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "agMessage.h"
#include "hwconfig_api.h"
#include "acl_api.h"
#include "aclproto.h"
#include "checksum.h"
#include "fwappheader.h"
#include "acl_nvram_constants.h"
#include "platform_api.h"
#include "ioutils.h"

#ifndef __linux__
#include "cpu_api.h"
#endif

typedef uint32_t (*IMAGE_ENTRY_FUNC)(void);

extern char g_Version[];

#define ACLCMD_DOWNLOAD            0xC0DE
#define LEN_ACL_CODEVERSION 8

#pragma pack(1)
typedef struct
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    uint32_t codeLength;
    uint8_t  version[LEN_ACL_CODEVERSION];
} downloadCmd_t;

#pragma pack()
#define ACLDOWNLOAD_STACK_SIZE		POSIX_MIN_STACK_SIZE

#ifdef __linux__
// ??????? - need to implement this for Linux
#else
static pthread_t    acl_download_thread_id;

static uint32_t elf_size = 0;;
static void *elf_addr = 0;
/**
 * agiACLSetDownloadInfo
 * 
 * @author mikee (12/8/2012)
 * 
 * @param downloadAddr 
 * @param size 
 */
void agiACLSetDownloadInfo(void *downloadAddr, uint32_t size)
{
    elf_addr = downloadAddr;
    elf_size = size;
}
/**
 * agiACLGetDownloadInfo - retrieve the addr & size of stored 
 * download file... 
 * 
 * @author mikee (12/8/2012)
 * 
 * @param addr 
 * @param size 
 */
void agiACLGetDownloadInfo(void **addr, uint32_t *size)
{
    *addr = elf_addr;
    *size = elf_size;
}
/**
 * \brief This is the download task 
 */

void *agiACLGetDownloadThreadStkStart(void)
{
    uintptr_t addr;
    uint32_t memSize;

     memSize = hwGetRamSize();

    // for now just put at end of memory - but STAY out of SLOW uncached
    addr = hwGetRamStartAddress() + memSize - hwGetUncachedRamSize()- ACLDOWNLOAD_STACK_SIZE;

    // leave 1k at end for fun
    addr -= 1024;
    return (void *)addr;
}
/**
 * \brief Called to get the address to use for downloading code.
 * 
 * \param[in] size The size of the downloaded data in bytes.
 */
void *agiACLGetDownloadAddr(unsigned int size)
{
    uintptr_t addr;

    addr = (uintptr_t)agiACLGetDownloadThreadStkStart() - size;

    // leave 1k between code and stack for fun
    addr -= 1024;
    addr &= 0xffffffe0;
    return (void *)addr;
}
/* FUNCTION NAME: AclDoDownload */
 
/** 
 * \brief Download and run code from the host.
 * This routine reads a download file from the host, verifies the checksum and
 * the platformid, unpacks the file into memory and starts running the file.
 * 
 * \param[in] Pipe IO Pipe to read and write data
 * 
 * \param[in] Buffer Response buffer
 * 
 * \param reserved For later use.
 * 
 * \retval error_type_t
 * 
 * \date 2/27/2006
 * 
 **/
error_type_t AclDoDownload(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    uint32_t crcVal, numBytes, CheckSum, size, HeaderSize;
    int32_t bytesRead;
    void *downloadAddr;
    void *stackStart;
    unsigned int totalBytes = 0;
    FW_APP_HEADER   *FwAppHeader;

    size = be32_to_cpu( ((downloadCmd_t *) AclCmd)->codeLength );

    //
    // At this point the other hosts have been stopped 
    // and we are ready for the download.
    //
    // get some memory to download the code into.  Make sure you get aligned,
    // makes usb much faster.
    //

#ifdef HAVE_ELF_DOWNLOAD_DISABLE
    downloadAddr = NULL;
    dbg_printf("%s firmware download disabled\n", __FUNCTION__ );
#else
    downloadAddr = MEM_MALLOC_ALIGN(size+128, cpu_get_dcache_line_size());    /// want the extra for authenication.
#endif

    if ( downloadAddr == NULL )
    {
        uint32_t bitBucketSize = 1000000;

        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: MEM_MALLOC() failed to find %u bytes for firmware download\n", size ));

        // We have no place to put this image but the host has already
        // sent it to us.  Therefore we must read in the bytes and
        // dispose of them. To accomplish this I employ a small buffer.

        downloadAddr = MEM_MALLOC_ALIGN(bitBucketSize+128, cpu_get_dcache_line_size());

        if ( downloadAddr == NULL )   // we're in trouble
            goto ACLFAIL;

        while (totalBytes < size)
        {
            bytesRead = Pipe->ReadConnect( Pipe, downloadAddr,
                                           MIN(size-totalBytes,bitBucketSize),
                                           PARSEREAD_EXACT );
            if (bytesRead <= 0)
            {
                // We had a problem reading, chuck the data and return.

                DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ReadConnect() failed and returned %d\n", bytesRead));
                goto ACLFAIL;        // FAIL calls MEM_FREE_AND_NULL(downloadAddr)
            }
            totalBytes += bytesRead;
        }
        goto ACLFAIL;        // FAIL calls MEM_FREE_AND_NULL(downloadAddr)
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Downloading image of %u bytes to address 0x%08x\n", size, (uintptr_t)downloadAddr));



    while (totalBytes < size)
    {
        bytesRead = Pipe->ReadConnect(Pipe, downloadAddr + totalBytes, size - totalBytes, PARSEREAD_EXACT );
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("bytes Read %d total %d\n",bytesRead, totalBytes));
        if (bytesRead <= 0)
        {
            // We had a problem reading, chuck the data and return.

            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ReadConnect() failed and returned %d\n", bytesRead));
            goto ACLFAIL;
        }

        totalBytes += bytesRead;

    }
    //
    // Now check that we have a valid checksum.  If not get out of here.
    // First we compute the checksum from the downloaded image...
    crcVal = CalcChecksum((uint32_t *) downloadAddr, size - CHECKSUM_LENGTH);
    // ... then we read the checksum transmitted with the downloaded image.
    for (numBytes = 0, CheckSum = 0; numBytes < CHECKSUM_LENGTH; numBytes++)
    {
        CheckSum = (CheckSum << 8) + ((uint8_t *)downloadAddr)[size - CHECKSUM_LENGTH + numBytes];
    }
    //
    // Now check the checksum
    //
    if (crcVal != CheckSum)
    {
        // The checksum failed, clean up and get out of here.
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Failed download checksum Sent %x Calc %x\n",CheckSum, crcVal));
        goto ACLFAIL;
    }

    memset((char *)((uintptr_t) downloadAddr + size - CHECKSUM_LENGTH),0,128); // zero out the padding.

    FwAppHeader = downloadAddr;

    if(CodeCheckStart(downloadAddr, size-CHECKSUM_LENGTH) != 0)
	{
        goto ACLFAIL;          // did not authenticate, get out of here.
	}

    HeaderSize = (be32_to_cpu(FwAppHeader->LengthKey) & 0x7fff0000)>>HEADER_LEN_SHIFT;    // get the header length out of the size.
    size -= (HeaderSize+CHECKSUM_LENGTH);
    downloadAddr = (void *)((uintptr_t) downloadAddr + HeaderSize);
    // store our location and size, for our handling thread to SAFELY process
    agiACLSetDownloadInfo(downloadAddr,size);


    // We don't need to employ MEM_MALLOC() to get the stack
    // for the download thread since we will shortly be
    // blowing away the old heap anyway.

    stackStart = agiACLGetDownloadThreadStkStart();
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: download stack located at address 0x%08x\n", (uintptr_t)stackStart));

    posix_create_thread(&acl_download_thread_id, agiACLHandleDownload, (void*)0, "acl_parser", stackStart, ACLDOWNLOAD_STACK_SIZE, POSIX_THR_PRI_HIGHEST);

    while (1)
    {
        // too dangerous to just return to system -- our download thread destroys MANY things, non repairable.
        posix_sleep_ms(1000);
        DPRINTF((DBG_LOUD | DBG_OUTPUT),("%s spin\n",__FUNCTION__));
    }
    return OK;

ACLFAIL:
    MEM_FREE_AND_NULL(downloadAddr);
    return FAIL;
}
#endif
