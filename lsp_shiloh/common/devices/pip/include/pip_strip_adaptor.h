/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_strip_adaptor.h
 *
 * \brief base class adaptor to a generic strip.
 *  
 */

#ifndef PIP_strip_adaptor_H_INCLUDED
#define PIP_strip_adaptor_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include "dma_buffer.h"       //< interface to allow dma in user space 
#include "pip_page_adaptor.h" 

struct pip_strip_adaptor 
{
#define pip_strip_adaptor_BaseClassMembers				\
    /** pip recieves strip, the input image */				\
    void (*recv_strip)( struct pip_page_adaptor *from_page,		\
			struct pip_strip_adaptor *from_strip,		\
			void *from_strip_blob );			\
    /** pip send strip, the output image	*/			\
    void (*pip_send_strip)( struct pip_strip_adaptor *to_strip,		\
			    BigBuffer_t *output_buffer[6] );		\
    /** attach external strip data must be subclassed */		\
    void (*attach_from_strip)( struct pip_strip_adaptor *strip,		\
			       void *from_stripblob /* user defined */	\
	);								\
    /** dma'able pointer to image data */				\
    BigBuffer_t *(*get_bb_in)( struct pip_strip_adaptor *strip );	\
    /** todo: cleaner pip_cfg interface */				\
    struct pip_cfg * (*get_pip_cfg)( struct pip_strip_adaptor *strip );	\
    /** free/or destroy reference to strip_blob that attached */        \
    void (*free_in_strip)( struct pip_strip_adaptor *strip );		\
    /** begin new page with next strip */				\
    void (*new_page)( struct pip_strip_adaptor *strip );		\
    /** todo: debug dump isn't active */				\
    void (*dbg_dump_from_strip_data)( struct pip_strip_adaptor *strip );\
    void (*dbg_dump_to_strip_data)( struct pip_strip_adaptor *strip );	\
    /** variables are set/computed in attach_from_strip() */		\
    uint32_t width_pix_in;  /**< input unpadded pixel width of image */	\
    uint32_t lines_in; /**< input strip height */			\
    uint32_t scale_x; /**< output image is 2 X scaled */		\
    uint32_t scale_y; /**< output image is 2 X scaled */		\
    uint32_t width_pix_out; /**< padded width in pixels out*/		\
    uint32_t output_bpp; /**< 1 or 2 bpp  4 is possible but why...*/	\
    uint32_t out_strip_height; /**< output strip height >= input */	\
    bool is_start_of_page; 						\
    bool is_end_of_page /**< maybe short, scan adf paper size is unknown */
    /// end BaseClassMembers
    

    pip_strip_adaptor_BaseClassMembers;  // put this macro in your subclass 
};

/* derived strip class calls: the base class constructor */
struct pip_strip_adaptor *construct_pip_strip_adaptor( 
    struct pip_strip_adaptor *reuse_if_not_null );

/* user calls: once per strip prior to the first strip */
void pip_new_page( struct pip_strip_adaptor *strip );

/* user calls: once per input image/strip */
void pip_recv_strip( 
    struct pip_page_adaptor *from_page, 
    struct pip_strip_adaptor *to_strip, 
    void *from_strip_blob );

#endif // PIP_strip_adaptor_H_INCLUDED
