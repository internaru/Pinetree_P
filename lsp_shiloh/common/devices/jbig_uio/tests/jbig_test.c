/*
 *
 * ============================================================================
 * Copyright (c) 2010,2013   Marvell International, Ltd. All Rights Reserved.
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
#include "posix_ostools.h"
#include "ATypes.h"
#include "memAPI.h"
#include "string.h" // used for memcpy and memset
#include "lassert.h"
#include "logger.h"
#include "jbig_api.h"
#include "jbigUtils.h"
#include "cmd_proc_api.h"
#include "BigBuffer.h"
#include "dma_buffer.h"
#include "debug.h"

#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_JBIG | LOGGER_JBIG_TEST)  // pick a module from logger.h 

#if defined (HAVE_CMD) || defined (HAVE_UNIT_TEST)
// Test size must be a power of 4 or the test will fail due to the way it is written

#define JBIG_TEST_NUM_OUTPUT_STRIPS 267
BigBuffer_t* testJBIGOutput[JBIG_TEST_NUM_OUTPUT_STRIPS];

ALIGN32 UINT8 testBadJBIGBlack[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x80,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xaa, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xee, 0x00, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02
}; // 110 bytes


ALIGN32 UINT8 testJBIGBlack[] = {
0xfd, 0x2f, 0xa4, 0xb8, 0xc7, 0x63, 0xcf, 0x80,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02, 0xff, 0x02,
0xff, 0x02, 0xff, 0x02, 0xff, 0x02
}; // 110 bytes


ALIGN32 UINT8 testJBIGArray127[] = {
         0xff, 0x02, 0xbe, 0x00,
		 0x4b, 0x92, 0xbe, 0xe2,
		 0x00, 0xbe, 0xeb, 0x73,
		 0x53, 0x32, 0xc2, 0x29,
		 0x7c, 0x22, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02 ,0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		 0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xff, 0x02, 0xff, 0x02,
		0xdb, 0xc3, 0x62, 0xaa,
		0xcf, 0x2f, 0x57, 0x19,
		0x80, 0xff, 0x02, 0xff,
		0x02, 0xff, 0x02, 0x00
};

// Test array that will create a blank image or all 0's
UINT8 testJBIGArray[] ALIGN32= {0xff, 0x02, 0x00,  0x00};

int jbigTest(void);
int jbigSimpleTest( void );
int jbigDelayedOutputTest( void );
int jbig_test_blocking_encode_decode( void );



int jbig_full_test(void)
{
    int testFailed = 0;

    testFailed = jbigTest();
    if( testFailed )
    {
        dbg_printf("JBIG: TEST 1 FAILED\n");
        return testFailed;
    }

    testFailed = jbig_test_blocking_encode_decode();
    if( testFailed )
    {
        dbg_printf("JBIG: TEST 2 FAILED\n");
        return testFailed;
    }

/*
    testFailed |= jbigSimpleTest();
    if( testFailed )
    {
        dbg_printf("JBIG: TEST 2 FAILED\n");
    }
*/
    //jbigDelayedOutputTest();
    
    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("JBIG: tests complete\n"));

    return testFailed;
}


static void DumpDataBuffer( uint8_t * buffer, uint32_t size)
{

    dbg_printf("Buffer: %x\n",(uint32_t)buffer);
    uint32_t loopCounter = 0;
    for(loopCounter = 0;loopCounter < size;loopCounter++)
    {
        dbg_printf("%02x ",(uint8_t)*buffer);
        buffer++;
        if( (loopCounter % 50) == 0 )
        {
            dbg_printf("\n");
        }
    }
    dbg_printf("\n");
}


uint8_t jbigComplete = 0;

static bool jbigWaitForJBIGToComplete()
{
    uint8_t loopCounter = 0;
    bool passed = false;

    jbigComplete = 0;

    while( !jbigComplete )
    {
       posix_sleep_ticks(10);
       //dbg_printf("JBIG: Test: Compression timer expired\n");
       if( loopCounter++ > 30 )
       {
           break;
       }
    }

    if( loopCounter >= 30)
    {
        DBG_MEMLOG(LOG_DEBUG_M,"jbig test: timed out without ODMA completing\n" );
        dbg_printf("jbig test: timed out without completing\n");
    }
    else
    {
        dbg_printf("jbig test: JBIG processing complete\n");
        passed = true;
    }

    return passed;
}

void jbigSimpleTestCallback( void *user_data, uint32_t int_status )
{
    if( user_data )
    {

        if( int_status & JBIG_IDMA_COMPLETE_FLAG )
        {
            // This indicates that the JBIG block has finished reading an input buffer
            dbg_printf("JBIG simple test callback: user input beffer %c complete\n",*(char*)user_data);
        }
        else
        {

            dbg_printf("JBIG simple test callback: last output buffer complete = %x\n",(unsigned int)user_data);
            
        }
        
    }

    if( int_status & JBIG_CONTROL_FLAG )
    {
        // This indicates the that the JBIG block has completed processing for the given parameters
        dbg_printf("--- int_status = %x\n", int_status);
        jbigComplete = 1;
    }
}

typedef struct jbig_test_buf_wrapper_s
{
    int buf_sequence_id;
    bool free_big_buf;
    BigBuffer_t* buf;
}jbig_test_buf_wrapper_t;

static void jbig_odma_test_callback(jbig_handle_t* handle, void* context)
{
    jbig_test_buf_wrapper_t* bufwrap = (jbig_test_buf_wrapper_t*) context;
    BigBuffer_t* buf = bufwrap->buf;

    // convert back to user level buffer
    dbg_printf("%s: buffer sequence %d, addr %#x complete\n", __func__, bufwrap->buf_sequence_id, buf);
    if (bufwrap->free_big_buf)
    {
        BigBuffer_Free(bufwrap->buf);
    }
    MEM_FREE_AND_NULL(bufwrap);
}

static void jbig_idma_test_callback(jbig_handle_t* handle, void* context)
{
    jbig_test_buf_wrapper_t* bufwrap = (jbig_test_buf_wrapper_t*) context;
    BigBuffer_t* buf = bufwrap->buf;

    // convert back to user level buffer
    dbg_printf("%s: buffer sequence %d, addr %#x complete\n", __func__, bufwrap->buf_sequence_id, buf);
    if (bufwrap->free_big_buf)
    {
        BigBuffer_Free(bufwrap->buf);
    }
    MEM_FREE_AND_NULL(bufwrap);
}

static void jbig_test_callback(jbig_handle_t* handle, void* context, uint32_t status)
{
    dbg_printf("JBIG Callback: status = %x\n", status );
    if (status != 0)
    {
        dbg_printf("JBIG: ERROR CONDITION IN JBIG COMPLETION STATUS status = %#x\n", status);
    }

    jbigComplete = 1;
}

#if 0
void jbigTestOddEncodeParameters()
{
    uint16_t loopCounter = 0;
    uint32_t Xd = 984;
    uint32_t Yd = 608;
    uint32_t ld = 72;
    uint32_t pageSize = Xd*Yd/8;
    uint32_t bufferSize = pageSize/JBIG_TEST_NUM_OUTPUT_STRIPS;
    jbig_handle_t * jbigEncodeHandle = NULL;
    void * lastBuffer = NULL;


    uint8_t * test_buffer = MEM_MALLOC_ALIGN( pageSize, e_32_byte );

    if( jbig_open( &jbigEncodeHandle, JBIG_CODEC_BLOCK ) != OK )
    {

        dbg_printf("JBIG: COMPRESSION FAILED TO GET HANDLE\n");
        MEM_FREE_AND_NULL( test_buffer );
        return;


    }

    DBG_MEMLOG(LOG_DEBUG_M,"jbig test: set the encode\n");
    jbig_set_encode( jbigEncodeHandle );

    jbig_disable_cbi( jbigEncodeHandle );

    jbig_set_x_dimension( jbigEncodeHandle, Xd );
    jbig_set_y_dimension( jbigEncodeHandle, Yd );
    jbig_set_stripe_length( jbigEncodeHandle, ld );

    odma_func->set_output_dma_line_width_in_bytes((void *)jbigEncodeHandle, Xd/8);
    //jbig_set_write_line_width( jbigEncodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigEncodeHandle);


    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        jbig_add_input_buffer( jbigEncodeHandle, testJBIGOutput[loopCounter], bufferSize, NULL );
    }

    memset( testJBIGEncode, 0xaa, 1024);
//-jrs    cpu_dcache_writeback_region( testJBIGEncode,  1024 );

    lastBuffer = (void *)testJBIGEncode;
    jbig_add_output_buffer( jbigEncodeHandle, testJBIGEncode, 1024, lastBuffer );

    jbig_start_transaction( jbigEncodeHandle, jbigTestCallback );

    loopCounter = 0;

    //jbigWaitForJBIGToComplete();

    dbg_printf("JBIG: Test: stop Transaction\n");
    jbig_stop_transaction( jbigEncodeHandle );

//    cpu_dcache_invalidate_region(testJBIGEncode, 1024 );

    dbg_printf("JBIG: Test: close\n");

    jbig_close( jbigEncodeHandle );

    DumpDataBuffer(testJBIGEncode, 110);
/* 
    uint8_t testPassed = 1;
 
    if( memcmp(testJBIGEncode, testJBIGBlack, 110) == 0)
    {
        dbg_printf("JBIG TEST: compression passed\n");
    }
    else
    {
        dbg_printf("JBIG TEST: compression failed\n");
        testPassed = 0;
     
    }
*/
}
#endif

int jbigTest()
{

	uint16_t loopCounter = 0;
	uint32_t Xd = 9984;
	uint32_t Yd = 6408;
	uint32_t ld = 128;
	uint32_t pageSize = Xd*Yd/8;
	uint32_t bufferSize = pageSize/JBIG_TEST_NUM_OUTPUT_STRIPS;
	uint32_t bufferLength = bufferSize;
	jbig_handle_t * jbigEncodeHandle = NULL;
	uint32_t retryCounter = 0;
    jbig_test_buf_wrapper_t* bufwrap;
    uint32_t out_buf_count = 0;
    error_type_t err;
    BigBuffer_t* jbig_encode_buf = 0;;
    mlimiter_t* limiter = mlimiter_by_name("print");
    mlimiter_start(limiter);

    DBG_MEMLOG(LOG_DEBUG_H,"jbig test\n");
    dbg_printf("JBIG: *** TEST BEGIN ***\n");
    dbg_printf("JBIG: output buffer size = %d\n", bufferLength);

    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
    	testJBIGOutput[loopCounter] = BigBuffer_Malloc(limiter, bufferLength);
    	
    	if ( testJBIGOutput[loopCounter] )
    	{
    		memset( testJBIGOutput[loopCounter]->data, 0x77, bufferLength );

    	}
    	else
    	{
    		dbg_printf("JBIG TEST: not enough memory\n");
    		ASSERT(0);
    	}
    }

	jbig_handle_t * jbigDecodeHandle = NULL;

	jbig_open( &jbigDecodeHandle, JBIG_CODEC_BLOCK );

    jbig_set_event_callback(jbigDecodeHandle, jbig_test_callback, 0);
    jbig_set_idma_callback(jbigDecodeHandle, jbig_idma_test_callback );
    jbig_set_odma_callback(jbigDecodeHandle, jbig_odma_test_callback );

	jbig_set_decode( jbigDecodeHandle );

    jbig_disable_cbi( jbigDecodeHandle );


    jbig_set_x_dimension( jbigDecodeHandle, Xd );
    jbig_set_y_dimension( jbigDecodeHandle, Yd );
    jbig_set_stripe_length( jbigDecodeHandle, ld );
    jbig_set_output_dma_line_width( jbigDecodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigDecodeHandle);

    dbg_printf("JBIG: Test: Start Write DMA\n");

    retryCounter = 0;

    DumpDataBuffer(testJBIGBlack,110);

    bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
    ASSERT(bufwrap);
    bufwrap->buf = BigBuffer_Malloc(limiter, 32);
    bufwrap->buf_sequence_id = 1;
    bufwrap->free_big_buf = true;
    ASSERT(bufwrap->buf);
    memcpy(bufwrap->buf->data, testJBIGBlack, 32);
    err = jbig_add_input_buffer( jbigDecodeHandle, bufwrap->buf, 32, bufwrap );
    ASSERT(err == OK);

    bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
    ASSERT(bufwrap);
    bufwrap->buf = BigBuffer_Malloc(limiter, 32);
    bufwrap->buf_sequence_id = 2;
    bufwrap->free_big_buf = true;
    ASSERT(bufwrap->buf);
    memcpy(bufwrap->buf->data, &testJBIGBlack[32], 32);
    err = jbig_add_input_buffer( jbigDecodeHandle, bufwrap->buf, 32, bufwrap );
    ASSERT(err == OK);

    bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
    ASSERT(bufwrap);
    bufwrap->buf = BigBuffer_Malloc(limiter, 46);
    bufwrap->buf_sequence_id = 3;
    bufwrap->free_big_buf = true;
    ASSERT(bufwrap->buf);
    memcpy(bufwrap->buf->data, &testJBIGBlack[64], 46);
    err = jbig_add_input_buffer( jbigDecodeHandle, bufwrap->buf, 46, bufwrap );
    ASSERT(err == OK);

    jbig_start_transaction( jbigDecodeHandle );

    /*  roll the JBIG pixel DMA */
    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        retryCounter = 0;
        bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
        ASSERT(bufwrap);
        bufwrap->buf = testJBIGOutput[loopCounter];
        bufwrap->buf_sequence_id = ++out_buf_count;
        bufwrap->free_big_buf = false;
        ASSERT(bufwrap->buf);
    	while( jbig_add_output_buffer( jbigDecodeHandle, testJBIGOutput[loopCounter], bufferSize, bufwrap ) == FAIL )
    	{
    	    dbg_printf("JBIG: Test: Too Many Buffers!! \n");
    	    if( retryCounter++ > 10 )
    	    {
    	        break;
    	    }
    	    posix_sleep_ticks(2);    	
    	};
    }
    
    jbigWaitForJBIGToComplete();
    
    jbig_stop_transaction( jbigDecodeHandle );

    jbig_close( jbigDecodeHandle );
    jbigDecodeHandle = NULL;

    uint8_t * testData;
    uint8_t testPassed = 1;

    dbg_printf("JBIG: Test: Decompression verify data\n");

    testData = dma_buffer_mmap_forcpu(testJBIGOutput[0]);
    DumpDataBuffer(testData, 110);
    dbg_printf("JBIG: Test: Verify output buffer 0\n");
    for( loopCounter = 0; loopCounter < 1248; loopCounter++)
    {
    	if( loopCounter < 1248 -16 )
    	{
			if( *testData != 0xFF)
			{
				testPassed = 0;
				break;
			}
    	}
    	else
    	{
			if( *testData != 0x00)
			{
				testPassed = 0;
				break;
			}

    	}
        if (loopCounter > 0 && (loopCounter % bufferLength == 0))
        {
            dma_buffer_unmmap_forcpu(testJBIGOutput[(loopCounter/bufferLength)-1]);
            testData = dma_buffer_mmap_forcpu(testJBIGOutput[loopCounter/bufferLength]);
            dbg_printf("JBIG: Test: Verify output buffer %d\n", loopCounter/bufferLength);
        }
        else
        {
            testData++;
        }
    }
    dma_buffer_unmmap_forcpu(testJBIGOutput[(loopCounter/bufferLength)]);

    if( testPassed )
    {
    	dbg_printf("JBIG: Decompress Passed\n");
    }
    else
    {
    	dbg_printf("JBIG: Decompress FAILED\n");
        goto JBIG_TEST_1_FAIL;    	
    }

#if 0
    //--- Test JBIG Encoding using the same parameters as we just used above

    jbigTestOddEncodeParameters();
#endif

    if( jbig_open( &jbigEncodeHandle, JBIG_CODEC_BLOCK ) != OK )
    {        
        dbg_printf("JBIG: COMPRESSION FAILED TO GET HANDLE\n");
        testPassed = 0;
        goto JBIG_TEST_1_FAIL;    
    }
    
    jbig_set_event_callback(jbigEncodeHandle, jbig_test_callback, 0);
    jbig_set_idma_callback(jbigEncodeHandle, jbig_idma_test_callback );
    jbig_set_odma_callback(jbigEncodeHandle, jbig_odma_test_callback );

    dbg_printf("JBIG_TEST: Test encode\n");
    jbig_set_encode( jbigEncodeHandle );
    
    jbig_disable_cbi( jbigEncodeHandle );

    jbig_set_x_dimension( jbigEncodeHandle, Xd );
    jbig_set_y_dimension( jbigEncodeHandle, Yd );
    jbig_set_stripe_length( jbigEncodeHandle, ld );
    jbig_set_output_dma_line_width( jbigEncodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigEncodeHandle);    

    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
        ASSERT(bufwrap);
        bufwrap->buf = testJBIGOutput[loopCounter];
        bufwrap->buf_sequence_id = ++out_buf_count;
        bufwrap->free_big_buf = false;
        ASSERT(bufwrap->buf);
    	err = jbig_add_input_buffer( jbigEncodeHandle, testJBIGOutput[loopCounter], bufferSize, bufwrap );
        ASSERT(err == OK);
    }

    jbig_encode_buf = BigBuffer_Malloc(limiter, 1024);
    memset( jbig_encode_buf->data, 0xaa, 1024);

    bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
    ASSERT(bufwrap);
    bufwrap->buf = jbig_encode_buf;
    bufwrap->buf_sequence_id = 1;
    bufwrap->free_big_buf = false;

    jbig_add_output_buffer( jbigEncodeHandle, jbig_encode_buf, 1024, bufwrap );

    jbig_start_transaction( jbigEncodeHandle );

    jbigWaitForJBIGToComplete();

    dbg_printf("JBIG: Test: stop Transaction\n");
    jbig_stop_transaction( jbigEncodeHandle );

    dbg_printf("JBIG: Test: close\n");

    jbig_close( jbigEncodeHandle );
    jbigEncodeHandle = NULL;

    testData = dma_buffer_mmap_forcpu(jbig_encode_buf);

    DumpDataBuffer(testData, 110);

    if( memcmp(testData, testJBIGBlack, 110) == 0)
    {
        dma_buffer_unmmap_forcpu(jbig_encode_buf);
    	dbg_printf("JBIG TEST: compression passed\n");
    }
    else
    {
        dma_buffer_unmmap_forcpu(jbig_encode_buf);
        dbg_printf("JBIG TEST: compression failed\n");
        testPassed = 0;
        goto JBIG_TEST_1_FAIL;
    }

//-- now lets decompress what we just compressed and compare

    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        testData = dma_buffer_mmap_forcpu(testJBIGOutput[loopCounter]);
        memset( testData, 0x99, bufferLength );
        dma_buffer_unmmap_forcpu(testJBIGOutput[loopCounter]);
    }
    
	jbig_open( &jbigDecodeHandle, JBIG_CODEC_BLOCK );

    jbig_set_event_callback(jbigDecodeHandle, jbig_test_callback, 0);
    jbig_set_idma_callback(jbigDecodeHandle, jbig_idma_test_callback );
    jbig_set_odma_callback(jbigDecodeHandle, jbig_odma_test_callback );

	jbig_set_decode( jbigDecodeHandle );

    jbig_disable_cbi( jbigDecodeHandle );

    jbig_set_x_dimension( jbigDecodeHandle, Xd );
    jbig_set_y_dimension( jbigDecodeHandle, Yd );
    jbig_set_stripe_length( jbigDecodeHandle, ld );
    jbig_set_output_dma_line_width( jbigDecodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigDecodeHandle);
    //jbigEnableATMOV( jbigEncodeHandle );
    //jbig_set_atmov_value( jbigEncodeHandle, 2 );


    /*  roll the JBIG pixel DMA */
    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        retryCounter = 0;
        bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
        ASSERT(bufwrap);
        bufwrap->buf = testJBIGOutput[loopCounter];
        bufwrap->buf_sequence_id = ++out_buf_count;
        bufwrap->free_big_buf = false;
        ASSERT(bufwrap->buf);
        while( jbig_add_output_buffer( jbigDecodeHandle, testJBIGOutput[loopCounter], bufferSize, bufwrap ) == FAIL )
        {
            dbg_printf("JBIG: Test: Too Many Buffers!! \n");
            if( retryCounter++ > 10 )
            {
                break;
            }
            posix_sleep_ticks(2);    	
        };
    }

    dbg_printf("JBIG: Test: Start Write DMA\n");

    bufwrap = MEM_MALLOC(sizeof(jbig_test_buf_wrapper_t));
    ASSERT(bufwrap);
    bufwrap->buf = jbig_encode_buf;
    bufwrap->buf_sequence_id = 1;
    bufwrap->free_big_buf = false;
    jbig_add_input_buffer( jbigDecodeHandle, jbig_encode_buf, 110, bufwrap );

    jbig_start_transaction( jbigDecodeHandle );

    jbigWaitForJBIGToComplete();

    jbig_stop_transaction( jbigDecodeHandle );

    // read the first 1248 bytes of the first data buffer. They should all be set to 0xFF except for the last 16 bytes which
    // will be set to 0

    testData = dma_buffer_mmap_forcpu(testJBIGOutput[0]);
    testPassed = 1;

    dbg_printf("JBIG: Test: Decompression verify data\n");

    for( loopCounter = 0; loopCounter < 1248; loopCounter++)
    {
    	if( loopCounter < 1248 -16 )
    	{
			if( *testData != 0xFF)
			{
				testPassed = 0;
				break;
			}
    	}
    	else
    	{
			if( *testData != 0x00)
			{
				testPassed = 0;
				break;
			}

    	}
        if (loopCounter > 0 && (loopCounter % bufferLength == 0))
        {
            dma_buffer_unmmap_forcpu(testJBIGOutput[(loopCounter/bufferLength)-1]);
            testData = dma_buffer_mmap_forcpu(testJBIGOutput[loopCounter/bufferLength]);
            dbg_printf("JBIG: Test: Verify output buffer %d\n", loopCounter/bufferLength);
        }
        else
        {
            testData++;
        }
    }
    dma_buffer_unmmap_forcpu(testJBIGOutput[(loopCounter/bufferLength)]);

    if( testPassed )
    {
        dbg_printf("JBIG: Decompress Passed\n");
    }
    else
    {
        dbg_printf("JBIG: Decompress FAILED\n");
        goto JBIG_TEST_1_FAIL;    	
    }

JBIG_TEST_1_FAIL:

    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
    	BigBuffer_Free(testJBIGOutput[loopCounter]);
    }
    if (jbig_encode_buf)
    {
        BigBuffer_Free(jbig_encode_buf);
    }

    mlimiter_stop(limiter);

    if (jbigDecodeHandle != NULL)
    {
        jbig_close( jbigDecodeHandle );
    }
    if (jbigEncodeHandle != NULL)
    {
        jbig_close( jbigEncodeHandle );
    }


    DBG_MEMLOG(LOG_DEBUG_H,"done: jbigTest\n");
    dbg_printf("done: jbigTest\n");

    if( testPassed )
    {
        return AT_SUCCESS;
    }
    else
    {
        return AT_FAILURE;
    }
}


int jbig_test_blocking_encode_decode( void )
{
    #define STRIP_WIDTH_PIXELS 5104  // Letter edge to edge padded to 16pixel boundary
    #define STRIP_HEIGHT 128
    #define BUFFER_SIZE_BYTES ((STRIP_WIDTH_PIXELS/8)*128)
    int test_status = 0;    // success
	uint32_t Xd = STRIP_WIDTH_PIXELS ;
	uint32_t Yd = STRIP_HEIGHT;
	uint32_t ld = STRIP_HEIGHT;
	uint32_t bufferSize = BUFFER_SIZE_BYTES;
    uint32_t bytes_output = 0;
	jbig_handle_t * handle = NULL;
    error_type_t err;
    int i;
    BigBuffer_t* jbig_src_buf = NULL;
    BigBuffer_t* jbig_encode_buf = NULL;
    BigBuffer_t* jbig_decode_buf = NULL;
    uint8_t* src_data;
    uint8_t* test_data;
    mlimiter_t* limiter = mlimiter_by_name("print");
    mlimiter_start(limiter);

    jbig_src_buf = BigBuffer_Malloc(limiter, bufferSize);
    ASSERT(jbig_src_buf);
    jbig_encode_buf = BigBuffer_Malloc(limiter, bufferSize);
    ASSERT(jbig_encode_buf);
    jbig_decode_buf = BigBuffer_Malloc(limiter, bufferSize);
    ASSERT(jbig_encode_buf);

    // fill the source test buffer and destination buffers
    for (i = 0; i < bufferSize; i++)
    {
        jbig_src_buf->data[i] = (uint8_t)i;
    }
    memset(jbig_encode_buf->data, 0, bufferSize);
    memset(jbig_decode_buf->data, 0, bufferSize);
    
    err = jbig_open(&handle, JBIG_CODEC_BLOCK);
    if (err != OK)
    {
        test_status = __LINE__;
        goto EXIT;
    }
    
    // encode from src buf to encode buf
    err = jbig_encode_strip(handle, jbig_src_buf, jbig_encode_buf, Xd, Yd, ld, JBIG_TPBON, &bytes_output);
    if (err != OK || bytes_output == 0)
    {
        test_status = __LINE__;
        goto EXIT;
    }

    dbg_printf("%s: Blocking Encode compressed %d bytes to %d\n", __func__, bufferSize, bytes_output);

    // decode from encode buf to decode buf
    err = jbig_decode_strip(handle, jbig_encode_buf, bytes_output, jbig_decode_buf, Xd, Yd, ld, JBIG_TPBON, &bytes_output);
    if (err != OK || bytes_output == 0)
    {
        test_status = __LINE__;
        goto EXIT;
    }

    // close
    err = jbig_close(handle);
    if (err != OK)
    {
        test_status = __LINE__;
        goto EXIT;
    }
    handle = NULL;

    // decode should have output same number of bytes that encode started with
    if (bytes_output != bufferSize)
    {
        test_status = __LINE__;
        dbg_printf("%s: Blocking Decode produced %d bytes; expeced %d\n", __func__, bytes_output, bufferSize);
        goto EXIT;
    }

    // map buffers back to userspace since the have been converted to dma buffers by the jbig apis
    src_data = dma_buffer_mmap_forcpu(jbig_src_buf);
    test_data = dma_buffer_mmap_forcpu(jbig_decode_buf);

    // verify; source must equal test output and source must be unmodified
    for (i = 0; i < bufferSize; i++)
    {
        if (src_data[i] != test_data[i] || src_data[i] != (uint8_t)i)
        {
            break;
        }
    }

    if (i != bufferSize)
    {
        test_status = __LINE__;
        dbg_printf("%s: Decode verify failed at offset %d. Should be %d, src is %d, decode is %d\n", __func__, i, (uint8_t)i, src_data[i], test_data[i]);
        hex_dump(test_data, bufferSize);
    }

    dma_buffer_unmmap_forcpu(jbig_src_buf);
    dma_buffer_unmmap_forcpu(jbig_decode_buf);

EXIT:
    BigBuffer_Free(jbig_src_buf);
    BigBuffer_Free(jbig_encode_buf);
    BigBuffer_Free(jbig_decode_buf);
    mlimiter_stop(limiter);
    if (handle)
    {
        jbig_close(handle);
    }

    if (test_status == 0)
    {
        dbg_printf("%s: PASSED\n", __func__);
    }
    else
    {
        dbg_printf("%s: FAILED at line %d\n", __func__, test_status);
    }
    return test_status;
}




uint8_t testFeedback[] = {'1','2','3','4','5','6','7','8','9','0'};

int jbigSimpleTest()
{
    uint8_t testPassed = 0;
#if 0
	uint16_t loopCounter = 0;
	uint32_t Xd = 9984;
	uint32_t Yd = 6408;
	uint32_t ld = 128;
	uint32_t pageSize = Xd*Yd/8;
	uint32_t bufferSize = pageSize/JBIG_TEST_NUM_OUTPUT_STRIPS;
	uint32_t bufferLength = bufferSize;


    DBG_MEMLOG(LOG_DEBUG_H,"jbig test\n");
    dbg_printf("jbig simple test\n");

	jbig_handle_t * jbigDecodeHandle;

	if( jbig_open( &jbigDecodeHandle, JBIG_CODEC_BLOCK ) == FAIL )
	{
	    dbg_printf("JBIG TEST: FAILED DECODE block already in use.\n");
	    return 0;
	    
	}

    uint8_t * test_buffer = MEM_MALLOC_ALIGN( pageSize, e_32_byte );

    ASSERT(test_buffer);

    dbg_printf("JBIG TEST: memory successfully allocated.\n");

    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
    	testJBIGOutput[loopCounter] = &test_buffer[loopCounter*bufferLength];
    	if ( testJBIGOutput[loopCounter] )
    	{
    		memset( testJBIGOutput[loopCounter], 0x77, bufferLength );

    	}
    	else
    	{
    		dbg_printf("JBIG TEST: not enough memory\n");
    		ASSERT(0);
    	}
    }

    dbg_printf("JBIG TEST: memory intialized.\n");

    jbigComplete = 0;
//-jrs    cpu_dcache_writeback_region( test_buffer,  pageSize );
	
	jbig_set_decode( jbigDecodeHandle );
	

	jbig_set_decode( jbigDecodeHandle );

    jbig_disable_cbi( jbigDecodeHandle );


    jbig_set_x_dimension( jbigDecodeHandle, Xd );
    jbig_set_y_dimension( jbigDecodeHandle, Yd );
    jbig_set_stripe_length( jbigDecodeHandle, ld );

    odma_func->set_output_dma_line_width_in_bytes((void *)jbigDecodeHandle, Xd/8);
    // REVISIT DAB   jbig_set_write_line_width( jbigDecodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigDecodeHandle);


    dbg_printf("JBIG: Test: Start Write DMA\n");

    DumpDataBuffer(testJBIGBlack,110);

    jbig_add_input_buffer( jbigDecodeHandle, testJBIGBlack, 32, &testFeedback[0] );

    jbig_start_transaction( jbigDecodeHandle, jbigSimpleTestCallback );

    jbig_add_input_buffer( jbigDecodeHandle, &testJBIGBlack[32], 32, &testFeedback[1] );

    posix_sleep_ticks(70);

    jbig_add_input_buffer( jbigDecodeHandle, &testJBIGBlack[64], 46, &testFeedback[2] );

    void * lastBuffer = NULL;

    /*  roll the JBIG pixel DMA */
    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
        //dbg_printf("JBIG: Test: add output buffer %x\n",testJBIGOutput[loopCounter]);
        uint8_t failCounter = 0;
        
        if( loopCounter+1 == JBIG_TEST_NUM_OUTPUT_STRIPS)
        {
            lastBuffer = (void *)testJBIGOutput[loopCounter];
        }
    	while( jbig_add_output_buffer( jbigDecodeHandle, testJBIGOutput[loopCounter], bufferSize, lastBuffer ) == FAIL )
    	{
    	    posix_sleep_ticks(2);
            if( failCounter++ > 50 )
            {

                dbg_printf("JBIG: Decompress failed to add output buffer!!!!\n");
                break;

            }
    	}
    }


    jbigWaitForJBIGToComplete();


    jbig_stop_transaction( jbigDecodeHandle );

//-jrs    cpu_dcache_invalidate_region(test_buffer, pageSize );

    jbig_close( jbigDecodeHandle );
    uint8_t * testData = (uint8_t *)testJBIGOutput[0];
    uint8_t testPassed = 1;

    dbg_printf("JBIG: Decompress complete verify data\n");
    for( loopCounter = 0; loopCounter < 1248; loopCounter++)
    {
    	if( loopCounter < 1248 -16 )
    	{
			if( *testData != 0xFF)
			{
				testPassed = 0;
				break;
			}
    	}
    	else
    	{
			if( *testData != 0x00)
			{
				testPassed = 0;
				break;
			}

    	}
    	testData++;
    }

    MEM_FREE_AND_NULL( test_buffer );
#endif
    if( testPassed )
    {
        dbg_printf("JBIG: simple test passed\n");
        return AT_SUCCESS;
    }
    else
    {
        dbg_printf("JBIG: simple test failed\n");
        return AT_FAILURE;
    }
}

#if 0
static uint32_t delayedOutputTestCounter = 0;
static 	jbig_handle_t * jbigDelayTestDecodeHandle;
static uint8_t delayTestComplete = 0;
static uint32_t delayTestBufferCount = 0;


static void jbigTestDelayedOutputCallback( void *user_data, uint32_t int_status )
{
    uint32_t Xd = 9984;
	uint32_t Yd = 6408;
	uint32_t pageSize = Xd*Yd/8;
	uint32_t bufferSize = pageSize/JBIG_TEST_NUM_OUTPUT_STRIPS;
    error_type_t error = OK;
	
	uint32_t status = int_status & JBIG_EVENT_STATUS_MASK;

	
   if( int_status & JBIG_ODMA_COMPLETE_FLAG )
   {
       

	  if( (delayedOutputTestCounter >  100) &&( delayTestBufferCount < JBIG_TEST_NUM_OUTPUT_STRIPS) && (error == OK ))
	  {
	      DBG_MEMLOG(LOG_DEBUG_H,"jbig delay test: ODMA_COMPLETE callback status = %x %d %d\n",status, delayedOutputTestCounter,delayTestBufferCount);
	      if( jbigDelayTestDecodeHandle && jbigDelayTestDecodeHandle->inUse )
	      {
	         error = jbig_add_output_buffer( jbigDelayTestDecodeHandle, testJBIGOutput[delayTestBufferCount++], bufferSize, NULL );
	      }

	  }
	  
	  delayedOutputTestCounter++;

	  dbg_printf("jbig delay test: ODMA_COMPLETE callback status = %x %d\n",status, delayedOutputTestCounter);
	  posix_sleep_ticks(10);
	  
	   
   }

   if( int_status & JBIG_IDMA_COMPLETE_FLAG )
   {
	   dbg_printf("jbig delay test: IDMA_COMPLETE callback status = %x\n", status);
   }

   if( int_status & JBIG_CONTROL_FLAG )
   {
       delayTestComplete = 1;
       dbg_printf("jbig delay test: Control callback status = %x\n", status);
       DBG_MEMLOG(LOG_DEBUG_H,"jbig delay test: Control callback status = %x\n", status);
   }
    
}


int jbigDelayedOutputTest()
{

	uint16_t loopCounter = 0;
	uint32_t Xd = 9984;
	uint32_t Yd = 6408;
	uint32_t ld = 128;
	uint32_t pageSize = Xd*Yd/8;
	uint32_t bufferSize = pageSize/JBIG_TEST_NUM_OUTPUT_STRIPS;
	uint32_t bufferLength = bufferSize;


    DBG_MEMLOG(LOG_DEBUG_H,"jbig test\n");
    dbg_printf("jbig simple test\n");


    uint8_t * test_buffer = MEM_MALLOC_ALIGN( pageSize, e_32_byte );

    ASSERT(test_buffer);

    delayTestComplete = 0;
    for( loopCounter = 0; loopCounter < JBIG_TEST_NUM_OUTPUT_STRIPS; loopCounter++)
    {
    	testJBIGOutput[loopCounter] = &test_buffer[loopCounter*bufferLength];
    	if ( testJBIGOutput[loopCounter] )
    	{
    		memset( testJBIGOutput[loopCounter], 0x77, bufferLength );

    	}
    	else
    	{
    		dbg_printf("JBIG TEST: not enough memory\n");
    		ASSERT(0);
    	}
    }

//-jrs    cpu_dcache_writeback_region( test_buffer,  pageSize );

	jbig_open( &jbigDelayTestDecodeHandle, JBIG_CODEC_BLOCK );
	
	jbig_set_decode( jbigDelayTestDecodeHandle );


    jbig_disable_cbi( jbigDelayTestDecodeHandle );


    jbig_set_x_dimension( jbigDelayTestDecodeHandle, Xd );
    jbig_set_y_dimension( jbigDelayTestDecodeHandle, Yd );
    jbig_set_stripe_length( jbigDelayTestDecodeHandle, ld );

    odma_func->set_output_dma_line_width_in_bytes((void *)jbigDelayTestDecodeHandle, Xd/8);
    // REVISIT DAB   jbig_set_write_line_width( jbigDelayTestDecodeHandle, Xd/8 );

    jbig_enable_tpbon(jbigDelayTestDecodeHandle);

    /*  roll the JBIG pixel DMA */
    delayTestBufferCount = JBIG_TEST_NUM_OUTPUT_STRIPS - 100;
    delayedOutputTestCounter = 0;
    for( loopCounter = 0; loopCounter < delayTestBufferCount; loopCounter++)
    {
        DBG_MEMLOG(LOG_DEBUG_H,"jbig delay test: add Buffer = %d\n",loopCounter);
    	jbig_add_output_buffer( jbigDelayTestDecodeHandle, testJBIGOutput[loopCounter], bufferSize, NULL );
    }


    dbg_printf("JBIG: Test: Start Write DMA\n");

    //jbig_add_input_buffer( jbigDelayTestDecodeHandle, (uint32_t)testJBIGBlack, 110, NULL );
       
    jbig_add_input_buffer( jbigDelayTestDecodeHandle, testJBIGBlack, 32, NULL );
    
    
    jbig_start_transaction( jbigDelayTestDecodeHandle, jbigTestDelayedOutputCallback );

    //posix_sleep_ticks(10);
        
    loopCounter = 0;
    while ( jbig_add_input_buffer( jbigDelayTestDecodeHandle, &testJBIGBlack[32], 32, NULL ) == FAIL && loopCounter++ < 100)
    {
        dbg_printf("JBIG: Test: Input buffer 2 failed\n");
        posix_sleep_ticks( 5 );
    }

    if( loopCounter < 100 )
    {
        loopCounter = 0;
        while( jbig_add_input_buffer( jbigDelayTestDecodeHandle, &testJBIGBlack[64], 46, NULL ) == FAIL && loopCounter++ < 100)
        {
            dbg_printf("JBIG: Test: Input buffer 3 failed\n");
            posix_sleep_ticks( 5 );
        }
    }

    if( loopCounter >= 100 )
    {
        
        dbg_printf("JBIG: Test: failed to enter input buffer\n");
    }


    //posix_sleep_ticks(70);

     
    loopCounter = 0;

    //while(delayedOutputTestCounter < 801 &&(loopCounter++ < 65) )
    while( delayTestComplete == 0 &&(loopCounter++ < 150) )
    {
        
        posix_sleep_ticks(20);
    }
    
    dbg_printf("JBIG: Test: Timer Loop Count = %d\n", loopCounter);
    if(loopCounter >= 150 )
    {
        DBG_MEMLOG(LOG_DEBUG_H,"JBIG TEST: waiting for data TIMED OUT!!!!!!\n");
    }
        
    
    dbg_printf("JBIG: Test: stop transaction now\n");
    jbig_stop_transaction( jbigDelayTestDecodeHandle );
    loopCounter = 0;
    while( delayTestComplete == 0 &&(loopCounter++ < 150) )
    {
        
        posix_sleep_ticks(5);
    }

    delayedOutputTestCounter = 0;
    
//-jrs    cpu_dcache_invalidate_region(test_buffer, pageSize );

    jbig_close( jbigDelayTestDecodeHandle );
    uint8_t * testData = (uint8_t *)testJBIGOutput[JBIG_TEST_NUM_OUTPUT_STRIPS - 3];
    uint8_t testPassed = 1;

    for( loopCounter = 0; loopCounter < 1248; loopCounter++)
    {
    	if( loopCounter < 1248 -16 )
    	{
			if( *testData != 0xFF)
			{
			    
				testPassed = 0;
				break;
			}
    	}
    	else
    	{
			if( *testData != 0x00)
			{
				testPassed = 0;
				break;
			}

    	}
    	testData++;
    }

    if( testPassed )
    {
    	dbg_printf("JBIG: Decompress Passed\n");
    }
    else        
    {
        
    	dbg_printf("JBIG: Decompress FAILED!!\n");
    	
    	dbg_printf("%x %x\n",(unsigned int)testData,(unsigned int)*testData);
    }

    
    memset(test_buffer, 0xCC,pageSize);
    
    MEM_FREE_AND_NULL( test_buffer );

    return AT_SUCCESS;
}
#endif
#endif
