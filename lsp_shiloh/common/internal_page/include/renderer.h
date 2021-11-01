/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
 
#ifndef RENDER
#define RENDER

//
// Used to specify the type of object.
//
#include "agprint.h"
#include "agLinkedList.h"
#include "agConnectMgr.h" 
#include "internal_page_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef RENDERER_USE_FONT_ARRAY
    #define TEXT_FONT 0
#else
    #define TITLE_FONT_1  7
    #define TITLE_FONT_2  12
    #define TEXT_FONT     4
    #define SMALL_FONT    3
#endif
#define REND_MAX_COLORS 6


#define JUSTIFY_LEFT -1
#define JUSTIFY_CENTERED 0
#define JUSTIFY_RIGHT 1


/**
 * This enum specifies the type of object in the current DISP_LIST object
 */
typedef enum
{
	e_Text=1,         ///< Text object see TEXT structure start at 1 for a null check in function table
	e_Graphics,     ///< Graphic object see GRAPHIC structure
    e_LinkGraphics, ///< Graphic object based on a linked list.
    e_DrawLine,      ///< draw line between DISP_LIST points OBJ_X,OBJ_Y and OBJ_X_LEN,OBJ_Y_LEN
    e_DrawCurve      ///< draw a curve passing all the points given 
} OBJ_TYPE;
///
/// This enum tells what to do with the data.
///
typedef enum
{
	e_nothing,					///< place and ignore anything underneath
	e_and,						///< and with the underlying data
	e_not_and,					///< not the object then and it in.
	e_not_or,					///< not the object then or it in.
	e_or,						///< or with the underlying data
	e_xor,						///< xor with the underlying data
    e_not_xor,                  ///< not and xor
    e_diag_down,				///< put a 45deg angle on the front of the object.
    e_diag_up,                   ///< put a 45deg angle up on the front of the object.
} OBJ_OPERATION;
///
/// This structure is used to place graphics in the display list.
///
typedef struct
{
	uint32_t			X_DIMENSION;	        ///< Width of the graphic object in pixels			
	uint32_t			Y_DIMENSION;            ///< Height of the graphic object in pixels
    uint32_t			LENGTH_X;				///< this is how long one x pass is in bytes.
	void			*K_OR_R_PLANE;               ///< Graphic object K or R plane
	void			*C_OR_G_PLANE;               ///< Graphic object C or G plane
	void			*Y_OR_B_PLANE;               ///< Graphic object Y or B plane
	void			*M_PLANE;               ///< Graphic object M plane
	void			*Kp_PLANE;              ///< Graphic object Kp plane
	void			*A_PLANE;               ///< Graphic object A plane
} GRAPHIC;
typedef struct
{
    uint32_t TemplateLength;              ///< The number of dots in the template
    uint8_t  *Template;                   ///< The template
} LINE_OBJ;


typedef struct
{
    uint16_t *data;      ///< data to draw
    uint16_t length;     ///< length of curve in points
    uint16_t LineWidth;  ///< line width
    float x_inc;       ///< step size in x
    float y_inc;       ///< step size in y
} CURVE;

///
/// This structure is for line drawing
///
typedef struct
{
    uint32_t          LineWidth;              ///< The width of the line in pixels.
    LINE_OBJ        *LineObj;               ///< Used as a template for dashed lines etc.
    uint32_t          DotCount;               ///< The count for dots within the lineobj
    uint32_t        Color;                  ///< bit map for colors.

} LINE;
//
/// This structure is used to place linked graphics on the page.
// 
typedef struct
{
    uint32_t      X_DIMENSION;                ///< Width of the graphic object
    uint32_t      Y_DIMENSION;                ///< Height of the graphic object
    uint32_t      LENGTH_X;                   ///< Length of one x pass in bytes.
    uint32_t      InputLPS;                   ///< Number of lines per stripe.
    LINK_HEADER InPlane[NUM_MARKING_COLOR_CHANNELS];        ///< Input plane linked list
    LINK_HEADER ROPlane[NUM_MARKING_COLOR_CHANNELS];        ///< Pointer to the rotated plane.
    uint16_t      RotLPS;                     ///< Number of lines/stripe for rotated image.
    bool        PlaneRotated[NUM_MARKING_COLOR_CHANNELS];   ///< Flag to say if data in ROPlane is valid.
} LINKGRAPHIC;

typedef enum
{
	e_OneQuarterDensity,    ///< Density of 25%
    e_OneHalfDensity,       ///< Patch density of 50%
    e_ThreeQuartDensity,    ///< Patch denisity of 75%
    e_FullDensity,          ///< Patch density of 100%
    e_NoDensity             ///< Blank
} PATCH_DEN;

//
// This structure specifies the text attributes
typedef struct
{
    ipage_justify_t justify;            //
    string_mgr_language_t lang;         ///< Language of text
    uint32_t			Color;				///< which color to place the font .  0 = cyan 1 = yellow etc.
	uint32_t			FONT;               ///< The font number to use in placing the text. 
                                        ///< or size of the text for ufst fonts
	char			*Data;				///< Null terminated text.
} TEXT;
//
// This structure is used to place patches on the page.
// 
#if 0
typedef struct
{
    uint16_t  X_Loc;
    uint16_t  Y_Loc;
    uint16_t  X_Size;
    uint16_t  Y_Size;
    COLOR   Color;
    PATCH_DEN Density;
    OBJ_OPERATION Operation;
} PATCHES;
#endif
//
// Font structure
//
typedef struct
{
	SINT16		Code;           ///< Ascii code
    SINT16		LeftOffset;     ///< Offset from the left side for start of graphic
    SINT16		TopOffset;      ///< Offset from the top for start of graphic
    SINT16		CharSpacing;    ///< Width of the character in pixels.
    SINT16		CharHeight;     ///< Height of the character
    SINT16		CharWidth;      ///< This is the width of the graphic of character.
    uint16_t		LineLength;     ///< This is the linelength in bytes of the character
    uint16_t		SpaceLength;    ///< Width of the space if used in this slot.
} FONT_FORMAT;
/*
    This struct specifies the linked list members for specifing the display 
    list for the renderer.  When the renderer is called a linked list of
    these objects is passed in for putting on the page.
*/

typedef struct DisplayList
{
	struct DisplayList		*NextObj; ///< pointer to next object on the list.
	uint32_t			OBJ_X;			///< x location of upper left corner
	uint32_t			OBJ_Y;			///< y location of upper left corner
	uint32_t			OBJ_X_LEN;		///< how long the x direction, For line, destination x point
	uint32_t			OBJ_Y_LEN;		///< how long the y directions is. For Line destination y point
	OBJ_OPERATION Operation;	///< How to merge the data into the strip.
	OBJ_TYPE	Type;           ///< The type of object to put down.
	void			*Object;    ///< Pointer to the object.
} DISP_LIST;
//
// Variable type.
//
typedef enum
{
	e_DSVariable,           ///< Normal Variable
    e_String,               ///< String variable
} VAR_TYPE;
///
/// Enum for paper orientation in the renderer
///
typedef enum
{
    e_Portrait=0,           ///< Print the internal page in Portrait orientation
    e_Landscape=1           ///< Print the internal page in Landscape orientation
} PAPER_ORIENT;

//
/// This structure is used to define variables in the data list.
//
typedef struct 
{
	DISP_LIST 	DispList;
    
} VAR_LIST;

typedef struct RendererSession_Tag{
    SINT32 CurX, CurY;
    uint32_t Dirty;
    uint32_t YLen;
    uint32_t PIXELLINE;
}RendererSession;
//
// This is used to tell the renderer what is black, all 1's or all 0's.
typedef enum
{
    REND_DATA_CMYK,
    REND_DATA_RGB,
    REND_DATA_MONO
} rend_byte_type_t;
///
/// Define the input structure for the renderer
// 
typedef struct
{
    DISP_LIST   *Page;  ///< The display list to put on the page.
    uint32_t      XSize;  ///< Size of the output in pixels.
    uint32_t      YSize;  ///< Size of the output in pixels.
    PAPER_ORIENT    Rotation;  ///< Rotation of the paper.
    ROTATE_DIR  ImagesRotation; ///< Rotation of the images on the page.
    uint32_t      X_Resolution;  ///< pixels per inch.
    uint32_t      Y_Resolution;  ///< pixels per inch.
    uint32_t      BPP;         ///< Bits per pixel.
    bool        GenerateStripe; ///< If set renderer will allocate the stripe and send it to the printer.
    LINK_HEADER Output[NUM_MARKING_COLOR_CHANNELS];     ///< Linked list of output buffers.
    uint32_t      OutBufLPS;  ///< Output buffer lines per stripe.
    uint16_t       Copies;     ///< Number of copies to output.
    ipage_output_data_packing_t packing;   ///< Used for packing, # of pixels per item.  ie for xrgb this is 4, rgb 3
    MODULE_ID   Dest;        ///< Module of where to send results.
    rend_byte_type_t data_type;
    uint32_t        language;
    ipage_colors_t  color_order[NUM_MARKING_COLOR_CHANNELS];
} RENDER_INPUT;

typedef void (*RENDERER_FUN)(RendererSession *pRSession,
                             unsigned char *Strip,
                             DISP_LIST *TempPage,
                             RENDER_INPUT *RendInput,
                             uint32_t Color,
                             uint32_t Rotate,
                             bool *strip_dirty,
                             uint32_t color_index);

typedef struct 
{
    OBJ_TYPE type;
    RENDERER_FUN  renderer;
}OBJ_RENDERER;


void rotate(uint32_t , uint32_t , uint32_t ,LINK_MEMBER  *, LINK_MEMBER *, uint8_t , ROTATE_DIR);

uint8_t get_pixel_stride(ipage_output_data_packing_t pack);
uint8_t get_pixel_offset(ipage_output_data_packing_t pack);
int setObjectRenderer(OBJ_TYPE type, RENDERER_FUN renderer);

void* Renderer(RENDER_INPUT *);

void GetPatch(GRAPHIC *, COLOR , PATCH_DEN );

void ShutDownRenderer(void);


/**
 *
 * \brief Initialize the Renderer
 *
 */
void renderer_init(void);


void CopyObj(unsigned char *Strip, GRAPHIC *Object, uint32_t Plane, DISP_LIST *DispObj, uint32_t Start, 
             uint32_t Length, uint32_t ObjOffset, uint32_t LineLength, uint32_t XOffset, uint32_t BPP,
             uint8_t pixel_stride, uint8_t pixel_offset, rend_byte_type_t data_type);

void BlobberTextRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            uint32_t Color, 
                            uint32_t Rotate,
                            bool *strip_dirty,
                            uint32_t color_index);

void GraphicsRenderer(RendererSession *pRSession, 
                      unsigned char *Strip, 
                      DISP_LIST *TempPage, 
                      RENDER_INPUT *RendInput, 
                      uint32_t Color, 
                      uint32_t Rotate, 
                      bool *strip_dirty,
                      uint32_t color_index );

void LinkGraphicsRenderer(RendererSession *pRSession, 
                        unsigned char *Strip, 
                        DISP_LIST *TempPage, 
                        RENDER_INPUT *RendInput, 
                        uint32_t Color, 
                        uint32_t Rotate,
                        bool *strip_dirty,
                        uint32_t color_index);

void LineRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            uint32_t Color, 
                            uint32_t Rotate,
                            bool *strip_dirty,
                            uint32_t color_index);

void RenderStringToStrip(char *inStr, char *stripData, int pixelDepth, int stripWidth, int stripHeight,
                         int fontSize, int xOrigin, int yOrigin);

void OutlineStrip(char *stripData, int pixelDepth, int stripWidth, int stripHeight, int lineWidth);

int deleteRenderer();
int isRendererBusy();
int startRenderer(RENDER_INPUT *RendInput, int modId);
void  terminateRenderering();


#ifdef DEBUG
void StripToSerial(char *stripData, int pixelDepth, int stripWidth, int stripHeight);
#define STRIP_TO_SERIAL(stripData, pixelDepth, stripWidth, stripHeight) StripToSerial(stripData, pixelDepth, stripWidth, stripHeight)
#else // #ifdef DEBUG
#define STRIP_TO_SERIAL(stripData, pixelDepth, stripWidth, stripHeight)
#endif // #ifdef DEBUG

#ifdef __cplusplus
}
#endif
#endif

