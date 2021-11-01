/****************************************************************************** 
 * Copyright (c) 2009-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include "agRouter.h"
#include "memAPI.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "agprint.h"
#include "agJobUtils.h"
#include "lassert.h"
#include "dprintf.h"
#include "logger.h"
#include "internal_page_api.h"
#include "internalPage.h"
#include "internal_page_config.h"

#define IPAGE_ALIGN(x, y) ((x) + ((unsigned int)(x) % (y) ? \
                     (y) - (unsigned int)(x) % (y) : 0))
#define max(a,b) ((a) < (b) ? (b) : (a))


struct internal_page_doc_s{
    
    DOCTYPE docType;
    
    int pageCount;
    int pageCursor;           //Point to current page
    uint32_t language;          // the text language.
    
    DISP_LIST *header;        //Header
    DISP_LIST *contents[10];  //Each page has its own list
    DocElement *docHead;         //Link list head  all doc elements
    DocElement *docTail;         //Link list tail  all doc elements
    
    DocBuffer *buffer;    //A list store required buffers at various parts of the code

    int justification; // indicator for left or right page justification
    ipage_config_t config; // the configuration of this page.
    void (*addContent)(struct internal_page_doc_s *pIPage, DISP_LIST *newContent);
    void (*addHeaderContent)(struct internal_page_doc_s *pIPage, DISP_LIST *newContent);
    void (*freeResources)(struct internal_page_doc_s *pIPage); //Free all buffers and links
    void (*formatDocument)( struct internal_page_doc_s *pIPage );
};

/**
 * \brief map from internalpage operation to render operation
 */
OBJ_OPERATION operation_map(ipage_raster_operation_t op)
{
    switch(op)
    {
        case IPAGE_OR:
            return e_or;
        case IPAGE_AND:
            return e_and;
        case IPAGE_NOT_AND:
            return e_not_and;
        case IPAGE_NOT_OR:
            return e_not_or;
        case IPAGE_XOR:
            return e_xor;
        case IPAGE_NOT_XOR:
            return e_not_xor;
        default:
            ASSERT(0);
    }
    return e_or;
}
/** \brief Routine to allocate a buffers in building a internal page document.
 * 
 * \param[in] pIPage   Internal page document to be processed.
 */
void *ipage_malloc( internal_page_doc_t *pIPage, int size )
{
    char * pMem = MEM_CALLOC(1, size );

    DocBuffer *pBuf = MEM_CALLOC(1,  sizeof( DocBuffer ) );

    ASSERT(pMem != NULL);
    ASSERT(pBuf != NULL);
    pBuf->buf = pMem;
    pBuf->next = pIPage->buffer;
    pIPage->buffer = pBuf;

    return pMem;
}

/** \brief Routine to free all the buffers used in building a internal page document.
 * 
 * \param[in] pIPage   Internal page document to be processed.
 */
void freeInternalPageResources( internal_page_doc_t *pIPage )
{
    DocBuffer * pBuf;
    DocBuffer *pBuf2;

    pBuf = pIPage->buffer;

    while ( pBuf )
    {
        pBuf2 = pBuf;
        pBuf = pBuf2->next;
        MEM_FREE_AND_NULL( pBuf2->buf );
        MEM_FREE_AND_NULL( pBuf2 );
    }
}

/** \brief Routine to add a single DISP_LIST to a internal page document
 *  
 * \param[in]  pIPage      Internal page document to be processed.
 * \param[in]  newContent  Content to add to the document.
 * */

void addInternalPageContent(internal_page_doc_t *pIPage, DISP_LIST *newContent )
{
    DocElement *pNewElement = ( DocElement * ) ipage_malloc( pIPage, sizeof( DocElement ) );
    pNewElement->type = E_DISP;
    pNewElement->pValue = newContent;

    pNewElement->iValue = 0;
    pNewElement->next = 0;

    //    CPDBG(("Add content!\n"));
    if ( pIPage->docHead == 0 )
        pIPage->docHead = pNewElement; //first in the list
    else
        pIPage->docTail->next = pNewElement;

    pIPage->docTail = pNewElement;
}
/**
 * \brief This adds a displays list item to the header display list.
 * For multi-page internal pages, whatever is on the header in this display list 
 * will be put on the page.  This adds an item to the header display list.
 * \param pIPage the internal page data structure.
 * \param[in] newContent A display list item to put on the header internal page.
 * \returns The item put on the page data structure header display list.
 */
void addInternalPageHeaderContent(internal_page_doc_t *pIPage, DISP_LIST *newContent )
{

    // put the item into the header.
    if(pIPage->header == NULL)
    {
        pIPage->header = newContent;
    } else
    {
        while(pIPage->header->NextObj != NULL);
        pIPage->header->NextObj = newContent;
    }
}
/**\brief Routine to insert a Hint mark to identify some format info.
 * 
 * \param[in] pIPage Internal page document to be processed
 * \param[in] hint   Hint to apply 
 */
void ipage_apply_hint( internal_page_doc_t *pIPage, ipage_hint_t hint )
{
    //DocElement *pNewElement = ( DocElement *) MEM_MALLOC( sizeof( DocElement ) );
    DocElement * pNewElement = ( DocElement * ) ipage_malloc( pIPage, sizeof( DocElement ) );

    pNewElement->type = E_HINT;
    pNewElement->iValue = hint;
    pNewElement->pValue = 0;
    pNewElement->next = 0;

    if ( pIPage->docHead == 0 )
        pIPage->docHead = pNewElement; //first in the list
    else
        pIPage->docTail->next = pNewElement;

    pIPage->docTail = pNewElement;
}


/**\brief Routine to format the DISP elements in the InternalPage to generate a page-based printout array
 * 
 * \param[in] pIPage  Internal page document to work on.
 * 
 * */
void formatInternalPageDoc( internal_page_doc_t *pIPage )
{
    int iPage;
    long pageBase = 0;
    long pageSide = 0; //0 is left, i is right  never changed ?db
    int tmpY;
    DISP_LIST *pDISP = 0;
    DISP_LIST *pContDISP = 0;
    DocElement *pElm;
    int breakFlag;

    for ( iPage = 0; iPage < 10; iPage++ )
        pIPage->contents[ iPage ] = 0;  //clear the page contents

    iPage = 0;

    pageBase = 0;

    pageSide = 0; //left column

    breakFlag = 0;

    pElm = pIPage->docHead;

    while ( pElm )
    {
        // currently at a breakable point, so see if we need to use it
        if ( ( pElm->type == E_HINT ) && ( pElm->iValue == IPAGE_BREAKABLE_POINT ) )
        {
            DocElement * pElm2 = pElm;

            // search for the next available breakpoint after this one
            while ( ( pElm2->next ) )
            {
                // if we reached the element just before next breakable point, check if this is at the same page!
                if ( ( ( pElm2->next ) ->type == E_HINT ) && ( pElm2->next->iValue == IPAGE_BREAKABLE_POINT ) )
                {
                    // if we have back to back breakable points, just skip this one
                    if (pElm2 == pElm)
                    {
                        break;
                    }

                    pDISP = ( DISP_LIST* ) pElm2->pValue;

                    if (e_DrawLine == pDISP->Type)
                    {
                        // for a line, tmpY is set to the max y value of either endpoint
                        tmpY = max( pDISP->OBJ_Y, pDISP->OBJ_Y_LEN );
                    }
                    else
                    {
                        // for all other objects, tmpY is set to the y value plus the y length
                        tmpY = ( pDISP->OBJ_Y + pDISP->OBJ_Y_LEN );
                    }

                    if ( ( tmpY - pageBase ) > ( ( int ) ( pIPage->config.page_block_y - pIPage->config.block_top ) ) )
                    {
                        breakFlag = 1; //Break now!
                    }

                    break;
                }

                pElm2 = pElm2->next;
            }
        }

        if ( pElm->type == E_DISP )
        {
            pDISP = ( DISP_LIST * ) pElm->pValue;

            if (e_DrawLine == pDISP->Type)
            {
                // for a line, tmpY is set to the max y value of either endpoint
                tmpY = max( pDISP->OBJ_Y, pDISP->OBJ_Y_LEN );
            }
            else
            {
                // for all other objects, tmpY is set to the y value plus the y length
                tmpY = ( pDISP->OBJ_Y + pDISP->OBJ_Y_LEN );
            }

            if ( ( tmpY - pageBase ) > ( ( int ) ( pIPage->config.page_block_y - pIPage->config.block_top ) ) || breakFlag > 0 )
            {  //page break;
                pageBase = pDISP->OBJ_Y;
                breakFlag = 0;

                pageSide = 0;  //back to left
                iPage++;
            }

            if ( ! pIPage->contents[ iPage ] )
                pIPage->contents[ iPage ] = pDISP; //Add to the head
            else
            {
                ASSERT(pContDISP);
                pContDISP->NextObj = pDISP; //add to the tail
            }

            pContDISP = pDISP;           //point to the tail

            //adjust Y location in page
            pContDISP->OBJ_Y = pContDISP->OBJ_Y - pageBase + pIPage->config.block_top;

            if (e_DrawLine == pContDISP->Type)
            {
                //adjust Y location of line endpoints
                pContDISP->OBJ_Y_LEN = pContDISP->OBJ_Y_LEN - pageBase + pIPage->config.block_top;  
            }

            if ( JUSTIFY_RIGHT == pIPage->justification )
            {
                //adjust X location in page 
                pContDISP->OBJ_X = pContDISP->OBJ_X + 
                                   ( pageSide > 0 ? -pIPage->config.left_2nd_margin : -pIPage->config.left_margin );

                if (e_DrawLine == pContDISP->Type)
                {
                    //adjust X location of line endpoints
                    pContDISP->OBJ_X_LEN = pContDISP->OBJ_X_LEN + 
                                           ( pageSide > 0 ? -pIPage->config.left_2nd_margin : -pIPage->config.left_margin );
                }
            }
            else
            {
                //adjust X location in page
                pContDISP->OBJ_X = pContDISP->OBJ_X + 
                                   ( pageSide > 0 ? pIPage->config.left_2nd_margin : pIPage->config.left_margin );

                if (e_DrawLine == pContDISP->Type)
                {
                    //adjust X location of line endpoints
                    pContDISP->OBJ_X_LEN = pContDISP->OBJ_X_LEN + 
                                           ( pageSide > 0 ? pIPage->config.left_2nd_margin : pIPage->config.left_margin );
                }
            }

            pContDISP->NextObj = 0;
            
           // CPDBG(("Obj y=%d, x=%d  y_len=%d, x_len=%d", pContDISP->OBJ_Y, pContDISP->OBJ_X, pContDISP->OBJ_Y_LEN, pContDISP->OBJ_X_LEN));
        }

        pElm = pElm->next;
    }
}
/**
 * \brief Set up the order that the page planes are rendered.
 * Copies from the plane order to our internal structure
 */
static void _internal_page_init_color_order(internal_page_doc_t *page, RENDER_INPUT *RendInput)
{
    int i;

    for(i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        RendInput->color_order[i] = page->config.color_order[i];
    }
}
/**
 * \brief Render the page to strips.
 * This takes the display list and renders the page into the data_list data.
 * Does not allocate any data, just returns the strips with the objects rendered
 * into the strips.
 * \param[in] page Data about the display list and page to render.
 * \param data_list The strips to render into
 */
void ipage_renderer_page_to_strips(internal_page_doc_t *page, LINK_MEMBER *data_list[NUM_MARKING_COLOR_CHANNELS])
{

    int iPage,i;
    DISP_LIST *headerTail;
    JOBINFO jobInfo;
    RENDER_INPUT RendInput;
    PAGEINFO   Info;
    error_type_t status;
    uint32_t xsize,ysize;

    // Zero all structs that we pass around, so members that are not
    // explicitly set below will not contain total garbage.
    memset(&jobInfo, 0, sizeof(JOBINFO));

    if (page->formatDocument)
        page->formatDocument(page );

    ASSERT(page->contents[1] == 0);
    iPage = 0;

    // Zero all structs that we pass around, so members that are not
    // explicitly set below will not contain total garbage.
    memset(&Info, 0, sizeof(PAGEINFO));
    memset(&RendInput, 0, sizeof(RENDER_INPUT));

    RendInput.BPP = page->config.bpp;
    RendInput.Copies = 1;
    RendInput.GenerateStripe = false;
    RendInput.ImagesRotation = e_NoRotate;
    RendInput.OutBufLPS = page->config.strip_size;
    RendInput.language = page->language;
    //
    // Put the output data into the render input.
    //
    for(i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        agAddHead(&RendInput.Output[i], data_list[i]);
    }
    _internal_page_init_color_order(page, &RendInput);
    //
    // Map from ipage colors to renderer colors.
    switch (page->config.color_type)
    {
        case IPAGE_MONO:
            RendInput.data_type = REND_DATA_MONO;
            break;
        case IPAGE_CMYK:
            RendInput.data_type = REND_DATA_CMYK;
            break;
        case IPAGE_RGB:
            RendInput.data_type = REND_DATA_RGB;
            break;
    }

    RendInput.packing = page->config.packing;
    if(page->header == NULL)
    {
        RendInput.Page = page->contents[ iPage ];
    } else
    {

        headerTail = page->header;
        while(headerTail->NextObj)
            headerTail = headerTail->NextObj;   // find the last item in the header
        headerTail->NextObj = page->contents[iPage];
        RendInput.Page = page->header;
    }
    RendInput.X_Resolution = page->config.resolution_x;
    RendInput.Y_Resolution = page->config.resolution_y;
    RendInput.Rotation = page->config.orientation;
    if((status = paper_get_media_size(page->config.mediasize, &xsize, &ysize)) == OK)
    {
        RendInput.XSize = (double)xsize/25.4 * RendInput.X_Resolution * RendInput.BPP;
        RendInput.YSize = (double)ysize/25.4 * RendInput.Y_Resolution;
    } else
    {
        // if we fail the call do a default.
        RendInput.XSize = (80 * RendInput.X_Resolution*RendInput.BPP)/10;
        RendInput.YSize = (105 * RendInput.Y_Resolution) / 10;
    }
    //
    // Make sure the size in bytes is a multiple of 16 bytes.
    // This is needed for the video to work correctly.
    //
    if (((RendInput.XSize/8)&0xf) != 0)
        RendInput.XSize = (((RendInput.XSize/8) & 0xfffffff0) + 0x10)*8;

    //
    // Get the media information.
    //

    // Setup the print structures 
    //
    Info.BitsPerPixel = RendInput.BPP;
    Info.PageType = 0;
    Info.PageSize = 0;
    Info.InputTray = 0;
    Info.PixelsPerLine = RendInput.XSize/RendInput.BPP;
    Info.Video_X = Info.PixelsPerLine;
    Info.Video_Y = IPAGE_ALIGN( RendInput.YSize, RendInput.OutBufLPS ); // set Video_Y to the page image height, aligned to the strip height
    Info.X_Resolution = RendInput.X_Resolution;
    Info.Y_Resolution = RendInput.Y_Resolution;
    Info.copies = RendInput.Copies;
    Info.RawData = TRUE;
    Info.TonerSaveMode = FALSE;
    Info.DocType = page->docType;

    //RendInput.YSize = (105 * RendInput.Y_Resolution*RendInput.BPP)/10;

    //CPDBG( ( "Sent out page %d ", iPage + 1 ) );

    {
        UINT32 param3;

        startRenderer(&RendInput, INTERNALMGRID);
        while (isRendererBusy())
        {
            if ((param3 = checkIPMCancel(200)))
            {
                terminateRenderering();
                while (isRendererBusy());
                ackIPMCancel(param3);   //ACK the cancel
                deleteRenderer();  //Remove render thread 
                return;
            }
        }
    }
    iPage++;

    //
    // This job is complete.
    //
    deleteRenderer();  //Remove render thread 
}

/**\brief Default print routine for the Menupage 
 * */
void ipage_print_page( internal_page_doc_t *pIPage, UINT32 Copies, MODULE_ID Dest)
{
    int iPage;
    DISP_LIST *headerTail;
    JOBINFO jobInfo;
    error_type_t ret;
    
    // Zero all structs that we pass around, so members that are not
    // explicitly set below will not contain total garbage.
    memset(&jobInfo, 0, sizeof(JOBINFO));

    if(pIPage->formatDocument)
        pIPage->formatDocument(pIPage );

    iPage = 0;
    //
    // Start the job.
    //
    jobInfo.DocType = pIPage->docType;
    jobInfo.JobName = NULL;
    jobInfo.job_pipe = NULL;
    jobInfo.JobMode = e_Normal;
//    jobInfo.JobData = e_PrintJob;
    StartJob(&jobInfo, NULL, Dest, mlimiter_by_name("ipage_print"));

    while ( pIPage->contents[ iPage ] )
    {
        RENDER_INPUT RendInput;
        PAGEINFO   Info;

        // Zero all structs that we pass around, so members that are not
        // explicitly set below will not contain total garbage.
        memset(&Info, 0, sizeof(PAGEINFO));
        memset(&RendInput, 0, sizeof(RENDER_INPUT));

        RendInput.Dest = Dest;  // initialize the destination of the data
        RendInput.BPP = pIPage->config.bpp;
        RendInput.Copies = Copies;
        RendInput.GenerateStripe = TRUE;
        RendInput.ImagesRotation = e_NoRotate;
        RendInput.OutBufLPS = pIPage->config.strip_size;
        RendInput.language = pIPage->language;
        _internal_page_init_color_order(pIPage, &RendInput);
        Info.monoOrColor = e_Color;
        switch(pIPage->config.color_type)
        {
            case IPAGE_MONO:
                Info.monoOrColor = e_Mono;
                RendInput.data_type = REND_DATA_MONO;
                break;
            case IPAGE_CMYK:
                Info.monoOrColor = e_Color;
                RendInput.data_type = REND_DATA_CMYK;
                break;
            case IPAGE_RGB:
                Info.monoOrColor = e_Color;
                RendInput.data_type = REND_DATA_RGB;
                break;
        }
        RendInput.packing = pIPage->config.packing;
        RendInput.Page = pIPage->contents[ iPage ];
        if(pIPage->header != NULL)
        {

            headerTail = pIPage->header;
            while(headerTail->NextObj)
                headerTail = headerTail->NextObj;   // find the last item in the header
            headerTail->NextObj = pIPage->contents[iPage]; // add to end of header
            RendInput.Page = pIPage->header;        // add in the header.
        }
        RendInput.X_Resolution = pIPage->config.resolution_x;
        RendInput.Y_Resolution = pIPage->config.resolution_y;
        RendInput.Rotation = pIPage->config.orientation;
       
        RendInput.XSize = (85 * RendInput.X_Resolution*RendInput.BPP)/10;
        //
        // Make sure the size in bytes is a multiple of 16 bytes.
        // This is needed for the video to work correctly.
        //
        if(((RendInput.XSize/8)&0xf) != 0)
            RendInput.XSize = (((RendInput.XSize/8) & 0xfffffff0) + 0x10)*8;
        RendInput.YSize = (110 * RendInput.Y_Resolution) / 10;

        //
        // Get the media information.
        //

        // Setup the print structures 
        //
        Info.BitsPerPixel = RendInput.BPP;
        Info.PageType = MEDIATYPE_PLAIN_PAPER;     // Currently the only supported print mode.
        Info.PageSize = 0;
        Info.InputTray = 0;
        Info.PixelsPerLine = RendInput.XSize/RendInput.BPP;
        Info.Video_X = Info.PixelsPerLine;
        Info.Video_Y = IPAGE_ALIGN( RendInput.YSize, RendInput.OutBufLPS ); // set Video_Y to the page image height, aligned to the strip height
        Info.X_Resolution = RendInput.X_Resolution;
        Info.Y_Resolution = RendInput.Y_Resolution;
        Info.copies = RendInput.Copies;
        Info.RawData = TRUE;
        Info.TonerSaveMode = FALSE;
        Info.DocType = pIPage->docType;

        StartPage(&Info, Dest, mlimiter_by_name("ipage_print"));
        //RendInput.YSize = (105 * RendInput.Resolution*RendInput.BPP)/10;

        //CPDBG( ( "Sent out page %d ", iPage + 1 ) );

        {
            UINT32 param3;

            ret = startRenderer(&RendInput, INTERNALMGRID);
            XASSERT(OK == ret, ret);
            while(isRendererBusy())
            {
                if((param3 = checkIPMCancel(200)))
                {
                    terminateRenderering();
                    while(isRendererBusy());
                    ackIPMCancel(param3);   //ACK the cancel
                    deleteRenderer();  //Remove render thread 
                    return;
                }
            }
        }

        EndPage(Dest, mlimiter_by_name("ipage_print"));
        iPage++;
        ret = deleteRenderer();  //Remove render thread
        XASSERT(OK == ret, ret);
    }
    //
    // This job is complete.
    //
    EndJob(Dest);
}

static void ipage_build_raster_linked_internal(internal_page_doc_t *page, int x0, int y0, 
                               int stretch_x, int stretch_y, uint32_t color,
                               ipage_raster_operation_t op, ipage_link_graphic_t *graphic, bool addheader)
{
    DISP_LIST *PatchDISP;
    LINKGRAPHIC   *PatchGRAPHIC;

    PatchDISP = ( DISP_LIST * ) ipage_malloc( page, sizeof( DISP_LIST ) );

    PatchDISP->OBJ_X = x0 + page->config.left_margin;  
    PatchDISP->OBJ_Y = y0;  
    PatchDISP->OBJ_X_LEN = stretch_x; 
    PatchDISP->OBJ_Y_LEN = stretch_y; 
    PatchDISP->Operation = operation_map(op);      //MenuPatch[ iPatch ].Operation;
    PatchDISP->Type = e_LinkGraphics;

    PatchGRAPHIC = ( LINKGRAPHIC * ) ipage_malloc( page, sizeof( LINKGRAPHIC ) );
    memset(PatchGRAPHIC, 0, sizeof(LINKGRAPHIC));
    PatchGRAPHIC->X_DIMENSION = graphic->x_dimension;
    PatchGRAPHIC->Y_DIMENSION = graphic->y_dimension;
    PatchGRAPHIC->LENGTH_X = graphic->length_x;
    PatchGRAPHIC->InputLPS = graphic->strip_size;
    // get the color planes initialized.
    if(color & COLOR_K_OR_R)
        agAddHead(&PatchGRAPHIC->InPlane[0], graphic->graphic_list);
    if(color & COLOR_C_OR_G)
        agAddHead(&PatchGRAPHIC->InPlane[1], graphic->graphic_list);
    if(color & COLOR_Y_OR_B)
        agAddHead(&PatchGRAPHIC->InPlane[2], graphic->graphic_list);
    if(color & COLOR_M)
        agAddHead(&PatchGRAPHIC->InPlane[3], graphic->graphic_list);
    if(color & COLOR_Kp)
        agAddHead(&PatchGRAPHIC->InPlane[4], graphic->graphic_list);
    if(color & COLOR_A)
        agAddHead(&PatchGRAPHIC->InPlane[5], graphic->graphic_list);

    PatchDISP->Object = PatchGRAPHIC;
    PatchDISP->NextObj = 0;
    if(addheader)
        page->addHeaderContent(page, PatchDISP);
    else
        page->addContent( page, PatchDISP );
}

void ipage_build_raster_linked(internal_page_doc_t *page, int x0, int y0, 
                               int stretch_x, int stretch_y, uint32_t color,
                               ipage_raster_operation_t op, ipage_link_graphic_t *graphic)
{
    ipage_build_raster_linked_internal(page,x0,y0,stretch_x,stretch_y,color,op,graphic, false);
}
void ipage_build_raster_linked_header(internal_page_doc_t *page, int x0, int y0, 
                               int stretch_x, int stretch_y, uint32_t color,
                               ipage_raster_operation_t op, ipage_link_graphic_t *graphic)
{
    ipage_build_raster_linked_internal(page,x0,y0,stretch_x,stretch_y,color,op,graphic, true);
}



static void ipage_build_raster_internal(internal_page_doc_t *pIPage, int x0, int y0, int stretch_x, int stretch_y, 
                     uint32_t color, ipage_raster_operation_t op,ipage_graphic_t *graphic, bool addheader)
{
    DISP_LIST *PatchDISP;
    GRAPHIC   *PatchGRAPHIC;

    PatchDISP = ( DISP_LIST * ) ipage_malloc( pIPage, sizeof( DISP_LIST ) );

    PatchDISP->OBJ_X = x0 + pIPage->config.left_margin;  
    PatchDISP->OBJ_Y = y0;  
    PatchDISP->OBJ_X_LEN = stretch_x; 
    PatchDISP->OBJ_Y_LEN = stretch_y; 
    PatchDISP->Operation = operation_map(op);      //MenuPatch[ iPatch ].Operation;
    PatchDISP->Type = e_Graphics;

    PatchGRAPHIC = ( GRAPHIC * ) ipage_malloc( pIPage, sizeof( GRAPHIC ) );
    memset(PatchGRAPHIC, 0, sizeof(GRAPHIC));   // initialize the structure
    PatchGRAPHIC->X_DIMENSION = graphic->x_dimension;
    PatchGRAPHIC->Y_DIMENSION = graphic->y_dimension;
    PatchGRAPHIC->LENGTH_X = graphic->length_x;
    // get the color planes initialized.
    if(color & COLOR_K_OR_R)
        PatchGRAPHIC->K_OR_R_PLANE = graphic->data;
    if(color & COLOR_C_OR_G)
        PatchGRAPHIC->C_OR_G_PLANE = graphic->data;
    if(color & COLOR_Y_OR_B)
        PatchGRAPHIC->Y_OR_B_PLANE = graphic->data;
    if(color & COLOR_M)
        PatchGRAPHIC->M_PLANE = graphic->data;
    if(color & COLOR_Kp)
        PatchGRAPHIC->Kp_PLANE = graphic->data;
    if(color & COLOR_A)
        PatchGRAPHIC->A_PLANE = graphic->data;

    PatchDISP->Object = PatchGRAPHIC;
    PatchDISP->NextObj = 0;
    if(addheader)
        pIPage->addHeaderContent(pIPage, PatchDISP);
    else
        pIPage->addContent( pIPage, PatchDISP );
}

void ipage_build_raster(internal_page_doc_t *pIPage, int x0, int y0, int stretch_x, int stretch_y, 
                     uint32_t color, ipage_raster_operation_t op,ipage_graphic_t *graphic)
{
    ipage_build_raster_internal(pIPage,x0,y0,stretch_x,stretch_y,color,op,graphic, false);
}
void ipage_build_raster_header(internal_page_doc_t *pIPage, int x0, int y0, int stretch_x, int stretch_y, 
                     uint32_t color, ipage_raster_operation_t op,ipage_graphic_t *graphic)
{
    ipage_build_raster_internal(pIPage,x0,y0,stretch_x,stretch_y,color,op,graphic,true);
}


static void ipage_build_line_internal( internal_page_doc_t *pMPage, int x0, int y0, int x1, int y1, 
                                uint8_t line_width, uint32_t color, bool addheader)
{
    DISP_LIST *strDISP;
    LINE      *strLINE;

    strDISP = ( DISP_LIST * ) ipage_malloc( pMPage, sizeof( DISP_LIST ) );
    //strDISP = ( DISP_LIST * ) getMemory( sizeof( DISP_LIST) , SCRATCHALLOC ); 
    strLINE = ( LINE * ) ipage_malloc( pMPage, sizeof( LINE ) );
   
    strLINE->LineWidth = line_width;
    strLINE->Color = color;


    strDISP->NextObj = 0;
    strDISP->OBJ_X_LEN = x1 + pMPage->config.left_margin;
    strDISP->OBJ_Y_LEN = y1;
    strDISP->Object = strLINE;
    strDISP->OBJ_X = x0 + pMPage->config.left_margin;
    strDISP->OBJ_Y = y0;
    strDISP->Operation = e_or;
    strDISP->Type = e_DrawLine;
    if(addheader)
        pMPage->addHeaderContent(pMPage, strDISP);
    else
        pMPage->addContent( pMPage, strDISP );
}

void ipage_build_line( internal_page_doc_t *pMPage, int x0, int y0, int x1, int y1, 
                                uint8_t line_width, uint32_t color)
{
    ipage_build_line_internal(pMPage,x0,y0,x1,y1,line_width,color,false);
}
void ipage_build_line_header(internal_page_doc_t *pMPage, int x0, int y0, int x1, int y1, 
                                uint8_t line_width, uint32_t color)
{
    ipage_build_line_internal(pMPage,x0,y0,x1,y1,line_width,color,true);
}
/**\brief Build a DISP for a text string on an internal page
 * */
static void ipage_build_text_internal( internal_page_doc_t *pMPage, int x, int y, char *oText, 
                    ipage_justify_t justify, UINT32 FONT, string_mgr_language_t lang,
                       uint32_t color, ipage_raster_operation_t op, bool addheader)
{
    //Build a text object for diaply
    DISP_LIST * strDISP;
    TEXT *strTEXT;
    char *text;
    
    text= ipage_malloc( pMPage, strlen(oText)+ 4 );
    strcpy(text, oText);
#if 0    
    {//Convert some special chars. TBD: Move to a new function
        char *p = text;
        while(*p)
        {
            switch(*p)
            {
                case 0x11:
                  *p = '>';
                  break;
                default:
                  break;  
            }
            p++;
        }
    }
#endif    
    //CPDBG( ( "Build text: %s", text ) );

    strDISP = ( DISP_LIST * ) ipage_malloc( pMPage, sizeof( DISP_LIST ) );
    strTEXT = ( TEXT * ) ipage_malloc( pMPage, sizeof( TEXT ) );

    strTEXT->Data = text;
    strTEXT->FONT = FONT;
    strTEXT->justify = justify;
    strTEXT->Color = color;
    strTEXT->lang = lang;

    strDISP->NextObj = 0;
    strDISP->OBJ_X_LEN = 0;
    strDISP->OBJ_Y_LEN = 0;
    strDISP->Object = strTEXT;
    strDISP->OBJ_X = x + pMPage->config.left_margin;
    strDISP->OBJ_Y = y;
    strDISP->Operation = operation_map(op);
    strDISP->Type = e_Text;
    if(addheader)
        pMPage->addHeaderContent(pMPage, strDISP);
    else
        pMPage->addContent( pMPage, strDISP );
}

void ipage_build_text( internal_page_doc_t *pMPage, int x, int y, char *oText, 
                    ipage_justify_t justify, UINT32 FONT, string_mgr_language_t lang,
                       uint32_t color, ipage_raster_operation_t op)
{
    ipage_build_text_internal(pMPage,x,y,oText,justify,FONT,lang,color,op, false);
}
void ipage_build_text_header(internal_page_doc_t *pMPage, int x, int y, char *oText, 
                    ipage_justify_t justify, UINT32 FONT, string_mgr_language_t lang,
                       uint32_t color, ipage_raster_operation_t op)
{
    ipage_build_text_internal(pMPage,x,y,oText,justify,FONT,lang,color,op,true);
}
/*Build a new internal page document*/
internal_page_doc_t *ipage_create_page(DOCTYPE docType)
{
   internal_page_doc_t *pDoc = MEM_CALLOC(1, sizeof( internal_page_doc_t ) );
    
    ASSERT( pDoc);
    
    pDoc->docType = docType;
    pDoc ->addContent = &addInternalPageContent;
    pDoc->addHeaderContent = &addInternalPageHeaderContent;
    pDoc ->freeResources = &freeInternalPageResources;
    pDoc->formatDocument =&formatInternalPageDoc;
    pDoc->language = LANG_ENGLISH;

    // determine whether to use right or left justified formatting
    pDoc->justification = -1;//isRightToLeftLang( DSGetIntVar( e_CurrentLang ) ) ? JUSTIFY_RIGHT : JUSTIFY_LEFT;

    pDoc->docHead = 0;
    pDoc->buffer = 0;

    // set the system default values 
    ip_set_default_config(&pDoc->config);

    return pDoc;
}

/*Delete an internal page doc and release all resources it uses*/
void ipage_delete_page(internal_page_doc_t *pDoc)
{
    ASSERT( pDoc);
    pDoc->freeResources( pDoc );
    MEM_FREE_AND_NULL(pDoc);
}
void ipage_set_left_margin(internal_page_doc_t *page, int margin)
{
    page->config.left_margin = margin;
}
void ipage_set_2nd_margin(internal_page_doc_t *page, int margin)
{
    page->config.left_2nd_margin = margin;
}
int ipage_get_left_margin(internal_page_doc_t *page)
{
    return page->config.left_margin;
}
int ipage_get_2nd_margin(internal_page_doc_t *page)
{
    return page->config.left_2nd_margin;
}
void ipage_get_config(internal_page_doc_t *page, ipage_config_t *config)
{
    ASSERT(config);     // make sure it is somewhat valid
    memcpy(config, &page->config,sizeof(ipage_config_t));
}
void ipage_set_config(internal_page_doc_t *page, ipage_config_t *config)
{
    ASSERT(config);
    memcpy(&page->config, config, sizeof(ipage_config_t));
}
//------------------------------------------------------------------------------
// FUNCTION NAME: justifyXCoord
/**
 * \brief Utility function to accept an x coordinate intended to be used on a left
 *        justified internal report and if needed return a modified coordinate that
 *        that can be used on a right justified internal report.
 *
 * \param leftJustifyX (int) Desired x coordinate for a left justified report.
 * \param justification (int) Desired justification for the current report.
 * 
 * \return int
 * \retval The x coordinate to use for the current report justification.
 *
 * \author Jeff Aguilera 2009/09/14
 *
 **/
int justifyXCoord( int leftJustifyX, int justification )
{
    return ( ( JUSTIFY_RIGHT == justification ) ? ( PAGE_WIDTH - leftJustifyX ) : leftJustifyX );
}


