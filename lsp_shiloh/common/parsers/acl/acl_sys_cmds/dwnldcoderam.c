/***********************************************************
* (c) Copyright 2006-2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#include <string.h>

#include "dprintf.h"
#include "ATypes.h"
#include "logger.h"
#include "debug.h"
#ifdef __linux__
#else
#include "cpu_api.h"
#endif
#include "fwappheader.h"
#include "ioutils.h"
#include "hwconfig_api.h"
#include "sys_init_api.h"

#ifdef __linux__
// not currently supported on linux
#else
extern int LoadELF(UINT8* imageStartAddr);
extern void   agiACLGetDownloadInfo(void **, uint32_t *);
extern void * agiACLGetDownloadAddr(unsigned int size);

 
/* FUNCTION NAME: agiACLHandleDownload */
 
/** 
 * \brief Start the downloaded 
 * 
 * \param codeLoc
 * 
 * \retval None
 * 
 * \author Dirk Bodily -- modified MikeE (20121207)
 * 
 * \date 2/27/2006
 * 
 **/
void* agiACLHandleDownload(void* xxx)
{
    uint32_t download_size;
    void *download_addr;
    agiACLGetDownloadInfo(&download_addr, &download_size);
    ASSERT(download_size);

    // I personally would prefer to disable ints here, AND have routines that, quickly FORCE off all DMA, not gentle shutdown routines...
    // IF you are having issue -- CHECK the SHUTDOWN_HARDWARE() and make sure YOUR project has routine populated in macro!!!
#ifdef SHUTDOWN_HARDWARE
    // shutdown ALL dma (usbd, usbh, network....)
    SHUTDOWN_HARDWARE();  // function in projects main.c to shutdown your asic hardware per your needs...
#else
    DPRINTF((DBG_LOUD | DBG_OUTPUT),("\n\n\n\n*** %s WARNING -- NO H/W SHUTDOWN DEFINED!!!!!!\n\n\n\n",__FUNCTION__));
#endif

    //
    // Turn off all the interrupts.  This prevents us from being bothered by spirious interrupts.
    //
    cpu_disable_interrupts();

    // At this moment the downloaded image is located in
    // memory at an arbitrary address selected by MEM_MALLOC().
    // But we now need to push it to the very end of our
    // address space in order to minimize the chance that
    // when we unpack its contents (which go at the start
    // of our memory space) we will write on top of the
    // downloaded image.

    // We use memmove() to relocate the downloaded image to
    // the address returned by agiACLGetDownloadAddr(),

    // BE CAREFUL TO EMPLOY memmove() here rather than memcpy()
    // since only the former can deal with an overlapping source
    // and destination !!
    void *codeLoc = agiACLGetDownloadAddr(download_size);
    memmove((void*) codeLoc, download_addr, download_size );
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: image moved to address %#08x\n", (unsigned int)codeLoc));

    int (*Loadelf)(UINT8* imageStartAddr);

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("%s: Processing code at address %#x:%#x\n",__FUNCTION__, codeLoc, download_size));

    Loadelf = (int (*)(UINT8*))(codeLoc - 0x1000);      // get the function ready to go.
    memcpy(Loadelf, LoadELF, 0x1000);

    cpu_dcache_writeback_all();
    cpu_disable_dcache();
    cpu_icache_invalidate_all();        // explicitly invalidate icache
    cpu_disable_mu();               // mpu or mmu

    // load image and run
    Loadelf((UINT8 *)codeLoc);       // load and run it.
    // we have serious issue if we returned, so reboot.  Our system is too shutdown(memory stomped on..) to continue...
    hwPlatformReset();
    return 0;
}

 
/* FUNCTION NAME: CodeCheckStart */
 
/** 
 * \brief Check a code image for validity, authenticate it and run it.
 * 
 * \param Code The image
 * 
 * \param Length The length of the image.
 * 
 * \retval None
 * 
 * \author Dirk Bodily
 * 
 * \date 9/21/2007
 * 
 **/
int CodeCheckStart(UINT32 *Code, UINT32 Length)
{
    FW_APP_HEADER *Header;
    //
    // Run the checks to see if this is a valid image.
    // 
    Header = (FW_APP_HEADER *)Code;
    //
    // See if the code header is valid and exit if not.
    //
    if ((be32_to_cpu(Header->LengthKey) & KEY_MASK) != HEAD_KEY_VALUE)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ERROR FW code header invalid\n\r"));
        return -1;
    }
   
    //
    // Check to make sure the version and platform match.  If they are both zero
    // we do not do the check.
    //
    if((((( be32_to_cpu(Header->Version) & PLAT_ID_MASK)>>PLAT_ID_SHIFT) != 0) || // if == 0 don't do check
        ((( be32_to_cpu(Header->ProgInfo) & VERSION_ID_MASK)>>VERSION_ID_SHIFT) != 0)))// &&
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("ERROR: Versions do not match, do not run\n\r"));
        return -1;
    }
    if ( be32_to_cpu(Header->LengthKey) & AUTH_MASK)   // do we need to authenticate?
    {
        // 
        // We need to authenticate, do that here.
        //
#if 0
        SecurityInit(); // get the sw ready to go
        SecurityDigestAuthenticate(Code,Length, 1);
        if (SecurityPayloadCheck( Code, Length) != 1)
        {
            // failed the check, get out and do not run.
            DPRINTF(DBG_LOUD,("ERROR failed authentication\n\r"));
            return -1;
        }
#endif
    } 
    return 0;

}
#endif
