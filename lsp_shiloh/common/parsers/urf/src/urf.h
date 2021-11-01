/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef URF_H
#define URF_H

#include "bstream.h"
/// use bstream interface
#define urf_bstream_t bstream_t
#define urf_bstream_construct bstream_construct
#define urf_bstream_close bstream_close
#define urf_bstream_rewind bstream_rewind
#define urf_bstream_read bstream_read

#define URF_ABORT_PARSE     -2

typedef enum {e_auto=0, 
	      e_portrait, 
	      e_landscape, 
	      end_of_orientation_list
} orientation_t;

typedef struct urf_page_s
{   
    /**
     * set by job header
     */
    uint32_t job_pages;//< pages in job
    uint32_t job_pages_done;//< pages in job printed
    /**
     * Set by page header
     */
    struct urf_parser_s * curr_parser;
    uint8_t bitsPerPixel; ///< 8, 24, 32, 48, 64 
    uint8_t colorSpace;   ///< 0 == luminace gamma=2.2, 1 == sRGB 
    uint32_t width;       ///< dots full bleed
    uint32_t height;      ///< dots full bleed
    uint32_t resolution;  ///< pixels per inch X and Y
    bool UnderColorRemoval; ///< Which color table to choose 
    uint8_t copies;
    bool duplex_specified;      ///< Duplex setting specified; apply if true else ignore and let system defaults kick in.
    DUPLEX_MODE duplex_mode;    ///< simplex, vertical binding, horizontal binding
    bool rotate_backside;       ///< if true requests print pipe to rotate backside image

    /**
     * Set urf parser
     */
    uint32_t urf_parse_y;
    uint32_t urf_parse_x;
    uint32_t x_offset;
    uint32_t curr_y_offset;
    cp_strip_t *strip_1;
    cp_strip_t *strip_2;
    cp_strip_t *strip;
    bool spew;
    DOCTYPE doc_type;
    bool this_is_jpeg_parser;
    bool rotate_image;
    orientation_t orientation;
    bool scale_image;
    PRINT_QUALITY print_quality;
    uint32_t stripHeight;
    int32_t vertical_padding;
}urf_page_t;

//typedef int (*intfunc_t)(color_pipe_t *cp, object_tag_t * object_tag);

typedef struct urf_parser_s
{
    void (*parser_register)( void );
    int (*parser_detect)(IOReg * pipe, urf_page_t * urf_page);
    int (*parser_header_read)(urf_bstream_t * bstream, urf_page_t * urf_page);
    int (*parser_decompress)(urf_bstream_t *bstream, urf_page_t *urf_page, print_page_t * p_page, bool (*parser_128_method) (color_pipe_t *cp, object_tag_t * object_tag, bool printable));
    bool (*parser_128_method)(color_pipe_t *cp, object_tag_t * object_tag, bool printable);
}urf_parser_t;

int urf_parser_decompress(urf_bstream_t *bstream, urf_page_t *urf_page, print_page_t* p_page, bool (*parser_128_method) (color_pipe_t *cp, object_tag_t * object_tag, bool printable));
int urf_parser_uncompressed_data(urf_bstream_t *bstream, 
                                 urf_page_t *urf_page, 
                                 print_page_t* p_page, 
                                 bool (*parser_128_method) (color_pipe_t *cp, object_tag_t * object_tag, bool printable));
int32_t urf_bstream_read( urf_bstream_t *bstr, uint8_t **ptr, uint32_t inlen );
void urf_bstream_rewind( urf_bstream_t *bstr );

#endif // URF_H
