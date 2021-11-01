/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file nvram_api.h
 *
 * \brief External interface definitions for the NVM Manager API
 *
 * The Non-Volatile Memory Manager handles the storage, retrieval and initialization
 * of non-volatile variables.  Variables are grouped into blocks (which may 
 * correspond to code modules) and are referenced by a 4 character ASCII label.
 * For example, the scan module might use "SCAN" and the print module might use
 * "PRNT".  The label should only use numbers and upper/lower case letters.
 *
 * Interactions with the NVM Manager begin
 * with a call to nvram_variable_register() to obtain a handle to a variable
 * block.  Note that once registered, there is no API method to delete a 
 * variable block.
 * 
 * Once the block owner has a valid handle:
 *  - the block can be read from the NV store.  This returns a complete copy of
 *    the block; the owner now has a RAM copy that can be used to access variables
 *    without re-reading the physical NV store.
 *  - the block can be written to the NV store.  If the owner makes changes to 
 *    their RAM copy, it can be committed to the NV store.
 *
 * The block owner provided init function will be called in the following 
 * situations:
 *  - nvram_variable_register(): the block existed in the NV store, but the 
 *    CRC (e_NvramCRCErr), Version (e_NvramVerErr), or Length (e_NvramLenErr) 
 *    did not match the parameters in the register call
 *  - nvram_variable_register(): the block did not exist in the NV store, so we
 *    need to initialize a new block (e_NvramDefault)
 *  - nvram_get_var_ptr(): we read the block from the NV store, but a CRC check
 *    failed (e_NvramCRCErr)
 *
 * Architecturally this API is considered to be at the application layer
 * and does not have detailed knowledge of the underlying memory hardware:
 *      +---------------------------------------------------+
 *      |            NVRAM API (Application Layer)          |
 *      |                     (THIS FILE)                   |
 *      +---------------------------------------------------+
 *      |                NV Device Shim Layer               |
 *      +------------------+---------------+----------------+
 *      |  EEPROM Support  |  SPI Support  |  NAND Support  |
 *      +------------------+---------------+----------------+
 *      |                Partition Manager API              |
 *      +---------------------------------------------------+
 *      |              Memory Device Driver Layer           |
 *      +---------------------------------------------------+
 *
 **/

#ifndef INC_NVRAMAPI_H
#define INC_NVRAMAPI_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *  \brief NVM Instance Handle
 *
 *  Externally this will look like a void pointer, but internal files that include
 *  nvram_dev_shim.h will have full access to the structure members.
 **/
typedef struct NVM_Handle_s nvram_handle_t;


/**
 *  \brief Possible NVM restore conditions
 *
 *  These restore conditions are passed back to the block owners NvramInitFunction
 *  when the NVM Manager detects a problem with the variable block.  This context
 *  assists the block owner in taking the proper action to restore the variables.
 **/
typedef enum
{
    NVRAM_NEW,                  /**< Did not find an existing block */
    NVRAM_ERROR,                /**< Found existing block, but errors were detected */
    NVRAM_VERSION_CHANGE,       /**< Found existing block, but version has changed */
    NVRAM_OK,                   /**< Found existing block with no errors or changes */
    NVRAM_FACTORY_DEFAULTS,     /**< The state before any board customization 
                                     wipes out all user settings, page counts, warranty
                                     information language settings.  Anything in 
                                     nvram variables is gone.  Does not touch mac address
                                     usb id, pid, vid etc. */
    NVRAM_LANG_DEFAULTS,        /**< wipes out all user settings, language/location
                                     settings */
    NVRAM_USER_DEFAULTS,        /**< wipes out all user settings */
} nvram_init_type_t;


/**
 *  \brief Init function pointer including user data.  This is a parameter to the
 *         nvram_variable_register function
 *
 *  NV variable block init functions must be provided by each block owner to
 *  initialize/recover variables when needed.  The following typedef defines
 *  the init function signature.
 *  
 * \param[in] InitType The reason this function is being called.
 * \param[in] Version If the InitType is e_NvramDefault then this Version is the Version 
 *                    that is to be initialized.  ie the version passed into the register function
 *                    Anything else this is the version that we found.
 * \param[in,out] InitLocation This is the buffer to fill with the data.  This has been allocated
 *                   by the calling function.  If the error is anything but e_NvramDefault this
 *                   also contains the old data.
 * \param[in] InitLocationSize The size of the buffer associated 
 *       with InitLocation; use this only for error checking
 *       when writing to InitLocation.
 * \param[in] UserData Opaque data that was provided by the user in the call to
 *                     nvram_variable_register().
 * \returns error_type_t
 * \retval OK The init was successful
 * \retval !OK The init failed, clean up and return variable not opened.
 * 
 **/
typedef error_type_t (*nvram_init_func_ptr)(nvram_init_type_t   InitType,
                                      uint16_t    Version,
                                      void       *InitLocation,
                                      uint32_t    InitLocationSize,
                                      void       *UserData);


/**
 *  \brief Initialize the NVM Manager interface
 *
 *  This routine should be called once at start-up to initialize the NVM Manager.
 *  The lower level memory device drivers and partition manager must be 
 *  initialized prior to calling this routine.
 *
 *  After internal state variables are initialized, the system will search
 *  for any existing variable blocks.
 *
 * \return error_type_t 
 * \retval OK     Initialization successful
 * \retval FAIL   Initialization failed
 *
 **/
error_type_t nvram_sys_init(void);


/**
 *  \brief Register a block of variables with the NVM Manager.
 *
 *  This function is used to register a block of variables with the NVM
 *  Memory manager and obtain a handle to those variables.  Note that 
 *  this registration call does not directly provide the variable definitions
 *  or values (they will come from either the NVM storage or the supplied
 *  NvramInitFunction function).
 *
 *  We start by trying to locate the variable block in the NVM storage. If found
 *  then the crc, version, and size of the stored block are compared to the values
 *  provided in the registration call.  If everything matches then a handle to
 *  the existing variables is returned to the caller.
 *
 *  If the block was not found or the data validation failed, the block owners
 *  NvramInitFunction is called (with the appropriate restore condition) to 
 *  initialize a new variable block.  A handle to the new block is returned to
 *  the caller.
 *
 *  \warning There is currently no logic to detect block label collisions.
 *
 *  \param[out] **Handle Pointer to a pointer where the nvram_handle_t will be 
 *                       stored.  The NVM Manager will allocate the memory needed
 *                       to store the handle.
 *
 *  \param[in] *Label    Pointer to a unique 4 character ASCII string to identify
 *                       the variable block.
 *
 *  \param[in]  Version  The version of the variable block to register
 *
 *  \param[in]  VarBlockLength The total length of the variable block in bytes
 *
 *  \param[in]  NvramInitFunction()
 *                       Identical function signature to NvramInitFunction() with the
 *                       addition of a UserData parameter.
 *  
 *  \param[in]  UserData Opaque user data that will be returned to the user in the
 *                       callback of the registered
 *                       NvramInitFunction() function.
 *
 *  \return error_type_t
 *  \retval OK    Block registration succeeded, the return handle is valid 
 *  \retval FAIL  Registration failure, the return handle is not valid
 *
 **/
error_type_t nvram_variable_register(nvram_handle_t  **Handle,
                                      char           *Label,
                                      uint16_t        Version,
                                      uint32_t        VarBlockLength,
                                      nvram_init_func_ptr NvramInitFunction,
                                      void           *UserData);

/**
 * \brief Free the previously registered variable block.
 * After a variable block is registered, this function is called to unregister that variable block
 * \param[in] nHandle The handle value returned from nvram_variable_register
 * \retval void
 */

void nvram_variable_unregister(nvram_handle_t *nHandle);


/**
 *  \brief Write data to a stored variable block
 *
 *  This routine will write the supplied variable buffer to a previously
 *  registered variable block.
 *
 *  \param[in] *handle      The handle returned from nvram_variable_register()
 *
 *  \param[in] *SrcLocation Pointer to a buffer containing the variables to write
 *                          (allocated by caller)
 *
 *  \return error_type_t 
 *  \retval OK    Variable block successfully written
 *  \retval FAIL  Block write failed
 *
 */
error_type_t nvram_set_var(nvram_handle_t *handle, void *SrcLocation);


/**
 *  \brief Blocks until all queued NV writes complete
 *
 *  The NVM Manager queues device write operations to a background thread.
 *  Some system conditions may need to flush this queue before proceeding 
 *  (a reboot for example).
 *
 *  \return error_type_t 
 *  \retval OK    Write queue flushed
 *
 */
error_type_t nvram_flush(void);

/**
 * \brief Reset all the registered nvram vars to a given type.
 * 
 * \param[in] reset_type See nvram_init_type_t for description of this parameter.
 * 
 */
error_type_t nvram_reset_all_vars(nvram_init_type_t reset_type);

/**
 * \brief Shut down the nvram system
 */
void nvram_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_NVRAMAPI_H */
