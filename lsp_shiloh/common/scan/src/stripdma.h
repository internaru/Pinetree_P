/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef STRIPDMA_H
#define STRIPDMA_H

struct stripdma_strip {
    uint32_t mask;
    struct stripdma_strip *next;
};

#define MAX_STRIPDMA_NAME 12

struct stripdma_list {
    /* for test/debug (+1 for NULL) */
    char name[MAX_STRIPDMA_NAME+1];

    int num_strips;

    /* Memory for strips (caller's responsibility to manage). One slot will be
     * always used for linked list management.
     */
    struct stripdma_strip *list;

    /* track the number of active strips so we know when we're clear to context
     * switch PIE
     */
    int num_pending;

    /* when all these bits are set, the strip is complete */
    uint32_t strip_complete_mask;

    /* stall the pipeline (don't allow new strips but allow pending strips to
     * finish)
     */
    bool stall_state;

    /* test/debug counter; count when we're asked to stall repeatedly */
    int stall_duplicates;

    /* "bottom" points to the bottom of the stack. Think Tetris. "bottom" points
     * to the bottom row of blocks. 
     */
    struct stripdma_strip *bottom;

    /* pointer to the end of the list (will be empty so the incoming list size
     * must be +1 the actual number of nodes in the queue)
     */
    struct stripdma_strip *top;
};


void stripdma_list_init( struct stripdma_list *stripdma, 
                        const char *name,
                        int num_strips, 
                        struct stripdma_strip list[] );

void stripdma_list_dump( struct stripdma_list *stripdma );

#define CHANNEL_TO_MASK(c)  (1<<(c))

void stripdma_set_mask( struct stripdma_list *stripdma, uint32_t read_mask, uint32_t write_mask );

void stripdma_open_read( struct stripdma_list *stripdma, uint8_t channels[], uint8_t num_channels );
void stripdma_open_write( struct stripdma_list *stripdma, uint8_t channels[], uint8_t num_channels );
void stripdma_close( struct stripdma_list *stripdma );

scan_err_t stripdma_push_read( struct stripdma_list *stripdma, uint32_t read_channel );
scan_err_t stripdma_push_write( struct stripdma_list *stripdma, uint32_t write_channel );

scan_err_t stripdma_try_push_read( struct stripdma_list *stripdma, uint32_t read_channel );
scan_err_t stripdma_try_push_write( struct stripdma_list *stripdma, uint32_t write_channel );

void stripdma_stall( struct stripdma_list *stripdma );
void stripdma_reload( struct stripdma_list *stripdma );

#endif

