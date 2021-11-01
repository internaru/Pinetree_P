/**
 * \file pictest.h
 *
 * \brief header file for pictest.c
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PICTEST_H
#define PICTEST_H

/* 28-Feb-2008; struct pic_dma_test is passed around between threads via a "void
 * *". The cookie gives ourselves a way to verify we have a valid structure when
 * we cast out of the "void *".
 */
#define ICETEST_COOKIE  0x49545354  /* 'ITST' */

#define ICE_DMA_TEST_MAX_CHANNELS 32

/* 18-Apr-05 ; pic_dma_test is a "wrapper" around all the information needed
 * to set up for a simple DMA transaction. I've found it simpler wrap these
 * variables up into a structure than to constantly declare each of them on the
 * stack for every test.
 */
struct pic_dma_test {
    uint32_t cookie;

    /* SCAN_TYPE_COLOR or SCAN_TYPE_MONO */
    int cmode;

    uint8_t num_channels;
    uint8_t channels[ICE_DMA_TEST_MAX_CHANNELS];

    /* total number of rows the ICE DMA can expect to see come through; needed
     * so the ICE DMA driver will know when the channel has completed
     */
    int total_rows;

    /* how many pixels are expected in each row */
    int pixels_per_row;

    /* how many bytes are expected in each row */
    int bytes_per_row;

    /* number of ice_dma_buffer structures allocated for the ice_dma_mm
     * structure in ice_dma_open()
     */
    int num_empties;

    /* 
     * The following settings apply to each channel.
     */

    /* number of buffers to malloc() and put onto the readies list */
    int buffers_per_channel;

    /* How many rows we want to put into each buffer; this setting is very
     * important!  PIE is very very very sensitive to the number of rows being
     * fed in via read DMA. The PIE read DMA row count cannot change while a
     * transaction is in process.
     *
     * As of this writing (16-Apr-05) rows_per_buffer % total_rows != 0 
     */
    int rows_per_buffer;

    /* 10-June-05 davep ; need to start differentiating between rows_per_buffer
     * and strip_size. In my original DMA code, the rows_per_buffer was the
     * strip_size. However, as MulitStrip (XYScaler and MultiFilter) read and
     * write DMAs are being turned on, we need to be able to support more than
     * a strip size in a buffer. Eventually need to fix the original DMA code
     * to support a buffer size > strip size and get rid of rows_per_buffer.
     */
    int strip_size;

    /* 
     * The following are useful things to keep around for testing and sanity
     * checks
     */

    /* The original number of buffers allocated for each channel when
     * setup_dma_for_test() was called; compare against actual number
     * (num_readies) after data run is finished to see if we leaked anything
     */
    int num_allocated[ICE_DMA_TEST_MAX_CHANNELS];
};

scan_err_t pic_run_tests( void );

void pic_setup_for_test( void );
void pic_cleanup_after_test( void );

void pictest_setup_dma( struct pic_dma_test *is );

//void pictest_rgb_feeder( ULONG input );

void pictest_choose_channels( scan_cmode_t cmode, struct pic_dma_test *pictest );

void pictest_make_thread( thread_return_t (*entry_function)(void *), void *msg );

void pictest_wait_for_feeder_complete( void );

pthread_t pictest_get_task_id( void );

void verify_pic_buffer_counts( struct pic_dma_test *pictest );

#ifdef LIST_H
void pictest_dma_cleanup( ATLISTENTRY *results_buffers );
/* davep 12-Feb-2013 ; is obsolete and will be removed. pictest_pipe_wait() is
 * the new hawtness
 */
void pictest_run_it( pthread_t thread, ATLISTENTRY *results_buffers, 
                        uint8_t wait_channels[], uint8_t num_channels, int expected_rows );
void pictest_pipe_wait( ATLISTENTRY *results_buffers, int expected_rows );
#endif

void pictest_feeder_wait_for_release( void );
void pictest_feeder_release( void );

#ifdef HAVE_THREE_BDR_LUT 
void bdr_3lut_test( void );
#endif

void pictest_feeder_sem_init( void );
void pictest_feeder_sem_delete( void );
void pictest_feeder_lock( void );
void pictest_feeder_wait_for_release( void );
void pictest_feeder_release( void );

#endif

