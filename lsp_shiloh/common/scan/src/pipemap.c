/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "list.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "safetylock.h"
#include "icedma.h"
#include "scandbg.h"
#include "pipemap.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "pie.h"

/* PIC WDMA <---> PIE RDMA */
uint8_t pipemap_pic_to_pie_channel( struct ice_dma_buffer *icebuf )
{
    uint8_t pic_channel;
    static int pic2pie_channels[ PIC_WDMA_NUM_CHANNELS ] = {
#ifdef HAVE_STAGGERED_SENSOR_SUPPORT
        PIE_RDMA_RED_EVEN,
        PIE_RDMA_RED_ODD,
        PIE_RDMA_GREEN_EVEN,
        PIE_RDMA_GREEN_ODD,
        PIE_RDMA_BLUE_EVEN,
        PIE_RDMA_BLUE_ODD
#elif defined HAVE_NSENSOR_SUPPORT
        /* davep 30-Jan-2013 ; dual sensor support requires re-wiring PIC WDMA
         * routing a bit. PIE still only has three read DMA channels but PIC
         * WDMA has six. (!)
         */
        PIE_RDMA_RED_EVEN,
        -1,
        PIE_RDMA_GREEN_EVEN,
        -1,
        PIE_RDMA_BLUE_EVEN,
        -1,
#else
        PIE_RDMA_RED_EVEN,
        PIE_RDMA_GREEN_EVEN,
        PIE_RDMA_BLUE_EVEN,
#endif
    };

    pic_channel = icebuf->channel;
    XASSERT( pic_channel < PIC_WDMA_NUM_CHANNELS, pic_channel );

    /* davep 22-May-2013 ; as of this writing, the only ASIC with nsensor
     * support has 6 PIC WDMA channels [0-5] and 3 PIE RDMA channels [0-2].
     * Need to map [0-5] to [0-2] in the most sanitary way possible.
     * Even PIC channels are sensor #0.  Odd PIC channels are sensor #1.
     * Everything maps to PIE [0,1,2].
     */
#ifdef HAVE_NSENSOR_SUPPORT
    XASSERT( pic_channel < 6, pic_channel );
    return pic_channel / 2;
#endif

    return pic2pie_channels[pic_channel];
}

uint8_t pipemap_pie_to_pic_channel( struct ice_dma_buffer *icebuf )
{
    uint8_t pie_channel;
    static int pie2pic_channels[ PIE_RDMA_NUM_CHANNELS ] = {
#ifdef HAVE_STAGGERED_SENSOR_SUPPORT
        PIC_DMA_CHANNEL_CEVEN_0,
        PIC_DMA_CHANNEL_CODD_0,
        PIC_DMA_CHANNEL_CEVEN_1,
        PIC_DMA_CHANNEL_CODD_1,
        PIC_DMA_CHANNEL_CEVEN_2,
        PIC_DMA_CHANNEL_CODD_2
#else
    #if PIE_RDMA_NUM_CHANNELS==6
        /* davep 29-Mar-2010 ; as of this writing, PIE channels are still 0,2,4
         * in non-staggered ASICs. We'll need "holes" in our map.
         */
        PIC_DMA_CHANNEL_CEVEN_0,
        -1,
        PIC_DMA_CHANNEL_CEVEN_1,
        -1,
        PIC_DMA_CHANNEL_CEVEN_2,
       -1 
    #else
       /* davep 18-Feb-2011 ; pie descriptor DMAs with three RDMA channels */
        PIC_DMA_CHANNEL_CEVEN_0,
        PIC_DMA_CHANNEL_CEVEN_1,
        PIC_DMA_CHANNEL_CEVEN_2,
    #endif
#endif
    };

    pie_channel = icebuf->channel;
    XASSERT( pie_channel < PIE_RDMA_NUM_CHANNELS, pie_channel );

#ifdef HAVE_NSENSOR_SUPPORT
    /* davep 02-Jul-2013 ;map duplex scanners and multiplexed three channel PIE
     * back to six channels of PIC. Note that ASIC after this date will
     * probably require a different mapping. 
     * 
     * PIE  Sensor ->  PIC 
     *  0     0         0
     *  1     0         2
     *  2     0         4
     *  0     1         1
     *  1     1         3
     *  2     1         5
     */

    return pie_channel*2 + icebuf->sensor_num;
#endif

    XASSERT( pie2pic_channels[pie_channel]>=0, pie_channel );

    return pie2pic_channels[pie_channel];
}

/* PIE WDMA <---> PIE-HP RDMA */
uint8_t pipemap_pie_to_piehp_channel( struct ice_dma_buffer *icebuf )
{
    uint8_t pie_channel;

    pie_channel = icebuf->channel;

    XASSERT( pie_channel < PIE_WDMA_NUM_CHANNELS, pie_channel );

    /* HalfPack has only one channel anyway */
    return 0;
}

uint8_t pipemap_piehp_to_pie_channel( struct ice_dma_buffer *icebuf )
{
    uint8_t hp_channel;

    hp_channel = icebuf->channel;

    XASSERT( hp_channel < PIEHP_RDMA_NUM_CHANNELS, hp_channel );

    /* if we're using HalfPack, PIE must be in Mono mode so push this buffer back
     * into the PIE mono DMA channel
     */
    return PIE_WDMA_CHANNEL_MONO;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 13-Jul-2013
 */

void pipemap_get_pic_channels_for_sensor( uint32_t sensor_num, uint8_t channel_array[] )
{
    if( sensor_num==0 ) {
        channel_array[0] = PIC_DMA_CHANNEL_CEVEN_0;
        channel_array[1] = PIC_DMA_CHANNEL_CEVEN_1;
        channel_array[2] = PIC_DMA_CHANNEL_CEVEN_2;
    }
    else { 
        /* TODO */
        XASSERT(0,sensor_num);
    }
}

