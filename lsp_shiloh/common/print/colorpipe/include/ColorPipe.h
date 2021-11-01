#ifndef __COLORPIPE_H__
#define __COLORPIPE_H__
/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipe.h
 *
 *
 *****************************************************************************/
#include <stdint.h>

/**
 * generic pointer for color_pipe structure
 */
typedef struct color_pipe_s color_pipe_t;

typedef enum {
    e_color_space_1_to_1,
    //e_color_space_3_to_1, //not supported
    //e_color_space_3_to_3, //not supported
    e_color_space_3_to_4
} color_space_conv_t;

typedef struct 
{
   uint8_t * pK;
   uint8_t * pC;
   uint8_t * pM;
   uint8_t * pY;
} cmyk_byte_array_t;

typedef struct 
{   
    uint32_t K;
    uint32_t C;
    uint32_t M;
    uint32_t Y;
    
} cmyk_uint32_t;

#define K_ONLY false
#define P_BLACK true
typedef struct 
{
    uint8_t * line;//if NULL use bool
    bool Pblack; //P_BLACK process black
                 //K_ONLY k-only black
}object_tag_t;

/**
 * this will allow us to support multiple pipes or SW & HW at the same time.
 * 
 * unused at the moment.
 * 
 * normal usage is e_color_pipe_default.
 */
typedef enum 
{
    e_color_pipe_default 
}color_pipe_mode_t;


/**
 * \brief this MUST be called at the top of EVERY page in which
 * the color pipe is used.
 * 
 * @param[in] output_bit_per_pixel: halftoned bbp.
 * 
 * @param[in] color_space_conv:
 * 
 * @param[in] pixels_per_line:Max output pixels. if imput pixels
 *       are less the pixels_per_line, block will fill in to end
 *       of line with blank pixels.
 * 
 * @param[in] output_pitch: number of bytes between lines in
 *       strip
 * 
 * @param[in] lines_per_strip:  strip size
 * 
 * @param[in] x_scale: 1, 2, 4, 8.  # of times the pixel will be
 *       replicated. (xscale * count).  y_scale is responsiblity
 *       of caller.
 * 
 * @param mode[in] set to e_color_pipe_default. When HW pipe is
 *            added, we may added testing modes that expand on
 *            this.
 * 
 * @return color_pipe_t* returns ptr to color_pipe and RESETS
 *         THE COLOR PIPE to TOP OF PAGE.
 */

color_pipe_t * color_pipe_page_open( uint32_t output_bits_per_pixel, 
                                     color_space_conv_t color_space_conv,
                                     uint32_t pixels_per_line,
                                     uint32_t output_pitch,
                                     uint32_t lines_per_strip,
                                     uint32_t x_scale, 
                                     color_pipe_mode_t mode);
/**
 * @brief sets up a output HT buffer.  This must be called
 *        before starting to write CSC to pipe and after strip
 *        is full and before writing more CSC data.
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] color_planes_out:1-4 ptrs to output HT buffers
 * @param[in] pitch amount to increment to get from one line to
 *       next.  MUST BE LARGER than pixels_per_line.
 * @param[in] strip_halftoned_callback:  callback that will be
 *       called when buffer is HT'ed.
 * @param[in] callback_private_data: caller's private data to be
 *       returned when buffer is halftoned.
 */
void color_pipe_new_halftone_strip(color_pipe_t * color_pipe, 
                                   cmyk_byte_array_t *color_planes_out,
                                   void(*strip_halftoned_callback)( void *callback_private_data, cmyk_byte_array_t *color_planes_out, uint32_t size ), 
                                   void *callback_private_data  );
/**
 * @brief updates line count & HT position location ONLY
 * 
 * @precondition: previous strip should have been finished with
 * "clear" pixels
 * 
 * @obsolete will delete.
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] num_lines:  number on lines to increment page line
 *       count by.  (used by HT pattern anchor correct)
 */
void color_pipe_blank_strip(color_pipe_t * color_pipe,uint32_t num_lines);

/**
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] line_ptr: ptr to input data. Either 24bit sRGB or
 *       8bit grey.
 * @param[in] ot_ptr ptr to object tag info. ptr to 8bit OT or NULL and use bool (see
 *       definition above).
 * 
 * @return bool false: strip full, starting HT.
 */
bool color_pipe_process_line(color_pipe_t * color_pipe,  
                             uint8_t *line_ptr,                       //inputs 3 byte/pixel, packed RGB or 1 byte/pixel grey
                             object_tag_t *ot_ptr                   //  1byte/pixel, packed OT or boolean flag.
                             );  
/**
 * \brief returns the calculated pixel counts based on formula usually called at end of page.
 *
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[out] weighted_pixels:TBD
 * @param[out] nonwhite_pixels:TBD
 * @param[out] white_pixels:TBD
 * @param[in] reset_counts:Reset counts to zero if true.
 */
void color_pipe_get_pixel_counts(color_pipe_t * color_pipe,
                                 cmyk_uint32_t * weighted_pixels, 
                                 cmyk_uint32_t * nonwhite_pixels, 
                                 cmyk_uint32_t * white_pixels, 
                                 bool reset_counts);

/**
 * @brief
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] count: repeat count of pixel to be added (modified
 *       by x_scale)
 * @param[in] ot_ptr: ptr to 8bit OT or NULL and use bool (see
 *       definition above).
 * @param[in] pixels:
 * 
 * 
 */
void color_pipe_process_repeated_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t * pixel);  //pixel to be added, may either be 24bit sRGB or 8bit Grey
/**
 * @brief Assumes pixels are unique and does not check for repeated pixels.
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] count: number of pixels to be added (modified
 *       by x_scale)
 * @param[in] ot_ptr: ptr to 8bit OT or NULL and use bool (see
 *       definition above).
 * @param[in] pixels:
 * 
 * 
 */
void color_pipe_process_multiple_pixels(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t * pixel);  //pixels to be added, may either be 24bit sRGB or 8bit Grey
/**
 * @brief doesn't not assume pixels do not contain runs of pixels and searches for them.
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] count: number of pixels to be added (modified
 *       by x_scale)
 * @param[in] ot_ptr: ptr to 8bit OT or NULL and use bool (see
 *       definition above).
 * @param[in] pixels:
 * 
 * 
 */
void color_pipe_process_multiple_pixels_unsorted(color_pipe_t * color_pipe,
                                        uint32_t count, 
                                        object_tag_t *ot_ptr,
                                        uint8_t *pixel);
/**
 * \brief handles "ragged right edge", pads to EOL and starts HT
 * of line and may finish HT of line depending on
 * implementation.  
 * 
 * @param[in] color_pipe: ptr to color_pipe struct.
 * @param[in] HT_if_true: HalfTone line if true, otherwise just
 *       return.  Used to munch top margin.
 * 
 * @return bool:COLOR_PIPE_STRIP_NOT_FULL: more room in strip.
 * @return bool:COLOR_PIPE_STRIP_FULL: strip full, starting HT.
 */
#define COLOR_PIPE_STRIP_FULL false
#define COLOR_PIPE_STRIP_NOT_FULL true
bool color_pipe_eol(color_pipe_t * color_pipe, bool HT_if_true);
                                                                                        
/**
 * \brief flushes color pipe to end of page.  Primarily useful for HW color pipes.  
 *        This call may block.
 *
 * @param[in] color_pipe: ptr to color_pipe struct.
 * 
 * @return uint32_t line # completed HT'ed.
 */
void color_pipe_eop(color_pipe_t * color_pipe); //returns when buffered lines are finished and last line finished.  
                                                    //May return immediately
/**
 * \brief cleans up malloc'ed memory.  
 *        Caller MUST do open_color_pipe_page before anything else.
 *
 * @param[in] color_pipe: ptr to color_pipe struct.
 * 
 * @return color_pipe_t* returns NULL.
 */
color_pipe_t * color_pipe_flush(color_pipe_t * color_pipe);   
                                                           


#endif // __COLORPIPE_H__
