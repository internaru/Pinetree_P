/****************************************************************************** 
 * Copyright (c) 2012-2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
 
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "uio_lib_api.h"
#include "ATypes.h"
#include "lassert.h"
#include "error_types.h"
#include "asic.h"
#include "logger.h"
#include "zx_idma_api.h"
#include "memAPI.h"


#define DBG_PRFX "ZX_IDMA: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

#define ZX_IDMA_NUM_DESCS 60   ///< max number of descriptors inflight 
#define ZX_RESTART_PENDING_FLAG 0x1
#define ZX_CLEARCOMPLETE_FLAG 0x2

typedef struct zx_stretchblt_cmd_s
{
    uint32_t cmd;
    uint8_t *data_address;
    uint32_t scale;
    uint32_t error;
} zx_stretchblt_cmd_t;

typedef struct zx_desc_s
{
    volatile uint16_t ctrl;
    volatile uint16_t own;
    volatile zx_stretchblt_cmd_t *src;
    volatile uint32_t length;
    volatile struct zx_desc_s *next;
    void* hwaddr; ///< this descriptors hw address.
    BigBuffer_t* bb_ptr; ///< 0 or bb for the data_address
    int   index;  ///< descriptor index
    int   lines;  ///< lines in this descriptor, #lines freed with this block, 32byte padding.
    volatile zx_stretchblt_cmd_t zx_creek_cmd;
    volatile uint8_t pad2[16]; ///< 64byte total for descriptor for alignment.

} zx_desc_t;

#define zx_regs zx_reg
extern ZX_REGS_t *zx_regs;
static sem_t zx_idma_restart_sem;
static uint32_t next_cpu_desc_index = 0;
static uint32_t next_block_desc_index = 0;
static uint32_t free_cpu_desc_index = 0;
static zx_desc_t* zx_idma_desc_cmds[ZX_IDMA_NUM_DESCS];
static uio_dev_t*   uio_dev;

static void zx_idma_isr(int32_t interrupt_count, void *context);
static bool no_isr = true; //< either poll on add or isr driven works.
static bool started = false; //< delays polling till after zx_idma_start  

void zx_dump_descriptor(zx_desc_t *desc )
{
    DBG_PRINTF_DEBUG(" ctrl %x\n", desc->ctrl);
    DBG_PRINTF_DEBUG("  own %x\n", desc->own);
    DBG_PRINTF_DEBUG("  src %x\n", desc->src);
    DBG_PRINTF_DEBUG("  len %x\n", desc->length);
    DBG_PRINTF_DEBUG("  hwa %x\n", desc->hwaddr);
    DBG_PRINTF_DEBUG("  bbp %x\n", desc->bb_ptr);
    DBG_PRINTF_DEBUG("  index %x\n", desc->index);
    DBG_PRINTF_DEBUG("  lines %x\n", desc->lines);
    DBG_PRINTF_DEBUG("  cmd %x\n", desc->zx_creek_cmd.cmd);
    DBG_PRINTF_DEBUG("  add %x\n", desc->zx_creek_cmd.data_address);
    DBG_PRINTF_DEBUG("  sca %x\n", desc->zx_creek_cmd.scale);
    DBG_PRINTF_DEBUG("  err %x\n", desc->zx_creek_cmd.error);
}

void zx_dump_descriptor_list( char *label )
{
    int i;
    DBG_PRINTF_DEBUG("%s %s\n", label, __FUNCTION__);
    for (i = 0; i < ZX_IDMA_NUM_DESCS; i++)  {
	if ( zx_idma_desc_cmds[i]->bb_ptr  
	     || zx_idma_desc_cmds[i]->own
	    ) {
	    zx_dump_descriptor(zx_idma_desc_cmds[i]);
	}
    }
}

void zx_idma_init(void)
{
    int ret;
    int i;
    void* hwaddr;

    ret = sem_init(&zx_idma_restart_sem, 0, 0);
    XASSERT(OK == ret, ret);
    uio_dev = uio_open_dev("zx_idma");
    XASSERT(uio_dev != NULL, errno);

    zx_regs->ZCR = 0;     // Clear block
    for (i = 0; i < ZX_IDMA_NUM_DESCS; i++) 
    {
        zx_idma_desc_cmds[i] = MEM_MALLOC_UNCACHED(&hwaddr, sizeof(zx_desc_t), e_32_byte);
        ASSERT(zx_idma_desc_cmds[i] != NULL);
        memset(zx_idma_desc_cmds[i], 0, sizeof(zx_desc_t));
        zx_idma_desc_cmds[i]->hwaddr = hwaddr;
	zx_idma_desc_cmds[i]->index = i;
	zx_idma_desc_cmds[i]->lines = 0;
	zx_idma_desc_cmds[i]->bb_ptr = 0;
	
    }
    for (i = 0; i < ZX_IDMA_NUM_DESCS; i++) {
	zx_idma_desc_cmds[i]->next = zx_idma_desc_cmds[((i+1) % ZX_IDMA_NUM_DESCS)]->hwaddr;
	DBG_PRINTF_DEBUG("zx init[%d] hwaddr %x %d \n", 
			 i, 
			 zx_idma_desc_cmds[i]->hwaddr,
			 zx_idma_desc_cmds[i]->index
	    );
    }
}

error_type_t zx_idma_open(void)
{
    uint32_t i;
    int s_ret = OK;

    for (i = 0; i < ZX_IDMA_NUM_DESCS; i++) 
    {
	zx_idma_desc_cmds[i]->own = 0;
	if ( zx_idma_desc_cmds[i]->bb_ptr ) {
	    DBG_PRINTF_NOTICE("zx open[%d] free BB %x \n", i, zx_idma_desc_cmds[i]->bb_ptr);
	    BigBuffer_Free( zx_idma_desc_cmds[i]->bb_ptr );  // clear from last page. 
	    zx_idma_desc_cmds[i]->bb_ptr = 0;
	}
    }

    next_cpu_desc_index = 0;
    next_block_desc_index = 0;
    free_cpu_desc_index = 0;
    started = false;

    // clear the semaphore
    while (s_ret == OK)
    {
        s_ret = sem_trywait(&zx_idma_restart_sem);
    }

    return OK;
}

error_type_t zx_idma_add_buffer(uint8_t *buffer, uint32_t width, uint32_t height)
{
    ASSERT(0 == 1);  // no implemented lspto fix unit test code.
    return FAIL;
}

#if 0 
///< testing of lots of nop descriptors 
int zx_idma_add_nop_descriptor( int next )
{
    zx_idma_desc_cmds[next]->ctrl = 0x0002UL;  // intOnFinish and stopOnFinish
    zx_idma_desc_cmds[next]->src = zx_idma_desc_cmds[next]->hwaddr + 32;
    zx_idma_desc_cmds[next]->length = 16; // With padding
    // zx_idma_desc_cmds[next_cpu_desc_index]->next = NULL;
    // hwaddr 
    zx_idma_desc_cmds[next]->bb_ptr = 0; // bb_in;
    // Now fill in the zx_creek_cmd
    zx_idma_desc_cmds[next]->lines = 0;
    zx_idma_desc_cmds[next]->zx_creek_cmd.cmd = 0;
    zx_idma_desc_cmds[next]->zx_creek_cmd.data_address = 0;
    zx_idma_desc_cmds[next]->zx_creek_cmd.scale = 0; 
    zx_idma_desc_cmds[next]->zx_creek_cmd.error = 0;
    zx_idma_desc_cmds[next]->own = 0x0001;   // Owned by block
    zx_dump_descriptor(zx_idma_desc_cmds[next]);
    next = (next + 1) % ZX_IDMA_NUM_DESCS;
    return next;
}
#endif

error_type_t zx_idma_add_bigbuffer(BigBuffer_t *bb_in, uint32_t width, uint32_t height, uint8_t zx_blt_cmd )
{
    uint32_t num_pixels = width * height;
    int s_ret;
    error_type_t ret = OK;
    void *hw_addr;

    if ( bb_in )
    {
	ASSERT(num_pixels < (0xFFFFFFUL)); // 24 bits of pixels per desc
	hw_addr = dma_buffer_map_single(bb_in, DMA_TO_DEVICE);
	DBG_PRINTF_NOTICE("zx addcpu[%d]block[%d] bb %p %x zdrr %x\n", 
			  next_cpu_desc_index, next_block_desc_index, bb_in, hw_addr,
			  zx_regs->ZDRR
	    );
	// disable the interrupt channel
	if (!no_isr) 
	    uio_int_disable(uio_dev);

	if (0 == zx_idma_desc_cmds[next_cpu_desc_index]->own)
	{

	    DBG_PRINTF_NOTICE("zx add[%d] i[%d]lines[%d] bb %p hw 0x%08x\n", 
			      next_cpu_desc_index, 
			      zx_idma_desc_cmds[next_cpu_desc_index]->index,
			      zx_idma_desc_cmds[next_cpu_desc_index]->lines,
			      zx_idma_desc_cmds[next_cpu_desc_index]->bb_ptr,
			      zx_idma_desc_cmds[next_cpu_desc_index]->hwaddr);
	    // Fill in the descriptor
	    zx_idma_desc_cmds[next_cpu_desc_index]->ctrl = 0x0002UL;  // intOnFinish and stopOnFinish
	    zx_idma_desc_cmds[next_cpu_desc_index]->src = 
		zx_idma_desc_cmds[next_cpu_desc_index]->hwaddr + 32;
	    zx_idma_desc_cmds[next_cpu_desc_index]->length = 16; // With padding
	    // zx_idma_desc_cmds[next_cpu_desc_index]->next = NULL;
	    // hwaddr 
	    zx_idma_desc_cmds[next_cpu_desc_index]->bb_ptr = bb_in;
	    zx_idma_desc_cmds[next_cpu_desc_index]->lines = height;
	    // Now fill in the zx_creek_cmd
	    zx_idma_desc_cmds[next_cpu_desc_index]->zx_creek_cmd.cmd = ((num_pixels-1) << 8) | zx_blt_cmd;
	    zx_idma_desc_cmds[next_cpu_desc_index]->zx_creek_cmd.data_address = hw_addr;
	    
	    ASSERT(((uint32_t)zx_idma_desc_cmds[next_cpu_desc_index]->zx_creek_cmd.data_address & 0x1F) == 0);
	    zx_idma_desc_cmds[next_cpu_desc_index]->zx_creek_cmd.scale = 1 << 24;
	    zx_idma_desc_cmds[next_cpu_desc_index]->zx_creek_cmd.error = 1 << 24; 
	    zx_idma_desc_cmds[next_cpu_desc_index]->own = 0x0001;   // Owned by block

	    // zx_dump_descriptor(zx_idma_desc_cmds[next_cpu_desc_index]);


	    if ( no_isr && started) {

		while ( !zx_idma_desc_cmds[next_block_desc_index]->own 
			&& zx_regs->ZDRR != (uint32_t)zx_idma_desc_cmds[next_block_desc_index]->hwaddr ) {
		    next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
		    DBG_PRINTF_DEBUG("zx_idma ZIAR %x %d own %d hw %08x lines%d\n\n", 
				     zx_regs->ZIAR,
				     next_block_desc_index, 
				     zx_idma_desc_cmds[next_block_desc_index]->own,
				     (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr),
				     zx_idma_desc_cmds[next_block_desc_index]->lines );
		    
		}
		// set an semph flag indicating that we must restart on the next buffer
		ret = sem_post( &zx_idma_restart_sem);
		XASSERT( OK == ret, ret );     

		if (1 == zx_idma_desc_cmds[next_block_desc_index]->own)
		{
		    DBG_PRINTF_DEBUG("zx_idma_isr kick %d own %d hw %08x lines%d ZSR %08x \n\n",
				     next_block_desc_index, 
				     zx_idma_desc_cmds[next_block_desc_index]->own,
				     (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr),
				     zx_idma_desc_cmds[next_block_desc_index]->lines,
				     zx_regs->ZSR
			);
		    if (0 == (zx_regs->ZSR & 0x00200) ) {
			zx_regs->ZDWR = (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr);
			next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
		    } else {
			DBG_PRINTF_DEBUG("zx dma status add inc cpu only \n");
		    }

		} 
		else {
		    DBG_PRINTF_DEBUG("zx add inc cpu only \n");
		}
	    } else {  // use isr 

		s_ret = sem_trywait(&zx_idma_restart_sem);
		if ( OK == s_ret) {
		    //XASSERT(next_block_desc_index == next_cpu_desc_index, next_block_desc_index);
		    DBG_PRINTF_NOTICE("zx add cpu %d == %d block\n", next_cpu_desc_index, next_block_desc_index);
		    ASSERT(1 == zx_idma_desc_cmds[next_cpu_desc_index]->own);
		    hw_addr = zx_idma_desc_cmds[next_block_desc_index]->hwaddr;
		    //next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
		    zx_regs->ZDWR = (uint32_t)hw_addr;
		}
		else {
		    DBG_PRINTF_DEBUG("zx add inc cpu only \n");
		}
	    }
	    DBG_PRINTF_NOTICE("zx add end cpu[%d] blk[%d] lines %d bb %p  \n", 
			     next_cpu_desc_index, 
			     next_block_desc_index, 
			     zx_idma_desc_cmds[next_block_desc_index]->lines,
			     zx_idma_desc_cmds[next_block_desc_index]->bb_ptr);
	    next_cpu_desc_index = (next_cpu_desc_index+1) % ZX_IDMA_NUM_DESCS;
	}
	else {
	    ret = FAIL;
	}

	// enable the interrupt channel
	if (!no_isr) 
	    uio_int_enable(uio_dev);
    }
    return ret;
}

int zx_idma_free_input( int max_num_lines_to_free, bool start_image, bool end_image )
{
    int lines_freed = 0;
    static int total_freed = 0;
    static int total_max = 0;


    if (start_image) {
	total_freed = 0;
	total_max = 0;
    } 
    total_max += max_num_lines_to_free;

    while ( total_freed + lines_freed + zx_idma_desc_cmds[free_cpu_desc_index]->lines < total_max 
	    && zx_idma_desc_cmds[free_cpu_desc_index]->own == 0 
	) 
    {
	if ( !zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr ) 
	    continue;
	dma_buffer_unmap_single( zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr,
				 DMA_TO_DEVICE);
	DBG_PRINTF_NOTICE("free input bb %d %p bbhw %x lines %d sum %d total %d start %d end %d\n", 
			 free_cpu_desc_index,
			 zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr,
			 zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr->dma_alloc->hw_addr,
			 zx_idma_desc_cmds[free_cpu_desc_index]->lines,
			 lines_freed,
			 total_max,
			 start_image, 
			 end_image
	    );
	BigBuffer_Free( zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr );
	zx_idma_desc_cmds[free_cpu_desc_index]->bb_ptr = 0;

	lines_freed += zx_idma_desc_cmds[free_cpu_desc_index]->lines;
	zx_idma_desc_cmds[free_cpu_desc_index]->lines = 0;

	free_cpu_desc_index = (free_cpu_desc_index+1) % ZX_IDMA_NUM_DESCS;
    }
    total_freed += lines_freed;
    DBG_PRINTF_NOTICE("free input  %d %d %d\n", lines_freed, total_freed, total_max );
    return lines_freed;
}

error_type_t zx_idma_start(uint32_t image_width_in_pixels,
                           uint32_t image_height_in_lines)
{
    error_type_t ret;
    int index;
    ret = OK;

    if (0 == ZX_ZCR_ENABLE_MASK_SHIFT(zx_regs->ZCR) && 
        0 == next_block_desc_index && 
        1 == zx_idma_desc_cmds[next_block_desc_index]->own)
    {
	if (!no_isr) {
	    uio_int_detach(uio_dev);
	    uio_int_attach(uio_dev, 0, zx_idma_isr, (void*)INTNUM_ZX);  // sk1: bad choice of callback data
	}

        zx_regs->ZICR = 0x1F;     // Clear ints
        zx_regs->ZIER = no_isr ? 0 : 0x1F;     // Enable ints
        zx_regs->ZLLR = image_width_in_pixels - 1;
        zx_regs->ZIHR = image_height_in_lines - 1;  // stefan foo try 
        zx_regs->ZCR = 0x00000081;       // enable block, LCM Off, stop on eoi


	// zx_dump_descriptor(zx_idma_desc_cmds[next_block_desc_index]);
	index = next_block_desc_index;
        // next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
	DBG_PRINTF_DEBUG("zx_idma_start block %d nblk %d cpu %d desc_hw 0x%08x h %d w %d\n", 
			 index,
			 next_block_desc_index, 
			 next_cpu_desc_index, 
			 (uint32_t)zx_idma_desc_cmds[index]->hwaddr,
			 image_height_in_lines,
			 image_width_in_pixels
	    );

	if (!no_isr) 
	    uio_int_enable(uio_dev);
        zx_regs->ZDWR = (uint32_t)(zx_idma_desc_cmds[index]->hwaddr);
        ret = OK;
	started = true;
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}

error_type_t zx_idma_close(void)
{
    error_type_t ret;

    ret = OK;

    zx_regs->ZIER = 0;       // Disable ints
    zx_regs->ZICR = 0x1F;    // Clear ints
    zx_regs->ZCR = 0;       // Disable block

    uio_int_disable(uio_dev);
    uio_int_detach(uio_dev);

    return ret;
}

void zx_idma_isr(int32_t interrupt_count, void *context)
{
    int ret;
    uint32_t int_source;

    // get the interrupt source
    int_source = zx_regs->ZIAR;

    // clear the interrupt
    uint32_t zdrr = zx_regs->ZDRR;
    DBG_PRINTF_DEBUG("zx_idma_isr int_source %x p %x a %x zsr %x zdrr %x zbpr %x zcsr %x zpsr %x zcbr %x interrupt_count %d\n", 
		   int_source, zx_regs->ZIPR, zx_regs->ZIAR,
		   zx_regs->ZSR, zx_regs->ZDRR, zx_regs->ZBPR,
		     zx_regs->ZCSR, zx_regs->ZPSR, zx_regs->ZCBR, interrupt_count);



    // identify the interrupt source and handle it
    if (1 == ZX_ZIAR_OWN_MASK_SHIFT(int_source))      // "own" interrupt
    {
        // do nothing
    }
    if (1 == ZX_ZIAR_DESC_MASK_SHIFT(int_source))      // "desc" interrupt
    {
        // check to see if there is an available input buffer
	DBG_PRINTF_DEBUG("zx_idma_isr %x %d own %d hw %08x lines%d\n\n", int_source,
			 next_block_desc_index, 
			 zx_idma_desc_cmds[next_block_desc_index]->own,
			 (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr),
			 zx_idma_desc_cmds[next_block_desc_index]->lines );

	while ( !zx_idma_desc_cmds[next_block_desc_index]->own 
		&& zdrr != (uint32_t)zx_idma_desc_cmds[next_block_desc_index]->hwaddr ) {
	    next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
	    DBG_PRINTF_DEBUG("zx_idma_isr %x %d own %d hw %08x lines%d\n\n", int_source,
			     next_block_desc_index, 
			     zx_idma_desc_cmds[next_block_desc_index]->own,
			     (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr),
			     zx_idma_desc_cmds[next_block_desc_index]->lines );
	    
	}
	// set an semaphore flag indicating that we must restart on the next buffer
	ret = sem_post( &zx_idma_restart_sem);
	XASSERT( OK == ret, ret );     

        if (1 == zx_idma_desc_cmds[next_block_desc_index]->own)
        {
	    DBG_PRINTF_DEBUG("zx_idma_isr kick %x %d own %d hw %08x lines%d\n\n", int_source,
			     next_block_desc_index, 
			     zx_idma_desc_cmds[next_block_desc_index]->own,
			     (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr),
			     zx_idma_desc_cmds[next_block_desc_index]->lines );
            zx_regs->ZDWR = (uint32_t)(zx_idma_desc_cmds[next_block_desc_index]->hwaddr);
            next_block_desc_index = (next_block_desc_index+1) % ZX_IDMA_NUM_DESCS;
        }
    }

    if (1 == ZX_ZIAR_ILLEGALCMD_MASK_SHIFT(int_source))      // "illegal cmd" interrupt
    {
        DBG_PRINTF_CRIT("zx status %x %x %x\n", zx_regs->ZSR, zx_regs->ZDRR, zx_regs->ZBPR);
        DBG_PRINTF_CRIT("*****************************************\n");
        DBG_PRINTF_CRIT("Illegal ZxCreek command received. ***\n");
        DBG_PRINTF_CRIT("*****************************************\n");
        ASSERT(0); 
    }

    if (1 == ZX_ZIAR_CLEARCOMPLETE_MASK_SHIFT(int_source))      // "clear complete" interrupt
    {
        // don't need to take any actions after this interrupt is triggered
    }

    if (1 == ZX_ZIAR_ENDOFIMAGE_MASK_SHIFT(int_source))      // "end of image" interrupt
    {
        // don't need to take any actions after this interrupt is triggered
    }
    zx_regs->ZICR |= int_source;
}
