/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef ICEDMA_H
#define ICEDMA_H

/* The PIC and PIE DMA (both read and write) registers are all actually a
 * 2-deep queue so we can load a next transaction during a current transaction. 
 */
/* 7-Apr-05 ; TODO - there is probably some optimzation I can do to the
 * 'running' list in ice_dma_channel to make it faster based on the assumption
 * everyone is two deep and only two deep.
 */
#define ICE_DMA_REG_DEPTH   2

/* Define ICE_DMA_LOAD_LOG to keep a running ring buffer copy of the last 'N'
 * assignments into the PIC/PIE DMA.  
 *
 * NOT interrupt safe!
 * 
 * Read as a debugging tool but don't write anywhere outside the interrupt
 * unless the PIE interrupt has been disabled.
 */
/* 15-June-05 davep ; the PIE MultiStrip driver runs entirely in thread space
 * instead of ISR. Same restriction applies. There is nothing about this list
 * (or any of the other DMA data structures for that matter) to make it
 * threadsafe.
 */
#ifdef ICE_DMA_LOAD_LOG
#define ICE_DMA_LL_MAX 32
struct ice_dma_loadlog {
    int idx;
    uint32_t data[ICE_DMA_LL_MAX];
    uint32_t rows[ICE_DMA_LL_MAX];

    /* 11-June-05 davep ; added support for DAR1/DAR2 MultiStrip PIE read DMA * */
    uint32_t dar2[ICE_DMA_LL_MAX];
    uint32_t drcr2[ICE_DMA_LL_MAX];
};
#endif

/* convenience structure holding all the parts for DMA-able memory */
struct dma_ptr {
    void *ptr;      /* virtual memory address (CPU accessible) */
    dma_addr_t dma_handle;  /* physical memory address (DMA device accessible) */
    size_t size;
};

/* 24-Mar-05 ; started adding the PIE DMA "buffer dance" data structures and
 * functions. I'm planning on cloning the PIC "buffer dance" as closely as
 * possible for now. I'm not sure where the PIE DMA might go in the future (the
 * read DMA has a lot of complexity I haven't tapped yet). To be on the safe
 * side, I'm keeping PIC and PIE DMA separate for now. If PIE DMA doesn't need
 * extra functionality, can eventually merge the data structures into
 * ice_dma_buffer, ice_dma_channel, etc.
 */
/* 29-Mar-05 ; added rows and bytes_per_row to the buffer since everything
 * moving through PIE must be in terms of rows/bytes_per_row
 */
/* stamped at front of ICE DMA structure for integrity checking */
#define ICE_DMA_COOKIE 0x49434521 /* 'ICE!' */

struct ice_dma_buffer {
    uint32_t cookie;
    ATLISTENTRY listnode;

    /** DMA channel this buffer is assigned to */
    uint8_t channel;

    /** 19-Aug-05 ; for debugging; each module should pass its own tag */ 
    uint8_t tag; 

    /** DMA ready! (cache bypass, correctly aligned, etc) */
    uint8_t *data; 

    /** total byte size of the data buffer */
    uint32_t datalen; 

    /** number of rows this buffer currently holds */   
    uint32_t num_rows; 

    /** maximum number of rows this buffer can hold */
    uint32_t max_rows; 

    /** how many bytes are in each row */
    uint32_t bytes_per_row;  

    /** 9-June-05 ; base_row is required for MultiStrip PIE read DMA (if we were
     * using C++, I'd just descend from ice_dma_buffer but, oh, well).  As of
     * this writing, only the MultiStrip PIE read DMA currently uses it. 
     *
     * base_row is the index in the image of the first row of our data 
     */
    int base_row;

    /* davep 27-Apr-2012 ; adding linux kernel virtual memory handle */
    dma_addr_t dma_handle;

    /* davep 08-Apr-2013 ; at long last, adding data type for self
     * identification 
     */
    scan_data_type dtype;

    /* davep 31-May-2013 ; surrending to the inevitable and adding sensor_num,
     * which sensor this icebuf came from
     */
    uint8_t sensor_num;
};

/* values for the tag field of ice_dma_buffer
 *
 * Everyone who allocates an ice_dma_buffer should tag it (usually passed
 * through ice_dma_add_buffer()) so we can more easily track memory leaks.
 */
#define ICEBUF_TAG_PICWDMA 1
#define ICEBUF_TAG_PIERDMA 2
#define ICEBUF_TAG_PIEWDMA 3
#define ICEBUF_TAG_HPRDMA  4
#define ICEBUF_TAG_HPWDMA  5
#define ICEBUF_TAG_PIEMSRDMA 6
#define ICEBUF_TAG_PIEMSWDMA 7
#define ICEBUF_TAG_PIPENET   8
#define ICEBUF_TAG_PIEMSPAD  9  /* msrdma pad/replication buffer */
#define ICEBUF_TAG_FILTER   10  /* ice data software filter */
#define ICEBUF_TAG_ICEFILE  11  /* ice data from host (usb or tcp or ...) */
#define ICEBUF_TAG_PIPECUT  12  /* pipe cutter */
#define ICEBUF_TAG_CISX     13  /* CISX */
#define ICEBUF_TAG_FW_RMARGIN 14 /* firmware margins, read side */
#define ICEBUF_TAG_FW_WMARGIN 15 /* firmware margins, write side */
#define ICEBUF_TAG_ICETEST_IDMA 16 /* ICETest block (rdma only) */
#define ICEBUF_TAG_PAGER    17  /* pager.c */

/* TODO - add more comments
 *
 * The running field is the "mirror" queue, the buffers currently pushed into
 * the DMA controller. The DMA controller doesn't give us a way to get a "done"
 * buffer so we have to remember it ourselves. When a DMA done interrupt is
 * hit, the top of the queue is popped off since it's assumed to be the just
 * completed buffer. When a buffer is loaded into the controller, it *must*
 * also be put onto this queue.
 *
 * The readies list holds the ice_dma_buffers WITH data available to be pushed
 * into the DMA controller. Readies can be fed into any of the channels.
 *
 */
struct ice_dma_channel {
    /* the channel number this data structure assigned to */
    uint8_t channel;

    /* 27-Apr-05 ; needed a way to unambiguously tell if a channel is in-use */
    bool is_open;

    /* expected = pending + running + completed 
     * 
     * Note these are *signed* numbers so I can detect errors.
     * (They should *never* go negative.)
     */
    int expected_rows;
    int pending_rows;   
    int running_rows;
    int completed_rows;

    /** buffers ready and waiting to be loaded into DMA controller */
    ATLISTENTRY readies;

    /** buffers currently loaded into DMA controller */
    ATLISTENTRY running;

    /** ice_dma_buffer(s) w/o data (blanks waiting to be filled) */
    ATLISTENTRY empties; 

    /* 27-Mar-05 ; orphans is a silly idea. If the PIE ISR can't send a
     * completed buffer to the task (probably because the message queue is
     * full), the buffer will be "dropped" but I don't want to free it from the
     * ISR. Store here until we can properly deal with out-of-memory
     * conditions.
     */
    ATLISTENTRY orphans;
    
    int num_readies;
    int num_running;
    int num_orphans;
    int num_empties;

    /* statistics */
    int no_readies;  /* out of memory, no buffer to feed in ISR */
    int buffer_sends;
    int msg_drops;   /* SYMsgSendNoWait() in ISR failed */
    int buffer_loads;
    int no_empties;

    int bytes_per_row;

#ifdef ICE_DMA_LOAD_LOG
    struct ice_dma_loadlog loadlog;
#endif

    /* davep 17-Sep-2005 ; we're mixing interrupt and threadspace drivers a lot
     * now. The PIE MultiStrip driver is threadspace but the SimpleStrip
     * drivers run in interrupt space (usually).
     *
     * Enable this define to add mutex checks to certain icedma.c level
     * functions. Enable this if things are going wonky and we're not sure why.
     * This is purely a testing/debugging aid. The scan code is designed to run
     * from a single thread; critical data structures are made safe by
     * intDisable().
     */
    struct safety_lock lock;

    /* davep 08-Apr-2013 ; adding scan_data_type so we can personalize the
     * icebufs
     */
    scan_data_type dtype;

    /* davep 23-May-2013 ; adding nsensor support deep into the DMA's in order
     * to multiplex pie
     */
    uint8_t sensor_num;
};

/* davep 13-Apr-2008 ; The recent ASIC added the ability to configure PIC and
 * PIE DMA to either 16 or 32-byte bursts.  The previous ASIC doesn't have this
 * ability. 
 *
 * XXX I don't yet know if HalfPack is selectable or is hardwired to 32-byte.
 */
typedef enum { ICE_DMA_BURST_16, ICE_DMA_BURST_32 } ice_dma_burst_size;

/* 12-June-05 davep ; function pointer type for a function called by
 * pie_dma_interrupt() and pic_interrupt() to handle setting up a new DMA
 * transaction. 
 */
typedef void (*iceisr_f)( uint8_t channel );

struct ice_dma_driver {
    int msg_data; /* message number to send when we have data */

    void (*reset)( uint8_t channel );
    void (*enable)( uint8_t channel );
    void (*disable)( uint8_t channel );

    /* davep 01-Jul-2013 ; adding the uint8_t * pointer and redeclaring the
     * uint32_t dma_dest to dma_addr_t. Need to support both hardware drivers
     * (which require a physical (not virtual) address) and firmware drivers
     * that require a cpu addressible address.
     */
    void (*load)( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, uint32_t rows, uint32_t bytes_per_row );

    void (*start)( uint8_t channel );
    bool (*is_enabled)( uint8_t channel );

    /* 12-June-05 davep ; added an ISR function pointer to handle flipping
     * between the simple strip PIE read/write DMA ISR and the complex
     * multi-strip PIE read/write DMA ISR.
     */
    /* Currently only the pie dma uses it and only to switch back and forth
     * between the piemsdma and piedma drivers. I switch back and forth
     * to run pie tests (pietest.c). With normal scans, the piemsdma driver is
     * every used.
     */
    iceisr_f icebuf_isr;

    /* max_queue_depth : how many commands (pointer, rows, bytes_per_row, etc) we
     *                   can push into the DMA channel controller
     *
     * davep 16-May-2011 ; moved from ice_dma_mm to the driver level so we can
     * support different drivers with different queue depth requirements (e.g.,
     * the PIE descriptor dma has depth==1, old register queue is depth==2)
     */
    int max_queue_depth;
};

/* 24-Mar-05 ; The PIE read/write DMA memory management (mm) structure contains
 * the stuff necessary to manage the DMA "buffer dance", the in/out flow of
 * data through the DMA channels.
 *
 * The read and write DMA are very similar so far but there is a lot of
 * complexity in the read DMA that is untapped as of this writing. I have a lot
 * of duplicate code between PIE read/write DMA and between PIE DMA and PIC DMA
 * because I'm not sure when that complexity will rear its head. If it never
 * does, the PIC/PIE read/write DMAs could be refactored and partially merged.
 */

struct ice_dma_mm {

#define ICE_NAME_MAX 12 
    char name[ICE_NAME_MAX+1]; /* useful for debugging (+1 for NULL) */

    uint8_t num_channels;

    struct ice_dma_channel *channels; /* ice_dma_buffer(s) pushed into DMA controller */

    /* davep 09-Nov-2005 ; maintain last channel we refilled so we can round
     * robin across the channels (this won't be used very often).
     * Note this is NOT an index into channels[] but an index into a list of
     * open channels built in ice_dma_mm_refill().
     */
    int next_refill_channel;

    const struct ice_dma_driver *drv;
};

/* The ISR sends buffers to threads via a message queue (see agMessage.h and
 * smsg.h). When the ISR decides the currently finished buffer is the last
 * buffer in the queue (expected_rows==completed_rows), the msg.param1 is set
 * to this flag telling the thread this paricular channel's data is done.
 */
#define ICE_DMA_LAST_BUFFER  1

/* 27-Apr-05 ; if an ice_dma_buffer is not yet own by a channel, set the
 * channel number to this value because zero is a valid channel number
 */
#define CHANNEL_UNUSED 0xff

/* davep 9-Nov-05; useful shorthand function pointers; I'm finding I'm needing
 * to use polymorphism a bit more throughout the ICEDMA "class hierarchy" so I
 * finally broke down and added these
 */
typedef void (*ice_add_ready_f)( uint8_t channel, struct ice_dma_buffer **addme );
typedef void (*ice_free_empty_f)( uint8_t channel, struct ice_dma_buffer **freeme );
typedef void (*ice_channel_launch_f)( uint8_t channel );
typedef scan_err_t (*ice_add_buffer_f)( uint8_t channel, uint8_t *data, uint32_t datalen,
                                  uint32_t rows, uint32_t bytes_per_row );

/* prototypes */
void ice_dma_init( struct ice_dma_mm *mm, 
                   const char *name, 
                   const struct ice_dma_driver *drv, 
                   struct ice_dma_channel channels[],
                   int max_channels );

int ice_dma_open( struct ice_dma_mm *mm, 
                  uint8_t channels[],
                  uint8_t num_channels,
                  int num_empties,
                  int total_rows,
                  int bytes_per_row,
                  uint8_t icebuf_tag );

scan_err_t ice_dma_add_buffer( struct ice_dma_mm *mm, 
                    uint8_t channel, 
                    uint8_t *data, 
                    int datalen, 
                    int rows, 
                    int bytes_per_row );

void ice_dma_add_ready( struct ice_dma_mm *mm, 
                        uint8_t channel, 
                        struct ice_dma_buffer **addme );

void ice_dma_free_empty( struct ice_dma_mm *mm, 
                         uint8_t channel,
                         struct ice_dma_buffer **freeme );

void ice_dma_channel_launch( struct ice_dma_mm *mm, uint8_t channel );

void ice_dma_sanity( struct ice_dma_mm *mm );

void ice_dma_debug_log( const struct ice_dma_mm *mm );

/* davep 30-Jul-2013 ; dump specific icebuf lists */
#define ICE_DMA_LIST_READIES 1
#define ICE_DMA_LIST_RUNNING 2
#define ICE_DMA_LIST_ORPHANS 3
void ice_dma_dump_list( const struct ice_dma_mm *mm, int which_list );

scan_err_t ice_dma_isr( struct ice_dma_mm *mm, uint8_t channel );

void ice_dma_close( struct ice_dma_mm *mm );


void icebuf_blank( struct ice_dma_buffer *icebuf );
void ice_free_list( ATLISTENTRY *list );
void ice_free_array( struct ice_dma_buffer *icebuf_array[],
                     int num_elements );
uint32_t ice_list_count_rows( ATLISTENTRY *list );
void ice_free_counted_list( ATLISTENTRY *list, int expected_cnt );
void icebuf_sanity( struct ice_dma_buffer *icebuf );
struct ice_dma_buffer *icebuf_pop( ATLISTENTRY *list );
void ice_buffer_callback( struct ice_dma_mm *mm, 
                     struct ice_dma_channel *ch, 
                     struct ice_dma_buffer *icebuf, 
                     bool last_buffer );

/* icebuf without data */
struct ice_dma_buffer * icebuf_new_blank( uint8_t tag );

/* icebuf with data */
struct ice_dma_buffer * icebuf_new( int max_rows, int bytes_per_row, uint8_t tag );

/* copy icebuf src to dst but NOT the data field */
void icebuf_copy_nodata( struct ice_dma_buffer *dst_icebuf, struct ice_dma_buffer *src_icebuf );

void icebuf_free( struct ice_dma_buffer **p_icebuf );

uint8_t *ice_dma_alloc_buffer( int datalen );
uint8_t *ice_dma_alloc_persistent_buffer( int datalen );

void ice_dma_refill( struct ice_dma_mm *mm, 
                     int rows_per_buffer, 
                     ice_add_buffer_f add_buffer_fcn, 
                     ice_channel_launch_f channel_launch_fcn );

void ice_dma_cancel( struct ice_dma_mm *mm );

#ifdef ICE_DMA_LOAD_LOG
void ice_dma_loadlog2( struct ice_dma_channel *ch, 
                       uint32_t data, uint32_t num_rows, 
                       uint32_t dar2, uint32_t drcr2 );

void ice_dma_loadlog( struct ice_dma_channel *ch, 
                        uint32_t data, uint32_t num_rows );

void ice_dma_dump_loadlogs( struct ice_dma_mm *mm );

#endif

struct ice_dma_buffer * ice_dma_get_ready( struct ice_dma_mm *mm, struct ice_dma_channel *ch );

void ice_dma_run_forever( struct ice_dma_mm *mm );

int ice_dma_setup_buffers( struct ice_dma_mm *mm, 
                            uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row, 
                            ice_add_buffer_f add_buffer_fcn );

/* davep 14-Aug-2012 ; dma+dcache functions */
void icebuf_dma_map( struct ice_dma_buffer *icebuf );
void icebuf_dma_unmap( struct ice_dma_buffer *icebuf );

void icebuf_free_data( struct ice_dma_buffer *icebuf );

#endif /* ICEDMA_H */

