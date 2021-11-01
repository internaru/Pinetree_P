/*
 * ============================================================================
 * (C) Copyright 2009, 2013  Marvell International Ltd.
 *                           All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include <stdint.h>
#include "ioutils.h"

#ifdef CPU_BIG_ENDIAN
/** \brief Handle the endian swap stuff for a big endian cpu. */
uint32_t cpu_to_be32(uint32_t val)
{
    return val;
}
uint32_t cpu_to_le32(uint32_t val)
{
    return bswap_32(val);
}

uint32_t be32_to_cpu(uint32_t val)
{
    return val;
}

uint32_t le32_to_cpu(uint32_t val)
{
    return bswap_32(val);
}

uint16_t cpu_to_be16(uint16_t val)
{
    return val;
}

uint16_t cpu_to_le16(uint16_t val)
{
    return bswap_16(val);
}

uint16_t be16_to_cpu(uint16_t val)
{
    return val;
}

uint16_t le16_to_cpu(uint16_t val)
{
    return bswap_16(val);
}
#endif

#ifdef CPU_LITTLE_ENDIAN
/** \brief Handle the endian swap stuff for a little endian cpu. */
extern inline uint32_t cpu_to_be32(uint32_t val);
extern inline uint32_t cpu_to_le32(uint32_t val);
extern inline uint32_t be32_to_cpu(uint32_t val);
extern inline uint32_t le32_to_cpu(uint32_t val);
extern inline uint16_t cpu_to_be16(uint16_t val);
extern inline uint16_t cpu_to_le16(uint16_t val);
extern inline uint16_t be16_to_cpu(uint16_t val);
extern inline uint16_t le16_to_cpu(uint16_t val);
#endif

void *ioremap(unsigned long phys_addr, unsigned long size)
{
    return (void *)phys_addr;
}

void iounmap(const void *address)
{
}


uint8_t ioread8(const void *addr)
{
    return *((volatile uint8_t *) addr);
}


uint16_t ioread16(const void *addr)
{
    return *((volatile uint16_t *)addr);
}


uint32_t ioread32(const void *addr)
{
    return *((volatile uint32_t *)addr);
}



void iowrite8(uint8_t value, const void *addr)
{
    *((volatile uint8_t *)addr) = value;
}


void iowrite16(uint16_t value, const void *addr)
{
    *((volatile uint16_t *)addr) = value;
}


void iowrite32(uint32_t value, const void *addr)
{
    *((volatile uint32_t *)addr) = value;
}


void ioread8_rep(const void *addr, void *buf, unsigned long count)
{
    uint8_t *mybuf = (uint8_t *)buf;
    volatile uint8_t *myaddr = (uint8_t *) addr;
    while(count-- > 0)
    {
        *mybuf++ = *myaddr;
    }
}


void ioread16_rep(const void *addr, void *buf, unsigned long count)
{
    uint16_t *mybuf = (uint16_t *)buf;
    volatile uint16_t *myaddr = (uint16_t *) addr;
    while(count-- > 0)
    {
        *mybuf++ = *myaddr;
    }
}


void ioread32_rep(const void *addr, void volatile *buf, unsigned long count)
{
    uint32_t *mybuf = (uint32_t *)buf;
    volatile uint32_t *myaddr = (uint32_t *) addr;
    while(count-- > 0)
    {
        *mybuf++ = *myaddr;
    }
}


void iowrite8_rep(const void *addr, const void *buf, unsigned long count)
{
    uint8_t *mybuf = (uint8_t *)buf;
    volatile uint8_t *myaddr = (uint8_t *) addr;
    while(count-- > 0)
    {
         *myaddr = *mybuf++;
    }
}


void iowrite16_rep(const void *addr, const void *buf, unsigned long count)
{
    uint16_t *mybuf = (uint16_t *)buf;
    volatile uint16_t *myaddr = (uint16_t *) addr;
    while(count-- > 0)
    {
         *myaddr = *mybuf++;
    }
}


void iowrite32_rep(const void *addr, const void *buf, unsigned long count)
{
    uint32_t *mybuf = (uint32_t *)buf;
    volatile uint32_t *myaddr = (uint32_t *) addr;
    while(count-- > 0)
    {
         *myaddr = *mybuf++;
    }
}


void memset_io(const void *addr, uint8_t value, unsigned int count)
{
    volatile uint8_t *myaddr = (uint8_t *) addr;
    while(count-- > 0)
    {
        *myaddr++ = value;
    }
}


void memcpy_fromio(void *dest, void *source, unsigned int count)
{
    uint8_t *mydest = (uint8_t *) dest;
    volatile uint8_t *mysource = (uint8_t *) source;
    while(count-- > 0)
    {
        *mydest++ = *mysource++;
    }
}

void memcpy_toio(void volatile *dest, void *source, unsigned int count)
{
    uint8_t *mydest = (uint8_t *) dest;
    uint8_t *mysource = (uint8_t *) source;
    while(count-- > 0)
    {
        *mydest++ = *mysource++;
    }
}

