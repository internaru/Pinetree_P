/*
 *
 * ============================================================================
 * Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/  
/**
 * \file internal_page_api.h
 *
 * \brief This file specifies the api for building internal pages.  
 *
 * \note The purpose of this api is to provide simple tools to allow the building
 * of internal pages.  It is not a pdl so is very limited.
 *
 * Internal page generation is a 2 step process.  The first step is generating
 * a display list of all the objects to put on the page.  More on the objects
 * later.  After the display list the objects are rendered onto the output page.
 * This supports multiple pages.  To do more than 1 page, just put down objects
 * past the first page and this will generate another page with a header added.
 * The header is something that will be played on each page in a job.  So anything
 * with header in the name will put that object on each page.
 * There are 3 object type supported by this interface.
 * 1.  Raster.  A raster is a bit map that is to be placed on a page.  The bitmap
 *     is put onto the page using a raster operation.  You can specify the origin of 
 *     of the object, the color, and the object can be duplicated.  If you specify
 *     a stretch value of 2x the dimension of the object it will be copied 2x in
 *     that dimension.  This is useful for applying half-tone screens or shading
 *     to the object.
 * 2.  Link Raster.  This is the same as a raster object except the input data is presented 
 *     in a linked list.
 * 3.  Text object.  This allows the placement of a string on the page.  The string 
 *      can be put anywhere on the page.  The origion of the string is usually the
 *     lower left corner of the first character of the string.  There can be 
 *     characters that go below this point, such as "g" so you should allow enough
 *     room for all the decenders in the string.  You can also specify the justification 
 *     of the string.
 * 4.  line object.  This allows the drawing of a line from point a to point b.  You can
 *     specify the color and width of the line.  The width is in pixels.
 *
 * This API is now extended to include the ability to dynamically register
 * permanent internal pages.  These functions are marked with a hashtag
 * of #pagebackend.  The pages can be invoked using the ipage_invoke_page()
 * function
 *
 */

#ifndef _INTERNAL_PAGE_API_H
#define _INTERNAL_PAGE_API_H
#include <stdint.h>
#include "agConnectMgr.h"
#include "string_mgr_api.h"
#include "agLinkedList.h"
#include "platform.h"
#include "paper_size_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "event_observer.h"

/**
 *
 * \brief A callback function used to invoke a permanent internal page that has
 * been registered using the ipage_register_page function
 *
 * \param name The NULL-terminated string that was used to register the
 * permanent page
 *
 * \param context The context parameter that was used when the permanent
 * page was registered
 *
 * #pagebackend
 *
 */
typedef error_type_t (*ipage_invoke_page_t)( const char *name, void *context );


typedef struct internal_page_doc_s internal_page_doc_t;
/**
 * \brief Enum used to specify the order that colors are to be processed by the renderer
 * This also determines the order of colors in the packed output in addition to the bit 
 * mask defines below.  Currently we are set up for RGB packed data order.  To do bgr change
 * the order of the enum and the bit defines below.
 */
typedef enum 
{
    IPAGE_RED,
    IPAGE_GREEN,
    IPAGE_BLUE,
    IPAGE_CYAN=0,
    IPAGE_MAGENTA,
    IPAGE_YELLOW,
    IPAGE_BLACK,
    IPAGE_BLACK_PHOTO,
    IPAGE_AUX
} ipage_colors_t;
/**
 * \brief defines for bit masks for color  should be 2^(enum value)
 */
#define COLOR_K_OR_R  1
#define COLOR_C_OR_G  2
#define COLOR_Y_OR_B  4
#define COLOR_M       8
#define COLOR_Kp      16
#define COLOR_A       32
#define COLOR_RGB_BLACK (COLOR_K_OR_R|COLOR_C_OR_G|COLOR_Y_OR_B)
///
/// Enum for paper orientation in the renderer
///
typedef enum
{
    IPAGE_PORTRAIT=0,           ///< Print the internal page in Portrait orientation
    IPAGE_LANDSCAPE=1           ///< Print the internal page in Landscape orientation
} ipage_paper_orientation_t;

typedef enum
{
    IPAGE_JUSTIFY_LEFT = -1,
    IPAGE_JUSTIFY_CENTER = 0,
    IPAGE_JUSTIFY_RIGHT = 1
} ipage_justify_t;
/**
 * \brief Raster operations for putting down data.
 */
typedef enum
{
    IPAGE_AND, ///< and with the underlying data
    IPAGE_OR,      ///< or with the underlying data
    IPAGE_NOT_AND, ///< not source and add with underlying data
    IPAGE_NOT_OR,  ///< not source and or with underlying data
    IPAGE_XOR,     ///< xor source with underlying data
    IPAGE_NOT_XOR, ///< not source then xor with underlying data.
} ipage_raster_operation_t;
/**
 * \brief Hints to put into the display list
 * IPAGE_BREAKABLE_POINT is used to delimit a block that must stay together 
 * on a given page.  So if you send the hint, send a series of display list
 * items, and then send another hint, that series of display list items will
 * remain together on a given page.
 */

typedef enum 
{
    IPAGE_BREAKABLE_POINT,
}ipage_hint_t;
/**
 * \brief type of data to output. 
 * This mostly affects the sense of the data put down.
 * For Mono and cmyk 0 is nothing put down, white, and 1 is something put down
 * for rgb it is the opposite.  So for black rgb is 0,0,0 and white is 1,1,1
 * for cmy white is 0,0,0,0 and black is 1,1,1.  When you set ipage_rgb the page
 * is initialized to 1,1,1 while with the others it is initialized to 0.
 */

typedef enum
{
    IPAGE_MONO,
    IPAGE_CMYK,
    IPAGE_RGB
} ipage_output_color_t;
/**
 * \brief structure specifing how the output data is to be packed.
 */
typedef enum
{
    IPAGE_PLANAR_INTERLEAVED,       ///< data is generated one plane/color data comes out
                                    ///< one strip of each color is generated then the next strip
    IPAGE_PLANAR_ORDERED, ///< One entire plane of color is generated, then the next plane.
                          ///< The order is determined by the color order array
    IPAGE_RGB_PACK,     ///< data is packed byte 1=R 2=G 3=B 4=R etc
    IPAGE_RGBX_PACK,    ///< data is packed byte 1=R 2=G 3=B 4=X(don't care) 5=R etc
    IPAGE_XRGB_PACK     ///< data is packed byte 1=X(don't care) 2=R 3=G 4=B 5=X 6=R etc.
} ipage_output_data_packing_t;

#define IPM_NOTIFICATION_ID 0xfeef

typedef enum 
{
    ipm_JobNone,            /* invalid job state */
    ipm_JobReceived,        /* Internal Page Manager received the job request, but not request the resources required by the job */
    ipm_JobStart,           /* requested the resources successfully, start to run the job */
    ipm_JobDone,            /* the job done successfully */
    ipm_JobCancel,          /* the job cancel */   
} ipm_job_status_t;


ipm_job_status_t ipm_get_job_status( Subject_t *s);
DOCTYPE ipm_get_doc_type(Subject_t *s);
void ipm_attach_observer(Observer_t *o);
void ipm_detach_observer(Observer_t *o);


#define MAX_IPAGE_COLORS_IN_CONFIG 10 

/**
 * \brief Structure for configuring the internal pages
 */
typedef struct
{
    uint8_t bpp;            ///< bits per pixel for this page.  Allowable are 1,2,4,8
    ipage_paper_orientation_t orientation; ///< wheather this page is portrait or landscape
    uint32_t resolution_x;  ///< resolution in pixels/inch
    uint32_t resolution_y;  ///< resolution in pixels/inch
    uint32_t strip_size;    ///< The height of a generated strip in lines
    uint32_t left_margin;      ///< Left margin
    uint32_t left_2nd_margin;  ///< Left margin for right column in 2-column page layout
    uint32_t page_block_y;     ///< Page break location (all components after this will be rendered
                               ///< on a subsequent page)
    uint32_t block_top;        ///< Offset subtracted from page_block_y
    ipage_output_color_t color_type;  ///< The kind of data being generated
    ipage_output_data_packing_t packing; ///< the required packing of the output data.
    mediasize_t mediasize;          ///< the media size to use.
    ipage_colors_t color_order[MAX_IPAGE_COLORS_IN_CONFIG]; ///< the order that the planes will be generated.  
                            ///< Entry 0 is first generated, 1 is second etc.
    uint32_t num_color_order_entries; ///<The number of color_order entries populated in this config 
} ipage_config_t;
/**
 * \brief Data structure for defines of graphic objects.
 * The graphic object is a bit map in the bpp of the print we are doing.  This 
 * object is copied to the x,y location.  The length_x is the length of a line
 * of the object in bytes.
 */
typedef struct
{
    uint32_t x_dimension;   ///< The width of the object in pixels.
    uint32_t y_dimension;   ///< The height of the object in lines/pixels.
    uint32_t length_x;      ///< The length of a line of the object in bytes.
    void    *data;          ///< pointer to the raster of the object.
} ipage_graphic_t;
/**
 * \brief Data structure for the definition of linked graphic objects.
 * The object to put in is a linked list of data stips.  The object is copied
 * into the output buffer with the specified operation
 */
typedef struct
{
    uint32_t x_dimension;       ///< The width of the object in pixels.
    uint32_t y_dimension;       ///< The height of the object in lines
    uint32_t length_x;          ///< The length of a line of the object in bytes.
    uint32_t strip_size;        ///< the number of lines in a strip of input data
    LINK_MEMBER *graphic_list;   ///< The input list of the object.
} ipage_link_graphic_t;


/*
 * The following set of functions are general functions for internal pages.
 */ 
/**
 * \brief initialize an internal page structure.  Ready for building a display list.
 * \param[in] doc_type The type of doc to be printing.
 * \returns internal_page_doc_t *
 * \retval NULL Unable to initialize the internal page stuff
 * \retval !NULL success
 */
internal_page_doc_t *ipage_create_page(DOCTYPE doc_type);
/**
 * \brief Clean up an internal page job. 
 * This frees everthing on the display list, data pointers and goes back to idle
 * \param[in] page Pointer to the returned value from ipage_create_page
 * \returns nothing
 */
void ipage_delete_page(internal_page_doc_t *page);
/**
 * \brief Get the configuration of the internal page "page" and return it.
 * config points to a memory area at least sizeof(ipage_config_t) long that
 * is to receive the config information.
 * \param[in] page Pointer from a created internal page object.
 * \param[in,out] config pointer to an area to receive the config data.
 */
void ipage_get_config(internal_page_doc_t *page, ipage_config_t *config);
/**
 * \brief Set the configuration of an internal page.
 * This can be called at any time up to the call of ipage_print_page.  That is
 * when the configuration is used to generate a page.
 * \param[in] page Pointer from a created internal page object.
 * \param[in] config A pointer to an area that has the config info
 */
void ipage_set_config(internal_page_doc_t *page, ipage_config_t *config);
/**
 * \brief Process the display list and print the internal page.
 * \param[in] page Pointer to an internal page object.
 * \param[in] copies The number of copies to do.
 * \param[in] destination Where to send the print job.
 */
void ipage_print_page(internal_page_doc_t *page, uint32_t copies, MODULE_ID destination);
/**
 * \brief Run the renderer and put the output into input_list. 
 * \param[in] page The page object
 * \param[in] data_list This is a list of buffers that the renderer will render into. 
 *            These buffers must match the parameters for the data.  They must be 
 *            strippage_size lines long and the length must be the paper width * bpp *
 *            packing stuff.  The strips must also be initialized to the background value
 *            The data is renderered into this set of buffers.
 */
void ipage_renderer_page_to_strips(internal_page_doc_t *page, LINK_MEMBER *data_list[4]); 
/**
 * \brief Allocate memory that is handled by the display list.
 * Any memory that is allocated by this function will be freed by the ipage_delete_page
 * function above.  Using a normal malloc the user is responsible for his own frees 
 * after a job.
 * \param[in] page Pointer to a created internal page object.
 * \param[in] size the number of bytes to allocate
 */
void *ipage_malloc( internal_page_doc_t *page, int size );
/*
 * This set of functions is for adding things to the display list of the internal page
 */
/**
 * \brief Put a text object into the display list for the internal page.
 * This takes an string and makes an entry for the string in the display list for 
 * the page being printed.  The x and y values
 * are relative to the origin of the string which is normally the lower left side
 * of the string.  Strings that are right to left are processed so the characters are left 
 * to right.  This always puts down characters left to right.
 * \param[in] page A page object returned from ipage_create_page
 * \param[in] x The x location of the string start in pixels.  
 * \param[in] y The y location of the string start in pixels.
 * \param[in] text The string to put down.
 * \param[in] justify Justifaction of the string either 
 *              JUSTIFY_LEFT, RIGHT, or centered (see macros above)
 * \param[in] pt_size The size of the text to put down in points.
 * \param[in] lang The language of this string.  Used to select the font.
 * \param[in] color Bit map of the colors to put down.  If the internal page is 
 *                  configured for rgb data then a mask of 0x5 will put this text
 *                  onto the red and blue planes.
 * \param[in] op The raster operation to perform on this text when placing it down.  
 *               or for example will or this text with the underlying data.
 * \returns void
 */
void ipage_build_text(internal_page_doc_t *page, int x, int y,  
                char *text, ipage_justify_t justify, uint32_t pt_size, 
                string_mgr_language_t lang, uint32_t color, ipage_raster_operation_t op);
/**
 * \brief identical to ipage_build_text, except this is put into the header 
 * The header is that part of the page that is duplicated on each page.
 */
void ipage_build_text_header(internal_page_doc_t *page, int x, int y,  
                char *text, ipage_justify_t justify, uint32_t pt_size, 
                string_mgr_language_t lang, uint32_t color, ipage_raster_operation_t op);

/**
 * \brief This puts a raster object into the display list.
 * A raster object is a block that is the same resolution as the page.  When this is placed 
 * it is copied into the page with the operation done on the data.  The orgion for this
 * object is the lower left corner of the graphic object.  Stretching allows the
 * object to be put over any length of height of area.  The object is simply copied
 * the number of times or partial times required to make the raster object cover
 * the stretch number.  This is useful for applying half tone screens.  One cell is
 * stored and then the cell is stretched over x and y to cover the area to be
 * half-toned.
 * \param[in] page Pointer to an internal page object
 * \param[in] x0 The x location of the object.
 * \param[in] y0 The y location of the object.
 * \param[in] stretch_x if 0 copy full object.  > 0 copy the object however many times 
 *                  required to insure that stretch_x length is covered by the object.
 * \param[in] stretch_y if 0 copy full object. > 0 copy the object however many times
 *                  required to insure that stretch_y length is covered by the object.
 * \param[in] color Bit map of the colors to put down.  If the internal page is 
 *                  configured for rgb data then a mask of 0x5 will put this text
 *                  onto the red and blue planes.
 * \param[in] op The raster operation to perform on this text when placing it down.  
 *               or for example will or this text with the underlying data.
 * \param[in] graphic Pointer to the graphic object that holds the raster.
 */
void ipage_build_raster(internal_page_doc_t *page, int x0, int y0, int stretch_x, int stretch_y, 
                     uint32_t color, ipage_raster_operation_t op, ipage_graphic_t *graphic);
/**
 * \brief This is identical to ipage_build_raster except this is put into the header which is 
 *      put at the top of all pages.
 */
void ipage_build_raster_header(internal_page_doc_t *pIPage, int x0, int y0, int stretch_x, int stretch_y, 
                     uint32_t color, ipage_raster_operation_t op,ipage_graphic_t *graphic);
/**
 * \brief put a linked raster image onto the page.
 * \param[in] page Pointer to an internal page object
 * \param[in] x0 x coordinate of the upper left of the graphics object.
 * \param[in] y0 y coordiante of the upper left of the graphics object.
 * \param[in] stretch_x if 0 copy the full size of the object.  If > 0 copy the object
 *              however many times is required to insure that stretch_x length is covered
 *              by the object.
 * \param[in] stretch_y if 0 copy full object.  If > 0 copy the object the number of times
 *              that is required to insure that stretch_x length in pixels is covered by
 *              the object.
 * \param[in] color Bit map of the colors to put down.  If the internal page is 
 *                  configured for rgb data then a mask of 0x5 will put this text
 *                  onto the red and blue planes.
 * \param[in] op The raster operation to perform on this object when placing it down.
 * \param[in] graphic The raster object that has the linked list of the object
 */
void ipage_build_raster_linked(internal_page_doc_t *page, int x0, int y0, 
                               int stretch_x, int stretch_y, uint32_t color,
                               ipage_raster_operation_t op, ipage_link_graphic_t *graphic);
/**
 * \brief identical to ipage_build_raster_linked except this adds to the header.
 * The header is added to each page.
 */
void ipage_build_raster_linked_header(internal_page_doc_t *page, int x0, int y0, 
                               int stretch_x, int stretch_y, uint32_t color,
                               ipage_raster_operation_t op, ipage_link_graphic_t *graphic);
/**
 * \brief draw a line on the page.
 * \param[in] page Pointer to an internal page object
 * \param[in] x0 x coordinate of the first endpoint of the line
 * \param[in] y0 y coordiante of the first endpoint of the line
 * \param[in] x1 x coordinate of the last endpoint of the line
 * \param[in] y1 y coordinate of the last endpoint of the line
 * \param[in] line_width The width of the line to draw in pixels.
 * \param[in] color Bit map of the colors to put down.  If the internal page is 
 *                  configured for rgb data then a mask of 0x5 will put this text
 *                  onto the red and blue planes.
 */
void ipage_build_line( internal_page_doc_t *page, int x0, int y0, int x1, int y1, 
                    uint8_t line_width, uint32_t color);
/**
 * \brief identical to ipage_build_line except this is put in the header.
 * The header is put on each page.
 */
void ipage_build_line_header( internal_page_doc_t *page, int x0, int y0, int x1, int y1, 
                    uint8_t line_width, uint32_t color);
/**
 * \brief stick a hint into the display list.
 * \param[in] page Pointer to an internal page object
 * \param[in] hint The hint to insert
 */

void ipage_apply_hint( internal_page_doc_t *page, ipage_hint_t hint );
/*
 * This set of function is for changing parameters on the internal page.
 */

/**
 * \brief set the left_margin of the internal page
 * \param[in] page Pointer to the internal page object.
 * \param[in] margin the left margin value
 */
void ipage_set_left_margin(internal_page_doc_t *page, int margin);
/**
 * \brief Get the left margin of the internal page.
 * \param[in] page Pointer to the internal page object.
 * \returns int
 * \retval The left margin of the internal page
 */
int ipage_get_left_margin(internal_page_doc_t *page);


/**
 *
 * \brief Initialize the internal page manager
 *
 */
void ipage_init(void);


/**
 *
 * \brief Register a permanent page with the internal page manager.  For
 * example a "configuration" page may be registered.  A "configuration" page
 * would contain configuration information about the device. Other modules
 * may use the ipage_invoke_page function to start the page.
 *
 * \param name A unique identifier for the page, a const char pointer to a 
 * NULL-terminated string
 *
 * \param starts_own_job true if this page starts its own job, false otherwise
 *
 * \param invoke_function A callback function that will be issued whenever
 * the page is to be rendered
 *
 * \param context opaque data to the internal page manager, but will
 * be bassed as a parameter to the callback function
 *
 * \return OK upon success, FAIL otherwise
 *
 * #pagebackend
 *
 */
error_type_t ipage_register_page( const char *name,
                                  bool starts_own_job,
                                  ipage_invoke_page_t invoke_function,
                                  void *context );


/**
 *
 * \brief Invoke a permanent page with the internal page manager.
 *
 * \param name The unique identifier for the page, as registered using
 * ipage_register_page. A const char pointer to a NULL-terminated string
 *
 * \return OK upon success, FAIL otherwise
 *
 * #page
 *
 */
error_type_t ipage_invoke_page( const char *name );


#ifdef __cplusplus
}
#endif

#endif
