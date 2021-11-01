/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/**
 * \file freetype_api.h
 *
 * \brief This is wrapper for the freetype opensource project.
 *
 **/

#ifndef FREETYPE_API_H
#define FREETYPE_API_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize the freetype wrapper
 *
 */
void freetype_init(void);

/**
 * \brief Create a handle for a rendering operation
 *
 * \param[in] fontfile     Which font file to use. This is a UFS path.
 * \param[in] pt_size      The pt_size to which to render (1 pt = 1/72 inch)
 * \param[in] x_resolution The x_resolution of the output (in DPI)
 * \param[in] y_resolution The y_resolution of the output (in DPI)
 * \retval                 The handle to be used with other library operations
 *                         The function returns NULL in the case of an error.
 */
void* freetype_open_font(const char* fontfile, /* UFS path */
                         unsigned int pt_size,
                         unsigned int x_resolution,
                         unsigned int y_resolution);

/**
 * \brief Destroy a previously created handle
 *
 * \param[in] font_handle  The previously created handle to destroy
 */
void  freetype_close_font(void* font_handle);

/**
 * \brief This is the glyph data type (it can be confusing)
 *
 * \param[out] rows      The height of this glyph (image rows)
 * \param[out] width     The width of this glyph (in image columns not bytes)
 * \param[out] pitch     The number of bytes of data for each row
 * \param[out] data      A pointer to the data (may be 1 bit per pixel or 8)
 * \param[out] left      The left bearing of the glyph (see freetype drawings)
 * \param[out] top       The top bearing of the glyph (see freetype drawings)
 *                       This is the distance from the baseline to the top most
 *                       glyph line. "Upwards" Y coordinates are positive.
 * \param[out] advance_x How far should the "pen" advance (in pixels)
 * \param[out] advance_y How far should the "pen" advance (in pixels)
 *
 * This datatype was organized to map most directly to freetype's data
 * structure. As such it may not match what you would expect but it is a tried
 * and true model.
 */
typedef struct
{
  int            rows;
  int            width;
  int            pitch; /* Number of bytes in one row */
  unsigned char* data;
  int            left; /* The left bearing of the glyph (see freetype drawing)*/
  int            top;  /* The top bearing of the glpyh (see freetype drawings)
                          Distance from the baseline to the top most glyph line.
                          Upwards Y coordinates are positve */
  int            advance_x; /* How far should the "pen" advance */
  int            advance_y; /* How far should the "pen" advance */
} freetype_glyph_data_t;

/**
 * \brief Render a single character into a graphic glyph
 *
 * \param[in] font_handle  A previously created handle
 * \param[in] charcode     A unicode value of the character (unicode?)
 * \param[in] mono         Do we want mono or grayscale (8bit) output
 * \param[in] glyph        A pointer to the location where the glyph should be
 *                         stored.
 * \retval                 0 upon success
 *
 * Note that the data written to glyph includes a pointer to internally managed
 * memory. The client should be sure to make a copy of each glyph's data before
 * proceeding to ask this wrapper to render the next glyph or the previous
 * output will be overwritten.
 */
int freetype_render_glyph(void* font_handle,
                          unsigned long charcode, int mono,
                          freetype_glyph_data_t* glyph);
 
#ifdef __cplusplus
}
#endif
#endif /* FREETYPE_API_H */
