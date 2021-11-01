/*
 * ============================================================================
 * (C) Copyright 2009-2010   Marvell International Ltd.  All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /**
  * \brief define an io interface for accessing registers in a consistent manner 
  * that will take care of endian issues.
  *
  * There are 2 parts to this.  
  * -#  The first section gives a series of programs to convert from cpu 
  *     endianess to big or little endianess.
  * -#  The last section is for read and writing to io addresses.  The first
  *     thing to do is to use ioremap to get the address of the io section
  *     The input is the address of the io block in our memory.  What this
  *     returns is the same thing.  This is added for other platforms that 
  *     may require that we map from an io section to regular memory.  That
  *     address is assigned to a variable that is the type of a structure 
  *     that defines the registers.  Then to use ioread32 for example do
  *     isread32(\&\<struct\>->register) the returned value is a 32 bit value
  *     that is byte swapped if needed.
  * 
  */


#ifndef _IOUTILS_H
#define _IOUTILS_H
#include "ATypes.h"
#include <stdint.h>
#include <byteswap.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CPU_BIG_ENDIAN
/**
 * \brief do endian conversions from cpu to data and back
 * \param[in] val the value to swap
 * \return the swapped value
 */
static inline uint32_t cpu_to_be32(uint32_t val)
{
    return val;
}
static inline uint32_t cpu_to_le32(uint32_t val)
{
    return bswap_32(val);
}

static inline uint32_t be32_to_cpu(uint32_t val)
{
    return val;
}

static inline uint32_t le32_to_cpu(uint32_t val)
{
    return bswap_32(val);
}

static inline uint16_t cpu_to_be16(uint16_t val)
{
    return val;
}

static inline uint16_t cpu_to_le16(uint16_t val)
{
    return bswap_16(val);
}

static inline uint16_t be16_to_cpu(uint16_t val)
{
    return val;
}

static inline uint16_t le16_to_cpu(uint16_t val)
{
    return bswap_16(val);
}
#endif

#ifdef CPU_LITTLE_ENDIAN
/**
 * \brief do endian conversions from cpu to data and back
 * \param[in] val the value to swap
 * \return the swapped value
 */
static inline uint32_t cpu_to_be32(uint32_t val)
{
    return bswap_32(val);
}
static inline uint32_t cpu_to_le32(uint32_t val)
{
    return val;
}

static inline uint32_t be32_to_cpu(uint32_t val)
{
    return bswap_32(val);
}

static inline uint32_t le32_to_cpu(uint32_t val)
{
    return val;
}

static inline uint16_t cpu_to_be16(uint16_t val)
{
    return bswap_16(val);
}

static inline uint16_t cpu_to_le16(uint16_t val)
{
    return val;
}

static inline uint16_t be16_to_cpu(uint16_t val)
{
    return bswap_16(val);
}

static inline uint16_t le16_to_cpu(uint16_t val)
{
    return val;
}
#endif

/**
 * \brief Map a physical io address into a virtual address.
 *
 * To access I/O memory, drivers need to have a virtual address that the
 * processor can handle.  ioremap satisfy this need.  Make sure that you don't
 * use this addressdirectly.  Use the following routines to access the memory.
 *
 * \param[in] phys_addr Physical address to map into our virtual space.
 * \param[in] size The length of the region to map
 * \return void *
 * \retval Virtual address or NULL for no mapping.
 */
void *ioremap(unsigned long phys_addr, unsigned long size);

/**
 * \brief unmap a virtual address mapped by ioremap.
 * \param[in] address value returned form an ioremap call.
 * \return nothing.
 */
void iounmap(const void *address);

/**
 * \brief Read a byte from an io address space.
 * \param[in] addr A pointer returned from the ioremap function
 * \return byte
 * \retval The byte read from the io memory area.
 */

uint8_t ioread8(const void *addr);

/**
 * \brief Read a short from an io address space and return it in cpu byte
 * ordering.
 *
 * \param[in] addr A pointer returned from the ioremap function
 * \return short
 * \retval The short read from the io memory area.
 */
uint16_t ioread16(const void *addr);

/**
 * \brief Read a 32 bit value from an io address space and return it in cpu byte
 * ordering.
 *
 * \param[in] addr A pointer returned from the ioremap function
 * \return uint32
 * \retval The uint32 read from the io memory area.
 */
uint32_t ioread32(const void *addr);

/**
 * \brief write a byte to an io address space.
 * \param value The byte to write
 * \param[in] addr A pointer returned from ioremap
 */

void iowrite8(uint8_t value, const void *addr);

/**
 * \brief Write a short 16bits to an io address space swapped to correct byte
 * order.
 *
 * \param[in] value The 16 bit value to write
 * \param[in] addr A pointer returned from ioremap
 */

void iowrite16(uint16_t value, const void *addr);

/**
 * \brief Write a uint 32 bits to an io address space swapped to correct byte
 * order.
 *
 * \param[in] value The 32 bit value to write
 * \param[in] addr A pointer returned from ioremap
 */

void iowrite32(uint32_t value, const void *addr);

/**
 * \brief Read count bytes from addr to memory buf.  
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf Where the bytes are to be placed.
 * \param[in] count The number of 8 bit values to read
 */
void ioread8_rep(const void *addr, void *buf, unsigned long count);

/**
 * \brief Read count 16 bit values from addr to memory buf.
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf A buffer of 16 bit values to read into
 * \param[in] count The number of 16 bit values to read
 */

void ioread16_rep(const void *addr, void *buf, unsigned long count);

/**
 * \brief Read count 32 bit values from addr to memory buf.
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf A buffer of 32 bit values to read into.
 * \param[in] count The number of 32 bit values to read
 */

void ioread32_rep(const void *addr, void volatile *buf, unsigned long count);

/**
 * \brief Write count byte values to addr.
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf A buffer of 8 bit values to write.
 * \param[in] count The number of 8 bit values to write
 */
void iowrite8_rep(const void *addr, const void *buf, unsigned long count);

/**
 * \brief Write count 16 bit values to addr.
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf A buffer of 16 bit values to write.
 * \param[in] count The number of 16 bit values to write
 */
void iowrite16_rep(const void *addr, const void *buf, unsigned long count);

/**
 * \brief Write count 32 bit values to addr.
 *
 * buf is incremented, addr is not.
 *
 * \param[in] addr The address returned by an ioremap function
 * \param[in] buf A buffer of 32 bit values to write.
 * \param[in] count The number of 32 bit values to write
 */
void iowrite32_rep(const void *addr, const void *buf, unsigned long count);

/**
 * \brief Do a memset to an io region.
 *
 * \param[in] addr The address returned from an ioremap function
 * \param[in] value The valup to splat over the io region
 * \param[in] count the number of bytes to write in the io region
 */
void memset_io(const void *addr, uint8_t value, unsigned int count);

/**
 * \brief Copy bytes from an io region to local memory byte at a time.
 *
 * \param dest The address in local memory to put the data
 * \param[in] source The address in io memory returned from an ioremap call.
 * \param[in] count The number of bytes to copy
 */

void memcpy_fromio(void *dest, void *source, unsigned int count);

/**
 * \brief Copy bytes from local memory to an io region.
 *
 * \param[in] dest The address in the io memory to put the bytes
 * \param[in] source The address in our memory to get the data
 * \param[in] count The number of bytes to write
 */
void memcpy_toio(void volatile *dest, void *source, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif
