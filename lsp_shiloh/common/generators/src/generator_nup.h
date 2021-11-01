/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
 
#ifndef GEN_UP
#define GEN_UP

//
// Used to specify the type of object.
//
#include "agprint.h"
#include "list.h"
#include "agConnectMgr.h" 
#include "mlimiter.h"
#include "BigBuffer.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enum used to specify the order that colors are to be processed by the renderer
 * This also determines the order of colors in the packed output in addition to the bit 
 * mask defines below.  Currently we are set up for RGB packed data order.  To do bgr change
 * the order of the enum and the bit defines below.
 */
typedef enum 
{
    NUP_PAGE_RED,
    NUP_PAGE_GREEN,
    NUP_PAGE_BLUE,
    NUP_PAGE_CYAN=0,
    NUP_PAGE_MAGENTA,
    NUP_PAGE_YELLOW,
    NUP_PAGE_BLACK,
    NUP_PAGE_BLACK_PHOTO,
    NUP_PAGE_AUX
} nup_page_colors_t;

/**
 * \brief structure specifing how the output data is to be packed.
 */
typedef enum
{
    NUP_PAGE_PLANAR_INTERLEAVED,       ///< data is generated one plane/color data comes out
                                    ///< one strip of each color is generated then the next strip
    NUP_PAGE_PLANAR_ORDERED, ///< One entire plane of color is generated, then the next plane.
                          ///< The order is determined by the color order array
    NUP_PAGE_RGB_PACK,     ///< data is packed byte 1=R 2=G 3=B 4=R etc
    NUP_PAGE_RGBX_PACK,    ///< data is packed byte 1=R 2=G 3=B 4=X(don't care) 5=R etc
    NUP_PAGE_XRGB_PACK     ///< data is packed byte 1=X(don't care) 2=R 3=G 4=B 5=X 6=R etc.
} nup_page_output_data_packing_t;

/**
 * \brief Rotation values, in 90-degree increments, progressing
 *        CounterClockWise (CCW) in order.
 */
typedef enum
{
    nup_NoRotate,
    nup_Rotate90,
    nup_Rotate180,
    nup_Rotate270,
} nup_ROTATE_DIR;

///
/// This enum tells what to do with the data.
///
typedef enum
{
   nup_nothing,    ///< place and ignore anything underneath
    nup_and,       ///< and with the underlying data
    nup_not_and,   ///< not the object then and it in.
    nup_not_or,    ///< not the object then or it in.
    nup_or,        ///< or with the underlying data
    nup_xor,       ///< xor with the underlying data
    nup_not_xor,   ///< not and xor
    nup_diag_down, ///< put a 45deg angle on the front of the object.
    nup_diag_up,   ///< put a 45deg angle up on the front of the object.
} nup_OBJ_OPERATION;
///
/// This structure is used to place graphics in the display list.
///
typedef struct
{
    uint32_t X_DIMENSION; ///< Width of the graphic object in pixels 
    uint32_t Y_DIMENSION; ///< Height of the graphic object in pixels
    uint32_t LENGTH_X;   ///< this is how long one x pass is in bytes.
    void    *K_OR_R_PLANE;  ///< Graphic object K or R plane
    void    *C_OR_G_PLANE;  ///< Graphic object C or G plane
    void    *Y_OR_B_PLANE;  ///< Graphic object Y or B plane
    void    *M_PLANE;       ///< Graphic object M plane
    void    *Kp_PLANE;      ///< Graphic object Kp plane
    void    *A_PLANE;       ///< Graphic object A plane
} nup_GRAPHIC;

//
/// This structure is used to place linked graphics on the page.
// 
typedef struct
{
    uint32_t  X_DIMENSION;    ///< Width of the graphic object
    uint32_t  Y_DIMENSION;    ///< Height of the graphic object
    uint32_t  LENGTH_X;       ///< Length of one x pass in bytes.
    uint32_t  InputLPS;       ///< Number of lines per stripe.

    ATLISTENTRY* InPlane[NUM_MARKING_COLOR_CHANNELS];///< Input plane linked list
    ATLISTENTRY* ROPlane[NUM_MARKING_COLOR_CHANNELS];///< Pointer to the rotated plane.

    uint16_t  RotLPS;  ///< Number of lines/stripe for rotated image.
    bool    PlaneRotated[NUM_MARKING_COLOR_CHANNELS];   ///< Flag to say if data in ROPlane is valid.
    uint32_t    X_DIMENSION_UNPADDED;///< Width of the graphic object in pixels                        
} nup_LINKGRAPHIC;

typedef struct nup_DisplayList
{
   struct nup_DisplayList  *NextObj; ///< pointer to next object on the list.
    uint32_t  OBJ_X;      ///< x location of upper left corner
    uint32_t  OBJ_Y;      ///< y location of upper left corner
    uint32_t  OBJ_X_LEN;  ///< how long the x direction, For line, destination x point
    uint32_t  OBJ_Y_LEN;  ///< how long the y directions is. For Line destination y point
    nup_OBJ_OPERATION Operation; ///< How to merge the data into the strip.
        //OBJ_TYPE        Type;           ///< The type of object to put down.
    nup_LINKGRAPHIC* Object;    ///< Pointer to the object.
} nup_DISP_LIST;

///
/// Enum for paper orientation in the renderer
///
typedef enum
{
    nup_Portrait=0,   ///< Print the internal page in Portrait orientation
    nup_Landscape=1   ///< Print the internal page in Landscape orientation
} nup_PAPER_ORIENT;

typedef struct nup_RendererSession_Tag{
    SINT32 CurX, CurY;
    uint32_t Dirty;
    uint32_t YLen;
    uint32_t PIXELLINE;
}nup_RendererSession;
//
// This is used to tell the renderer what is black, all 1's or all 0's.
typedef enum
{
    NUP_REND_DATA_CMYK,
    NUP_REND_DATA_RGB,
    NUP_REND_DATA_MONO,
    NUP_REND_DATA_CMYK_4
} nup_rend_byte_type_t;
///
/// Define the input structure for the renderer
///
typedef struct
{
    nup_DISP_LIST   *Page;  ///< The display list to put on the page.
    uint32_t      XSize;  ///< Size of the output in pixels.
    uint32_t      YSize;  ///< Size of the output in pixels.
    nup_PAPER_ORIENT Rotation;  ///< Rotation of the paper.
    nup_ROTATE_DIR  ImagesRotation; ///< Rotation images on the page
    uint32_t      X_Resolution;  ///< pixels per inch.
    uint32_t      Y_Resolution;  ///< pixels per inch.
    uint32_t      BPP;         ///< Bits per pixel.
    //bool        GenerateStripe; ///< If set renderer will allocate the stripe and send it to the printer.
    ATLISTENTRY* Output[NUM_MARKING_COLOR_CHANNELS];  ///< Linked list of output buffers.
    uint32_t      OutBufLPS;  ///< Output buffer lines per stripe.
    uint16_t       Copies;     ///< Number of copies to output.
    nup_page_output_data_packing_t packing;   ///< Used for packing, # of pixels per item.  ie for xrgb this is 4, rgb 3
    MODULE_ID   Dest;        ///< Module of where to send results.
    nup_rend_byte_type_t data_type;
    //uint32_t        language;
    nup_page_colors_t  color_order[NUM_MARKING_COLOR_CHANNELS];
} nup_RENDER_INPUT;

typedef struct _nup_data_strip_s
{
    ATLISTENTRY node;
    BigBuffer_t* databuf;
    uint32_t num_lines;     ///< number of lines in the data buffer
    uint32_t data_len;      ///< length of the data buffer in bytes
}nup_data_strip_t;

#ifdef __cplusplus
}
#endif

#endif

