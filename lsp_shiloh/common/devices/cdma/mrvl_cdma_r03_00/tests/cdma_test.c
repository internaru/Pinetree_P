/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief This file contains etc. - one or two sentences.
 *
 * Longer description.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "ATypes.h"
#include "tx_api.h"
#include "memAPI.h"
#include "string.h" // used for memcpy and memset
#include "lassert.h"
#include "logger.h"
#include "cdma_api.h"
#include "timer_api.h"

#if defined (HAVE_CMD) || defined (HAVE_UNIT_TEST)
// Test size must be a power of 4 or the test will fail due to the way it is written
#define DMA_TEST_SIZE 16*1024*1024

int cdma_test(void)
{
    uint32_t start1, start2, end1, end2, test_size = DMA_TEST_SIZE, i;
    bool flag = FALSE;
    cdma_handle_t *handle = NULL;
    CDMA_CONFIG MyCfg;
    uint8_t *destbuf, *srcbuf;
    int result = 0;

    /* ballen 12/2/2011 -- make destbuf and srcbuf wait for memory */
    while(1)
    {
        destbuf = (uint8_t *)MEM_MALLOC_ALIGN((sizeof(uint8_t) * DMA_TEST_SIZE), cpu_get_dcache_line_size());
        if(destbuf)
            break;
        tx_thread_sleep(20);
    }

    while(1)
    {
        srcbuf = (uint8_t *)MEM_MALLOC_ALIGN((sizeof(uint8_t) * DMA_TEST_SIZE), cpu_get_dcache_line_size());
        if(srcbuf)
            break;
        tx_thread_sleep(20);
    }

    ASSERT( destbuf != NULL );
    ASSERT( srcbuf != NULL );

    MyCfg.SourceEndian = CDMA_LITTLE_ENDIAN;
    MyCfg.DestEndian = CDMA_LITTLE_ENDIAN;
    MyCfg.SrcBurst = CDMA_BURST_8;
    MyCfg.DestBurst = CDMA_BURST_8;
    MyCfg.SrcXferWidth = CDMA_32_BITS;
    MyCfg.DestXferWidth = CDMA_32_BITS;
    MyCfg.FlowControl = CDMA_MEM_TO_MEM;
    MyCfg.SrcIncrement = CDMA_INC;
    MyCfg.DestIncrement = CDMA_INC;
    MyCfg.DestModify = CDMA_NO_MOD_DEST;
    MyCfg.TimeBase = CDMA_1_US;

    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("\n  bytes * CDMA clock count * memcpy clock count\n"));

    memset(srcbuf,0, DMA_TEST_SIZE);

    for(i = 0; i < DMA_TEST_SIZE; i = i + 4) {
        srcbuf[i + 0] = (i & 0x000000ff);
        srcbuf[i + 1] = (i & 0x0000ff00) >> 8;
        srcbuf[i + 2] = (i & 0x00ff0000) >> 16;
        srcbuf[i + 3] = (i & 0xff000000) >> 24;
    }

    while(test_size) {

        memset(destbuf,0, DMA_TEST_SIZE);

        if(CdmaOpen(CDMA_FIND_CHANNEL, &handle) == OK) {
            if(CdmaSetConfig(handle, &MyCfg) == FAIL) {
                result = __LINE__;
                goto bail;
            }
    
            memset(&MyCfg,0,sizeof(MyCfg));
            if(CdmaGetConfig(handle, &MyCfg) == FAIL) {
                result = __LINE__;
                goto bail;
            }

            start1 = timer_get_time_usec();
            if(test_size != CdmaTransfer(handle, (uint8_t *)srcbuf, (uint8_t *)destbuf, test_size, 0, NULL, NULL)) 
            {
                DPRINTF((DBG_LOUD|DBG_OUTPUT), ("INCOMPLETE CDMA transfer\n"));
                result = __LINE__;
                goto bail;
            }
            end1 = timer_get_time_usec();
            if(CdmaClose(handle) == FAIL) {
                result = __LINE__;
                goto bail;
            }

        } else {
            result = __LINE__;
            goto bail;
        }

        
        if (test_size >= 512)
            for(i = 0; i < test_size; i = i + 4)
                if (!(destbuf[i + 0] == (i & 0x000000ff) &&
                      destbuf[i + 1] == (i & 0x0000ff00) >> 8 &&
                      destbuf[i + 2] == (i & 0x00ff0000) >> 16 &&
                      destbuf[i + 3] == (i & 0xff000000) >> 24)) {
                    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("%7d  COMPARE MISMATCH %d\n", i));
                    result = __LINE__;
                    goto bail;
                }
                
        start2 = timer_get_time_usec();                
        memcpy(destbuf, srcbuf, test_size);
        
        //cpu_dcache_writeback_region(destbuf, test_size); 
        
        end2 = timer_get_time_usec();
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("%7d           %8d             %8d", test_size, end1-start1, end2-start2));
        if (!flag  && (end2 - start2 < end1-start1)) {
            DPRINTF((DBG_LOUD|DBG_OUTPUT), (" <<<"));
            flag = TRUE;
        }

        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("\n"));
        test_size >>= 1;
		test_size -= test_size % 4;  // Keep it 4 byte aligned
    }

bail:
    MEM_FREE_AND_NULL(destbuf);
    MEM_FREE_AND_NULL(srcbuf);

    return result;
}
#endif
