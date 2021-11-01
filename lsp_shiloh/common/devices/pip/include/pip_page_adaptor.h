/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_page_adaptor.h
 *
 * \brief base class adaptor to a generic page.
 *  
 */

#ifndef PIP_page_adaptor_H_INCLUDED
#define PIP_page_adaptor_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

/// base class defines interfaces for a page pip page adaptor 
/// a sub class must provide implementation for all the function pointers starting with from_.
struct pip_page_adaptor 
{
#define pip_page_adaptor_BaseClassMembers				\
    void (*attach_from_page)( struct pip_page_adaptor *page, void *from_pageblob ); \
    void (*recv_page)( struct pip_page_adaptor *page, void *from_pageblob ); \
    void (*pip_send_page)( struct pip_page_adaptor *page );		\
    void (*page_close)( struct pip_page_adaptor *page );	/* addor extra close behaviour */ \
    void (*add_strip)( struct pip_page_adaptor *page, uint32_t height, bool end_of_image ); \
    uint32_t (*from_page_pip_mode)( struct pip_page_adaptor *p );	\
    uint32_t scale_x;	/**< 1, or 2 x integer output scaling */	\
    uint32_t scale_y;	/**< 1, or 2 x integer output scaling */	\
    uint32_t pix_width;	 /**< unpadded input width in pixels */		\
    uint32_t page_height; /**< expected output page height */		\
    uint32_t strip_in_cnt;	/**< counter */				\
    uint32_t strip_in_height;	/**< running height sum */		\
    uint32_t strip_out_cnt;	/**< counter */				\
    uint32_t strip_out_height;	/**< running height sum */		\
    uint32_t out_page_height    /**< input desire with early exit reset */ 
    /// end BaseClassMembers
    

    pip_page_adaptor_BaseClassMembers;  ///< base class instantiated with this macro in derived class
};

/// accessors fetch / compute data 
///
uint32_t from_page_pip_mode( struct pip_page_adaptor *psa );

/// scale factor desired in output page:  1 2 
uint32_t from_page_x_scale( struct pip_page_adaptor *psa );
/// scale factor desired in output page:  1 2 
uint32_t from_page_y_scale( struct pip_page_adaptor *psa );

/// input page width in pixels  
uint32_t from_page_pix_width( struct pip_page_adaptor *psa );

/// anticipated input page height in lines, 
uint32_t from_page_page_height( struct pip_page_adaptor *psa );

/// true if we expect the next output strip to be the last output strip for the page.
bool from_page_at_output_eoi( struct pip_page_adaptor *page, uint32_t output_strip_height );

/// close device cleanly on end of page in the middle of work as a cancel/abort 
void from_page_close( struct pip_page_adaptor *page ); 

/// get a foreign page object, attach it to the page_adaptor and start operating on it.
/// calls pip_send_page for you.
/// recv() 
/// { 
///    attach()
///    send()
/// }
void pip_recv_page( 
    struct pip_page_adaptor *page, 
    void *from_page_blob );


/// base class constructor 
/// typically called once 
/// close / free is the current destructor path.
struct pip_page_adaptor *construct_pip_page_adaptor( 
    struct pip_page_adaptor *reuse_if_not_null );

#endif // PIP_strip_adaptor_H_INCLUDED
