
/*
 * ============================================================================
 * (C) Copyright 2009   Marvell International Ltd.
 *                           All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
    *
    * \brief test the utils functionality.
*/

#include <stdint.h>
#include "ATypes.h"

#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "memAPI.h"
#include "error_types.h"
#include "ioutils.h"
#include "base64_api.h"
#include "threads_api.h"
#ifdef HAVE_SYSTEM
#include "file_to_pipe_api.h"
#endif

#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
#endif



#ifdef HAVE_UNIT_TEST
static int testEndian(void);
static int utilsTest( void );
#endif


void utils_init( void )
{
    base64_init();
#ifdef HAVE_CMD
     //LSPTODO threads_init();
#endif
#ifdef HAVE_SYSTEM
    file_to_pipe_init();
#endif
#ifdef HAVE_UNIT_TEST
    error_type_t e_res;
    e_res = unit_test_register_testcase("utils", utilsTest);
    ASSERT( OK == e_res );
#endif
}

#ifdef HAVE_UNIT_TEST
static int utilsTest( void )
{
    int err;
    //
    // Test endian stuff
    //
    err = testEndian();

    return err;
}

#define BUFSIZE 512

/**
 * \brief Test the interface to the io system
 * This tests the endian and io.h interfaces to the io system.
 * \return int
 * \retval 0 successful
 * \retval !=0 The line number of the error.
 */
static int testEndian(void)
{
    int seed=0x12345678;
    int final32, final;
    uint16_t final16;
    uint8_t final8;
    uint8_t *buffer;

    final32 = be32_to_cpu(seed);
    final32 = cpu_to_be32(final32);
    if(seed != final32)
        return __LINE__;
    final32 = le32_to_cpu(seed);
    final32 = cpu_to_le32(final32);
    if(seed != final32)
        return __LINE__;
    
    final16 = be16_to_cpu(seed);
    final16 = cpu_to_be16(final16);
    if((seed& 0xffff) != final16)
        return __LINE__;
    final16 = le16_to_cpu(seed);
    final16 = cpu_to_le16(final16);
    if((seed& 0xffff) != final16)
        return __LINE__;
    final16 = be16_to_cpu(seed);
    final16 = cpu_to_be16(final16);
    final16 = le16_to_cpu(final16);
    final16 = cpu_to_le16(final16);
    if((seed& 0xffff) != final16)
        return __LINE__;
    //
    // Test the io routines
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("Test io functions\n"));
    final32 = 0;
    iowrite32(0xabcdef12, &final32);
    if(final32 != 0xabcdef12)     
        return __LINE__;
    seed = ioread32(&final32);
    if(final32 != seed)
        return __LINE__;
    seed = 0;
    iowrite16(0xfdcb, &final16);
    if( final16 != 0xfdcb)
        return __LINE__;
    seed = ioread16(&final16);
    if(0xfdcb != final16)
        return __LINE__;
    seed = 0;
    iowrite8(0xea, &final8);
    if( final8 != 0xea)
        return __LINE__;
    seed = ioread8(&final8);
    if(0xea != final8)
        return __LINE__;



    buffer= MEM_MALLOC(BUFSIZE);
    if(!buffer)
        return __LINE__;

    for(final=0; final < BUFSIZE; final++)
        buffer[final] = final;
    iowrite8_rep(&final8,buffer, BUFSIZE);
    if(final8 != ((BUFSIZE-1) & 0xff))
    {
        memFree(buffer);
        return __LINE__;
    }

    final8 = 0xa5;
    ioread8_rep(&final8, buffer, BUFSIZE);
    for(final = 0; final < BUFSIZE; final++)
    {
        if(buffer[final] != 0xa5)
        {
            memFree(buffer);
            return __LINE__;
        }
    }
    memset_io(buffer, 0x32, 30);
    for(final = 0; final < 30; final++)
    {
        if(buffer[final] != 0x32)
        {
            memFree(buffer);
            return __LINE__;
        }
    }
    for(final = 0; final < 30; final++)
    {
        buffer[final] = final;
    }
    memcpy_fromio(&buffer[50], buffer, 30);
    memcpy_toio(&buffer[90], buffer, 30);
    for(final = 0; final < 30; final++)
    {
        if((buffer[50+final] != buffer[final]) ||
           (buffer[90+final] != buffer[final]))
        {
            memFree(buffer);
            return __LINE__;
        }
    }
    memFree(buffer);
    
    return 0;
}
#endif
