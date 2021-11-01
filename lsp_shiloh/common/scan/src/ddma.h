/**
 * \file ddma.h
 *
 * \brief DMA driver/data structrues for chained descriptor based DMA hardware.
 *
 * davep 28-Mar-2011
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef DDMA_H
#define DDMA_H

/* 
 * BIG FAT NOTE!!!!
 *
 * Do NOT modify this structure unless you know what you're doing!
 *
 * This struct is fed to the DMA hardware and has to carefully match what the
 * hardware expects.
 *
 * This structure must be exactly 32 bytes to match the  
 * cacheline width as of this writing (15jun2012).
 *
 */
struct ddma_descriptor {
    /* Configuration Information of the DMA Descriptor, including the Own bit */
    volatile uint32_t config_flags; 

    /* Source Buffer Location in Memory (word address) 
     *
     * MUST BE A DMA POINTER! 
     */
    volatile uint32_t src_addr;    

    /* Length of Transfer (in bytes) (Bits[23:0] valid) */
    volatile uint32_t transfer_len_bytes;

    /* Pointer to Next Descriptor (word address) 
     *
     * MUST BE A DMA POINTER! 
     */
    volatile uint32_t dma_next_descriptor_addr;

    /*
     * Hardware *SHOULD* ignore everything below this line.
     *
     * Have to pad the structure out to cache line size.  As of this writing,
     * cache line size is 32.
     */
    
    /* davep 11-Oct-2010 ; adding a reverse pointer; this is NOT NOT NOT used
     * by the hardware!  I'm taking advantage of the structure's padding to add
     * another field for the firmware.
     */
    struct ddma_descriptor *fw_prev_descriptor_addr;
    struct ddma_descriptor *fw_next_descriptor_addr;

    /* davep 25-Apr-2012 ; store this structure's virtual memory address 
     * (for example, linux kernel's dma_addr_t). Points to this structure in
     * DMA-able memory. 
     */
    dma_addr_t dma_ptr_self;

    /* davep 08-Jun-2012 ; add a CPU accessible pointer to the data buffer */
    void *fw_src_addr;

} __attribute__((packed));

#define DDMA_NAME_LEN 31

struct ddma_descriptor_list {
    char name[DDMA_NAME_LEN+1];

    int num_descriptors;

    /* cpu pointer to memory block containing the descriptors */
    struct ddma_descriptor *list;

    /* dma pointer to the memory block containing the descriptors */
    dma_addr_t dma_handle;
};

/* values for icetest_idma_descriptor.config_flags */
#define DDMA_DESCRIPTOR_CONFIG_OWNER_CPU    (0<<16) 
#define DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK  (1<<16) 
#define DDMA_DESCRIPTOR_CONFIG_EOI  (1<<7)  /* end of transfer (image) */
#define DDMA_DESCRIPTOR_CONFIG_SOI  (1<<6)  /* start of transfer (image)*/
#define DDMA_DESCRIPTOR_CONFIG_INT  (1<<1)  /* interrupt on descriptor transfer finish */
#define DDMA_DESCRIPTOR_CONFIG_STOP (1<<0)  /* stop on descriptor transfer finish */

struct ddma_channel {
    uint8_t channel;

    bool is_open;

    char name[DDMA_NAME_LEN+1];

    /* head is the first running descriptor.
     * tail is the last running descriptor.
     *
     * On descriptor interrupt, head->data is popped, head=head->next
     * On channel_load(), tail=tail->next then tail->data set to new data
     *
     * empty list has tail==head and tail->src_addr==0
     * a list with one element has tail==head and tail->src_addr !=0
     *
     * Note: tail is the last *used* slot, not the next empty slot. tail->next
     * is the next empty slot.
     *
     * tail->next->data should always be NULL unless the list is full. The list
     * should *never* be full. If full, malloc more descriptors.
     *
     * The tail descriptor always has CONFIG_STOP set so DMA will stop if it
     * hits that descriptor. 
     */
    struct ddma_descriptor *head_desc, *tail_desc;
    int num_running;

    /* davep 11-May-2012 ; converting to smart DMA pointer container */
    struct ddma_descriptor_list desc_list;

    struct safety_lock lock;
};

#define DESCRIPTOR_OWNED_BY_BLOCK(desc) ((desc)->config_flags & DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK)

void ddma_desc_dump( struct ddma_descriptor *desc );
void ddma_desc_chain_dump( struct ddma_descriptor *head_desc, int num_descriptors );
void ddma_desc_flush( struct ddma_descriptor *desc );

scan_err_t ddma_channel_open( struct ddma_channel *dch, const char *name, uint8_t channel );
void ddma_channel_close( struct ddma_channel *dch );
scan_err_t ddma_channel_alloc_descriptors( struct ddma_channel *dch, int num_descriptors );
void ddma_channel_free_descriptors( struct ddma_channel *dch );

#define ddma_channel_sanity(x) real_ddma_channel_sanity(__FUNCTION__,__LINE__,(x) )
void real_ddma_channel_sanity( const char *function, int line, struct ddma_channel *dch );

#define ddma_data_peek( desc_list, bytes_per_row)\
            real_ddma_data_peek(__FUNCTION__,__LINE__,(desc_list),(bytes_per_row))
void real_ddma_data_peek( const char *function, int line_number, 
                            struct ddma_descriptor_list *desc_list,
                            int bytes_per_row );

void ddma_channel_load( struct ddma_channel *dch, uint8_t *dma_dest, uint32_t num_rows, uint32_t bytes_per_row );

scan_err_t ddma_descriptor_list_alloc( struct ddma_descriptor_list *desc_list, const char *name, int num_descriptors );
void ddma_descriptor_list_free( struct ddma_descriptor_list *desc_list );
void ddma_descriptor_list_dump( struct ddma_descriptor_list *desc_list );

scan_err_t ddma_descriptor_alloc( struct ddma_descriptor **pdesc );
void ddma_descriptor_free( struct ddma_descriptor **pdesc );

void ddma_channel_lock_init( struct ddma_channel *dch );
void ddma_channel_lock_free( struct ddma_channel *dch );
void ddma_channel_lock( struct ddma_channel *dch );
scan_err_t ddma_channel_try_lock( struct ddma_channel *dch );
void ddma_channel_unlock( struct ddma_channel *dch );

void ddma_channel_dump( const struct ddma_channel *dch );

#endif

