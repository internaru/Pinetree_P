/*
 ***************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ****************************************************************************** 
 */
/**
 *
 * \file uio_lib.h
 *
 * \brief Function prototypes and defines for the common UIO helper library
 * 
 **/

#ifdef HAVE_UIO_LIB

#ifndef UIO_LIB_H
#define UIO_LIB_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *
 * API Data Structures
 *
 */
 
/**
 *
 * \brief UIO map definitions.
 * 
 **/
typedef struct _uio_map_s {
	int             num;            /*!< The number of the mapping for the UIO device [uioX/maps/map{num}] */
    char*           name;           /*!< A string identifier for this mapping. This is optional, 
                                         the string can be empty. Drivers can set this to make it 
                                         easier for userspace to find the correct mapping. */
	unsigned long   phy_addr;       /*!< The hardware address of memory. */
	void*           virt_addr;      /*!< The userspace address of memory. */
	unsigned long   size;           /*!< The size, in bytes, of the memory. */
} uio_map_t;

/**
 *
 * \brief UIO device definitions.
 * 
 **/
typedef struct _uio_dev_s uio_dev_t;

/**
 *
 * \brief typedef defines a interrupt handler to be executed once the desired 
 * interrupt event triggered.  The handler parameter used for the 
 * uio_int_attach function must be of this type.
 *
 * \param[in] interrupt_count The interrupt count of your device. If the value is 
 * one more than the value you read the last time, everything is OK. If the 
 * difference is greater than one, you missed interrupts.
 *
 * \param[in] context The context passed into the uio_int_attach call.
 *
 */
typedef void (*UIO_PFN_ISR)( int32_t interrupt_count, void *context );


/**
 *
 * \brief Initializes the UIO driver interface. 
 *
 */
void uio_lib_init(void);

/**
 *
 * \brief  Find, open and initialize the UIO device.
 *
 * \param[in] name The name of the UIO device to open.
 *
 * \return void *
 * \retval Returns a pointer to a valid UIO device handle
 * \retval NULL upon failure
 *
 **/
uio_dev_t* uio_open_dev(const char *name);

/**
 *
 * \brief  Get the UIO device name.
 *
 * \param[in] hdev Valid handle returned by the uio_open_dev function.
 *
 * \return char*
 * \retval Returns the name of the UIO device.
 * \retval -1 upon failure
 *
 **/
char *uio_get_dev_name(uio_dev_t *hdev);

/**
 *
 * \brief  Get the UIO device file descriptor.
 *
 * \param[in] hdev Valid handle returned by the uio_open_dev function.
 *
 * \return int
 * \retval Returns the UIO device file descriptor.
 * \retval -1 upon failure
 *
 **/
int uio_get_dev_fd(uio_dev_t *hdev);

/**
 *
 * \brief  Close the UIO device.
 *
 * \param[in] hdev Valid handle returned by the uio_open_dev function.
 *
 * \return void
 *
 **/
void uio_close_dev(uio_dev_t *hdev);

/**
 *
 * \brief  Get number of mappings defined by the device.
 *
 * \param[in] hdev Valid handle returned by the uio_open_dev function.
 *
 * \return int
 * \retval Returns the number of mappings defined by the device.
 * \retval -1 upon failure
 *
 **/
int uio_get_map_count(uio_dev_t *hdev);

/**
 *
 * \brief  Get the specified UIO map definitions for a device 
 *
 * \param[in] hdev Valid handle returned by the uio_open_dev function.
 *
 * \param[in] index Index of the map [0..{n}]
 *
 * \return uio_map_t *
 * \retval Returns a pointer to a valid UIO map definition structure.
 * \retval NULL if the index is not valid.
 *
 **/
uio_map_t* uio_get_map(uio_dev_t *hdev, int index);

/**
 *
 * \brief Attach an interrupt handler to a UIO device interrupt.
 *
 * Allows calling systems to attach an interrupt handler to a UIO device
 * interrupt.  Note that this interrupt handler is disabled by default 
 * until uio_int_enable() is called.
 *
 * \param[in] hdev Valid UIO device handle that triggers the interrupt 
 * to attach to.
 *
 * \param[in] priority The desired priority of the interrupt (ignored right now)
 *
 * \param[in] handler A pointer to a function that will be invoked after the 
 * UIO device interrupt occurs.
 * 
 * \param[in] context This value will be passed through to the context
 * parameter of the interrupt handler when called.
 * 
 * \return int
 * \retval 0 upon success
 * \retval <0 upon failure
 * 
 */
int uio_int_attach(uio_dev_t *hdev, uint32_t priority, UIO_PFN_ISR handler, void *context);

/**
 *
 * \brief Detach an interrupt handler from a UIO device interrupt.
 *
 * \param[in] hdev Valid UIO device handle.
 *
 * \return int
 * \retval 0 upon success
 * \retval <0 upon failure
 * 
 * \note the UIO device interrupt is disabled after calling this function.
 * 
 */
int uio_int_detach(uio_dev_t *hdev);

/**
 *
 * \brief Enables the previously attached interrupt handler for a UIO device.
 *
 * \param[in] hdev Valid UIO device handle.
 *
 * \return int
 * \retval 0 upon success
 * \retval <0 upon failure
 * 
 * \note the UIO device interrupt is disabled after calling this function.
 * 
 */
int uio_int_enable(uio_dev_t *hdev);

/**
 *
 * \brief Disables the previously attached interrupt handler for a UIO device.
 *
 * \param[in] hdev Valid UIO device handle.
 *
 * \return int
 * \retval 0 upon success
 * \retval <0 upon failure
 * 
 * \note the UIO device interrupt is disabled after calling this function.
 * 
 */
int uio_int_disable(uio_dev_t *hdev);

#ifdef __cplusplus
}
#endif

#endif // UIO_LIB_H
#endif // HAVE_UIO_LIB
