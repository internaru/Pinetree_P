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
 * \file aclburnflash.c
 *
 * \brief Implements the acl burn flash command.
 *
 */

// Includes
#include <stdint.h>
#include "ATypes.h"
#include "acl_api.h"
#include "memAPI.h"
#include "logger.h"
#include "hwconfig_api.h"
#include "aclproto.h"
#include "ioutils.h"
#include "lassert.h"
#include "event_observer.h"
#include "response_subject.h"
#include "ACL_burn_flash_event_api.h"

// Defines

#define ACLCMD_BURN_FLASH       0x000F
#define ACL_BURN_FLASH_EVENT_ID 0x41434C20


// Structures

#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_CMD_BASE_CLASS_MEMBERS;
    uint16_t destination;  // the part to burn; must be one of the BF_... defines from above
    uint16_t checkSum;     // this is the lower half of the folded checksum;
                         // the upper 16 bit are always 0xFFFF anyway, so we don't send them
    uint32_t offset;       // offset in the part at which to burn the data
    uint32_t dataLen;      // the amount of data following the command; this is the data that will be burned to the part
} burnFlashCmd_t;
#pragma pack()


#if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)
typedef struct ACL_burn_flash_event_Subject_s
{
    // Common values added to all subjects.
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;

    acl_burn_flash_event_t burn_flash_event;

} ACL_burn_flash_event_Subject_t;
#endif


/** 
 * \brief Singleton Constructor, initializes, doubles as get 
 *        structure.
 * 
 * \retval pointer to ACL_burn_flash_event_Subject_t 
 * 
 **/
#if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)
static ACL_burn_flash_event_Subject_t *ACL_burn_flash_event(void)
{
    static ACL_burn_flash_event_Subject_t *ptr = NULL;

    if (!ptr)
    {
        // Construct a copy of the event and populate 
        // it with initial values
        uint32_t size = sizeof(ACL_burn_flash_event_Subject_t);

        ptr = (ACL_burn_flash_event_Subject_t *)MEM_MALLOC(size);
        ASSERT(ptr);

        ptr->burn_flash_event = BURN_FLASH_EVENT_UNKNOWN;

        // Initialize the sanity check 
        ptr->subject_observer_oid_key = ACL_BURN_FLASH_EVENT_ID;
        MInitList(&(ptr->observers));
    }
    return ptr;
}

/** 
 * \brief Notify of a burn flash event. 
 * 
 * \retval N/A
 * 
 **/
void ACL_Notify_burn_flash_event(acl_burn_flash_event_t burn_flash_event)
{   
    // Set the subjects values BEFORE calling the NOTIFY_OBSERVERS
    // otherwise Get calls will have stale data
    ACL_burn_flash_event()->burn_flash_event = burn_flash_event;

    // This will call each observers' callback
    NOTIFY_OBSERVERS(ACL_burn_flash_event(), observers);
}

/** 
 * \brief Attach a burn flash event observer.
 * 
 * \retval N/A
 * 
 **/
void ACL_AttachObserver_burn_flash_event(Observer_t *o)
{
    ATTACH_OBSERVER(ACL_burn_flash_event()->observers, o);
}

/** 
 * \brief Detach a burn flash event observer.
 * 
 * \retval N/A
 * 
 **/
void ACL_DetachObserver_burn_flash_event(Observer_t *o)
{
    DETACH_OBSERVER(ACL_burn_flash_event()->observers, o);
}

/** 
 * \brief Return the current burn flash event.
 * 
 * \retval the burn flash event.
 * 
 **/
acl_burn_flash_event_t ACL_Get_burn_flash_event(Subject_t *s)
{
    // Validate the subject
    ASSERT(VALIDATE_OBJECT_KEY(s, 
                               ACL_burn_flash_event_Subject_t, 
                               subject_observer_oid_key, 
                               ACL_BURN_FLASH_EVENT_ID));

    // Fetch and return the value from the subject structure
    return (ACL_burn_flash_event()->burn_flash_event);
}
#endif // #if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)

/* FUNCTION NAME: AclBurnFlash */
 
/** 
 * \brief Burn the flash with the new file from the host.
 * 
 * \param[in] Pipe  IO Pipe for reporting results to the host
 * 
 * \param[in] AclCmd The command block that caused this to be called.
 * 
 * \param reserved Save for later
 * 
 * \retval int32_t 0 success, 1 failure.
 * 
 * \author Dirk Bodily
 * 
 * \date 2/27/2006
 * 
 **/
#if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)
error_type_t AclBurnFlash(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void * cmd_data)
{
    unsigned char *DataBuf;
    ATSTATUS status=AT_SUCCESS;
    int32_t bytesRead;
    uint32_t size;
    acl_response_base_struct_t *Buffer;

    Buffer = (acl_response_base_struct_t *) construct_acl_response_buffer(ACLCMD_BURN_FLASH);
    // construct_acl_response_buffer should never return NULL
    ASSERT( NULL != Buffer );

    size = be32_to_cpu( ((burnFlashCmd_t *)AclCmd)->dataLen );

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ACLCMD_BURN_FLASH Download is %d bytes\n", size));

    DataBuf = NULL;

    if ( size != 0  )
    {
        DataBuf = (unsigned char *) MEM_MALLOC_ALIGN( size, cpu_get_dcache_line_size() );

        if ( DataBuf != NULL )
        {
            bytesRead = Pipe->ReadConnect(Pipe, DataBuf, size, PARSEREAD_EXACT );
            if (bytesRead < 0)
                bytesRead = 0;      // force an error if the read failed.

        }
        else
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Fail to allocate memory for DataBuf!!!\n"));
            bytesRead = 0;
        }

        if (bytesRead != size)
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Wrong number of bytes read, bytesRead=%d, abort\n", bytesRead));
            ACL_Notify_burn_flash_event(BURN_FLASH_EVENT_UPGRADE_ERROR);
            status = AT_FAILURE;
        }

        // write the flash; this routine will check the checksum, so we pass it in
        if ( ( status == AT_SUCCESS ) && ( bytesRead != 0 ) )
        {
            ASSERT( NULL != DataBuf );

            status = ACLBurnFlash(DataBuf, size, 
                                  be32_to_cpu( ((burnFlashCmd_t *)AclCmd)->offset ),
                                  be16_to_cpu( ((burnFlashCmd_t *)AclCmd)->destination ),
                                  be16_to_cpu( ((burnFlashCmd_t *)AclCmd)->checkSum ) );

        }
    }
    // both SUCCESS & PENDING are successfull, PENDING is just don't reboot..
    if((status != AT_SUCCESS) && (status != AT_PENDING))
    {
        ACL_Notify_burn_flash_event(BURN_FLASH_EVENT_UPGRADE_ERROR);
    }
    else
    {
        ACL_Notify_burn_flash_event(BURN_FLASH_EVENT_UPGRADE_COMPLETE);
    }

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);

    if ( ( size != 0 ) && ( NULL != DataBuf ) )
    {
        MEM_FREE_AND_NULL(DataBuf);
    }

    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent %d bytes\n", LEN_ACL_CMD));

    // wait for a bit to let the response get out then reset
    // note that we only reset if the burn was successful
    if (status == AT_SUCCESS)
    {
        sleep(1);
#ifdef __linux__
        //??????? hwPlatformReset();  // reset the entire block
#else
        hwPlatformReset();  // reset the entire block
#endif        
    }

    return 0;
}

#endif


