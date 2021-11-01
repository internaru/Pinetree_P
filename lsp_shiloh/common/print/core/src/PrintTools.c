/*
 *
 * ============================================================================
 * Copyright (c) 2006-2011   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include <string.h>

#ifdef HAVE_RTOS
#include "tx_api.h"
#endif
#include "memAPI.h"
#include "print_job_types.h"
#include "agMessage.h"


#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
//#include "agRotate.h" //lsptodo ???
#endif

#include "debug.h"
#include "logger.h"
#include "PrintTools.h"
#include "lassert.h"
#include "print_mode_api.h"
//------------------------------------------------------------------------------
// Macro Definitions
//------------------------------------------------------------------------------
#define DBG_PRFX "printtools: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(11)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Implementations
//------------------------------------------------------------------------------



/* FUNCTION NAME: SendPageStart */

/** 
 * \brief
 * 
 * \param TempPage
 * 
 * \retval None
 * 
 * \author db
 * 
 * \date 1/15/2007
 * 
 **/
void SendPageStart(PAGE_DATA *TempPage)
{
    ASSERT(IS_PAGE_DATA(TempPage));
    TempPage->PageReadyToSend = 0;      // hang onto the page till all in.
    TempPage->Racing = 0;
    // BUGBUG: should be done in engine/mech; not here TempPage->CanDeleteData = TRUE;
}


void AddPlaneToPage(PAGE_DATA *TempPage, void *Plane)
{
}



/* FUNCTION NAME: AddRasterToPlane */

/**
 * \brief Adds raster data pointed to by Mesag->param3 to the proper color plane
 *        associated with the page pointed to by TempPage.
 *
 * \param TempPage (PAGE_DATA*) Pointer to page to add raster data to.
 *
 * \param Mesag (MESSAGE*) Pointer to message that points to raster data.
 *
 * \param UseCount (UINT16) Use count to assign.
 *
 * \return void
 *
 * \date 1/15/2007
 *
 * This function assumes all pointers passed in are valid.
 *
 **/
void AddRasterToPlane(PAGE_DATA *pPage, MESSAGE *Mesag, UINT16 UseCount)
{
    PLANED*         pPlane = (PLANED*)Mesag->param3;
#ifdef HAVE_RTOS
    uint32_t interrupts;
#endif

    ASSERT(IS_PAGE_DATA(pPage));
    ASSERT(IS_PLANED(pPlane));

    // Set up the data use count.
    pPlane->UseCount = UseCount;
    if ( pPlane->big_buffer != NULL )
    {
        DBG_MEMLOG_WARNING("%s plane color: %d  msg color: %d plane 0x%x, data 0x%x\n", 
                  __func__, pPlane->sysPlaneColor, Mesag->param1, pPlane, pPlane->big_buffer->data);
    }
    else
    {
        DBG_MEMLOG_WARNING("%s plane color: %d  msg color: %d plane 0x%x, data NULL (empty plane)\n", 
                  __func__, pPlane->sysPlaneColor, Mesag->param1, pPlane );
    }

    // If racing make the use count on this plane = 1
   
    // If racing the page pointer is also down at the video so
    // disable ints while adding to the tail. If not racing the
    // whole page is sitting here in the job mgr so don't need to.
    if(pPage->Racing == 1)
    {
        pPlane->UseCount = 1;
    }

    // REVISIT  Shouldn't have to do this here - somehow the color gets LOST!
    pPlane->sysPlaneColor = (COLOR) Mesag->param1;

    // Set the "next print node" pointers for each color on the 
    // page. This pointer will travel the list, leaving it intact.
    // The list of plane data is maintained for the case of page
    // reprint; e.g. paper out or paper jam.

    pPage->numPlaneBuffers[pPlane->sysPlaneColor]++;

    if ((ROTATE_DIR)GetRotation(pPage->DUPLEX) == e_Rotate180)
    {
        DBG_MEMLOG_ERR("Plane add rotated %d \n", pPlane->LastData );
        // clear last data marker from all strips
        pPlane->LastData = false;

        // if this is the first plane it should have the last data marker since we are rotating
        if(MIsListEmpty(&pPage->planeDataPendingPrintList[pPlane->sysPlaneColor]))
        {
            pPlane->LastData = true;
        }
	    MInsertHeadList(&pPage->planeDataPendingPrintList[pPlane->sysPlaneColor], &pPlane->listNode);

        //Rolig: REVISIT TODO: Figure out how to avoid the case of racing a
        // page that is to be rotated! Ain't gonna work!
    }
    else // Not rotated 180 degrees.
    {
	    MInsertTailList(&pPage->planeDataPendingPrintList[pPlane->sysPlaneColor], &pPlane->listNode);
    }

}  


/* FUNCTION NAME: SendPageEnd */

/** 
 * \brief
 * 
 * \param TempPage
 * 
 * \param Mesag
 * 
 * \return void
 * 
 * \date 1/15/2007
 * 
 **/
void SendPageEnd(PAGE_DATA *TempPage, MESSAGE *Mesag)
{
    ASSERT(IS_PAGE_DATA(TempPage));

    //
    // Put the pixel counts into the page header.
    //
#if defined(PIXELCNT_BYHOST) || defined ( PIXELCNT_BYFW )
    {
        // attach the counts (if any, may be NULL) to the page.  Note that the memory must be freed
        // when the counts are used.
        TempPage->swPixCountPtr = (PIXELCNT *)Mesag->param3;
        if(TempPage->swPixCountPtr != NULL)
        {
            DBG_PRINTF_NOTICE("SendPageEnd page %#x, pixcounts: %#x, %#x, %#x, %#x\n", TempPage, TempPage->swPixCountPtr->pixelCntPerPage[0], TempPage->swPixCountPtr->pixelCntPerPage[1], TempPage->swPixCountPtr->pixelCntPerPage[2], TempPage->swPixCountPtr->pixelCntPerPage[3]);
            // Set the total page pixels.
            TempPage->swPixCountPtr->totalPagePixels = (uint32_t)TOTAL_PAGE_PIXELS;
        }
    }
#endif // #if defined(PIXELCNT_BYHOST) || defined(PIXELCNT_BYFW)

#if 0 // lsptodo: defined( HAVE_AUTOMATIC_DUPLEXER_SUPPORT )
    if ( (ROTATE_DIR)GetRotation(TempPage->DUPLEX) == e_Rotate180 )
    {
        FixUpRotatedPage( TempPage );
    }
#endif // defined( HAVE_AUTOMATIC_DUPLEXER_SUPPORT )

    //PrintPagePlaneInfo( TempPage ); // Comment in/out.

} // end SendPageEnd()

/** 
 * \brief
 * 
 * \param TempPage
 * 
 * \param Mesag
 * 
 * \return void
 * 
 * \date 1/15/2007
 * 
 **/
void SendPlaneEnd(PAGE_DATA *page, MESSAGE *Mesag)
{
    PLANED   *plane;
    COLOR plane_color = (COLOR) Mesag->param1;
    ATLISTENTRY* plane_node;

    // Done with this plane.  Set the flag in the data to indicate this is the 
    // last plane.

    if (page == NULL)
    {
        return;
    }

    ASSERT(IS_PAGE_DATA(page));

    plane_node = MListTail(&page->planeDataPendingPrintList[plane_color]);

    if ( plane_node )
    {
        plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
        ASSERT(IS_PLANED(plane));
        plane->LastData = TRUE;
    }
}

/*
 ** ScanBlocks
 *
 *
 *  PARAMETERS:   PlaneHead Is the linked list plane data to check
                  Clear     This is an enum to specify the function to do.  YES means will clear all.
                                                                            BY_1 will reduce by 1
                                                                            TO_1 will make use count 1
 *
 *  DESCRIPTION:  This routine takes a plane of data and depending on "Clear" does something
 *                to the plane.  If Clear = TO_1 the use count on the block is set to 1. Racing uses this.
 *                if Clear = BY_1 the use count is decremented by 1.  If Clear = YES the block
 *                  is released, ignoring the use counts.  If the use count = 0 then the block
 *                  is released.
 *
 *  RETURNS:
 *
 */


/* FUNCTION NAME: ScanBlocks */

/** 
 * \brief
 * 
 * \param PlaneHead
 * 
 * \param Clear
 * 
 * \return UINT32
 * 
 * \date 1/15/2007
 * 
 **/
UINT32 ScanBlocks(PAGE_DATA *Page, MLISTENTRY *PlaneHead, FORCE_CLEAR Clear)
{
    PLANED *plane;
    ATLISTENTRY* plane_node;

    uint32_t DataDeleted;
    uint32_t unused_use_count;

    DBG_MEMLOG_WARNING("%s ENTRY clear: %d\n", __func__, Clear);
    DataDeleted = 0;     // this is set when something has been removed.
    if (MIsListEmpty(PlaneHead))
    {
        DBG_MEMLOG_WARNING("%s PLANE List Empty!\n", __func__);
        return DataDeleted;
    }

    plane_node = MListHead(PlaneHead);
    ASSERT(plane_node != NULL);

    plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
    ASSERT(plane != NULL);
    ASSERT(IS_PLANED(plane));

    do
    {
        if ((Clear == BY_1) && (plane->UseCount != 0))
        {
            plane->UseCount--;             // reduce the count by 1.
        }

        if (Clear == TO_1)
        {
            plane->UseCount = 1;
        }

        if (Clear == TO_1_IF_UNUSED_CLEAR_IF_USED)
        {
            if (Page->JamRecovery)
            {
                unused_use_count = Page->COPIES * 2; 
            }
            else
            {
                unused_use_count = Page->COPIES;
            }

            if (plane->UseCount == unused_use_count)
            {
                plane->UseCount = 1; 
            }
            else
            {
                plane->UseCount = 0;
            }
        }

        ASSERT(plane_node == &plane->listNode);

        if ((plane->UseCount == 0) || (Clear == YES_CLEAR))    // this block should be freed
        {
            plane_node = MListNext(PlaneHead, &plane->listNode);
            MRemoveEntryList(PlaneHead, &plane->listNode);

            DataDeleted = 1;        // Data has been removed from this plane.  
                                    // Used to decide if a page can be restarted.

            if (plane->big_buffer != NULL)
            {
                DBG_MEMLOG_WARNING("%s freeing 0x%x\n", __func__, plane->big_buffer);

                BigBuffer_Free(plane->big_buffer);
                plane->big_buffer = NULL;
            }
            else
            {
                DBG_MEMLOG_WARNING("%s plane big buffer ptr is NULL\n", __func__);
            }

            DBG_ZEROMEM(plane, sizeof(PLANED)); // guard against reference to plane after free
            MEM_FREE_AND_NULL( plane );         // get rid of the plane data
        }
        else
        {
            plane_node = MListNext(PlaneHead, &plane->listNode);
        }

        if (plane_node == NULL)                 // if null we are done.
        {
            DBG_MEMLOG_WARNING("%s EXIT\n", __func__);
            return DataDeleted; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< EARLY RETURN
        }
        plane = (PLANED *) CONTAINING_RECORD(plane_node, PLANED, listNode);    // get the plane from the plane list.
        ASSERT(plane != NULL);
        ASSERT(IS_PLANED(plane));
        // check to see if we are done scanning; currently all clear types require
        // the full list of blocks to be scanned, including "IF_USECOUNT_IS_ZERO"
        // because zx does out of order retiring of data blocks
    } while ((Clear == YES_CLEAR) ||
             (Clear == BY_1) ||
             (Clear == TO_1) ||
             (Clear == CLEAR_IF_USECOUNT_IS_ZERO) ||
             (Clear == TO_1_IF_UNUSED_CLEAR_IF_USED)); 


    DBG_MEMLOG_WARNING("%s EXIT\n", __func__);
    return DataDeleted;
}


/* FUNCTION NAME: FreeRasterFromPage */
/**
 * \brief Free raster data from a page, as appropriate.
 *
 *  \param TempPage (PAGE_DATA*) Pointer to the page whose planes we are to check.
 *  \param Clear (FORCE_CLEAR) Enum to specify the function to do.
 *                      YES means will clear all.
 *                      BY_1 will reduce use count by 1.
 *                      TO_1 will make use count 1.
 * 
 *  This routine takes a plane of data and depending on "Clear" does something
 *  to the plane.
 *  - if (Clear == TO_1) the use count on the block is set to 1. Racing uses this.
 *  - if (Clear == BY_1) the use count is decremented by 1.
 *  - if (Clear == YES) the block is released, ignoring the use counts.
 *  - if (the use count == 0) then the block is released.
 *
 **/
UINT32 FreeRasterFromPage(PAGE_DATA * pPage, FORCE_CLEAR Clear)
{
    UINT32 retVal=0;
    int i;

    ASSERT(IS_PAGE_DATA(pPage));

    static PAGE_DATA* pLastPage = NULL;

    if(pPage->CanDeleteData || pPage->Racing || Clear != CLEAR_IF_USECOUNT_IS_ZERO)
    {
        for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
        {
            retVal |= ScanBlocks(pPage, &pPage->planeDataPendingPrintList[i], Clear);   
        }
    }

    if(retVal)
    {
        if(pLastPage != pPage)
        {
            pLastPage = pPage;
        }
        DBG_MEMLOG_WARNING("*** DataDeleted PG %#x ***\n", pPage);
        DBG_PRINTF_NOTICE("*** DataDeleted PG %#x ***\n", pPage);
    }

    return retVal;
}

/*
 ** GetPageHeader
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: This is a constructor for the print header.  It allocates the header
                and initializes it.
 *
 *  RETURNS:
 *
 */
PAGE_DATA *GetPageHeader(mlimiter_t* limiter)
{
    PAGE_DATA *Page;
    print_mode_t * print_mode;
    UINT32 i;

    Page = (PAGE_DATA *) MEM_MALLOC_LIMITED(limiter, sizeof(PAGE_DATA));
    memset(Page, 0, sizeof(PAGE_DATA));

    print_mode = (print_mode_t *)MEM_MALLOC_LIMITED(limiter, print_mode_get_sizeof());
    memset(print_mode, 0, print_mode_get_sizeof());

    Page->print_mode = print_mode;

    SET_PAGE_DATA_COOKIE(Page);

    // Initialze the planes "pending print" and "printed" lists.
    for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
    {
        MInitList(&Page->planeDataPendingPrintList[i]);
        Page->numPlaneBuffers[i] = 0;
    }

    Page->ManDup2ndPg = 0;
    Page->engOutTray = TRAY_1;
    Page->DEFAULTSOURCE = TRAY_ANY;
    Page->COPIES = 1;
    Page->PAPER = MEDIASIZE_ANY;
    Page->mediaType = MEDIATYPE_ANY;
    Page->manual_swaths = NULL;
#if defined(PIXELCNT_BYHOST) || defined(PIXELCNT_BYFW)
    Page->swPixCountPtr = NULL;
#endif
    return Page;
}

void SetRacingMode(PAGE_DATA *TempPage, UINT32 Mode)
{
    DBG_MEMLOG_WARNING("RACING %s!!!\n", (Mode ? "ON" : "OFF"));
    DBG_PRINTF_WARNING("RACING %s!!!\n", (Mode ? "ON" : "OFF"));
    ASSERT(IS_PAGE_DATA(TempPage));

    TempPage->Racing = Mode;
}

UINT32 ReturnRacingMode(PAGE_DATA *TempPage)
{
    ASSERT(IS_PAGE_DATA(TempPage));

    return TempPage->Racing;
}


/**
 ** UpdateCnts
 *
 *  PARAMETERS: \param plane_data_list    A pointer to the plane data list.
 *              \param NewCnt The value to update the use counts.
 *
 * \brief Change use counts on a given plane to NewCnt.
 *  DESCRIPTION: Change all the use counts to the value that is passed in.
 *               This allows a restart of a print job.
 *          Output Plane Data use counts updated.
 *
 *  RETURNS: Nothing.
 *
 */
void UpdateCnts(MLISTENTRY *plane_data_list, UINT32 NewCnt, ALLOW_ZERO isAllowed)
{
    PLANED* plane;
    ATLISTENTRY* plane_node;

    // Make sure the value is valid, in some cases we do not allow 0
    if ( (ZERO_IS_NOT_ALLOWED == isAllowed) && (NewCnt == 0) )
    {
        NewCnt = 1;
    }

    plane_node = MListHead(plane_data_list);
    while(plane_node != NULL)
    {
        plane = CONTAINING_RECORD(plane_node, PLANED, listNode);
        ASSERT(IS_PLANED(plane));
        plane->UseCount = NewCnt;
        plane_node = MListNext(plane_data_list, plane_node);
    }
}


/* FUNCTION NAME: UpdatePlanes */

/** 
 * \brief: update plane data after a page had been cancelled to see if
 *         page can be reprinted or not
 * 
 * \param pPage
 * 
 * \return void
 * 
 * \date 10/28/2005
 * 
 **/
void UpdatePlanes(PAGE_DATA* PageHead)
{
    int i;
    ASSERT(IS_PAGE_DATA(PageHead));

    // The page has to have its use counts updated.  Get the use count from the last block and
    // use that for all the others.
    for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
    {
        UpdateCnts(&PageHead->planeDataPendingPrintList[i], PageHead->CurrentUseCount, ZERO_IS_NOT_ALLOWED);
    }
}

#if defined( HAVE_DBG_PRINTF )

//------------------------------------------------------------------------------
// FUNCTION NAME: PrintPagePlaneInfo
/**
 * \brief Print information about a page's plane data.
 *
 * \param pPage (PAGE_DATA*) Pointer to page whose information we are to print.
 *
 * \return void
 *
 **/
void PrintPagePlaneInfo( PAGE_DATA* pPage )
{
    ATLISTENTRY* plane_node;
    int i;

    DBG_PRINTF_ERR("PrintPagePlaneInfo(0x%08X)\n", pPage );
    /**
     * Iterate through all the colors.
     */
    for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
    {
        plane_node = MListHead(&pPage->planeDataPendingPrintList[i]);

        if ( plane_node )
        {
            do
            {
                PLANED* pPlane = CONTAINING_RECORD(plane_node, PLANED, listNode);
                ASSERT(pPlane != NULL);

                PrintPlaneInfo( pPlane );

                plane_node = MListNext(&pPage->planeDataPendingPrintList[i], plane_node);
            } while ( plane_node );
        }
        else // ( plane_node == NULL )
        {
            DBG_PRINTF_ERR("LPT:   Plane list empty.\n");
        }
    }
} // end PrintPagePlaneInfo()


//------------------------------------------------------------------------------
// FUNCTION NAME: PrintPlaneInfo
void PrintPlaneInfo( PLANED* pPlane )
{
    DBG_PRINTF_ERR
        ("LPT:   ----- (PLANED*)pPlane=0x%08X -----\n", pPlane );

    if ( !pPlane )
        return;
    DBG_PRINTF_ERR
        ("LPT:   BlockDataType=%d=%s, RESOLUTION_X=%d, RESOLUTION_Y=%d\n",
         pPlane->BlockDataType, to_string_plane_type(pPlane->BlockDataType),
         pPlane->image_info.image_dpi.x, pPlane->image_info.image_dpi.y );
    DBG_PRINTF_ERR
        ("LPT: width_pixels = 0x%X = %d, height_lines = 0x%X = %d\n",
         pPlane->image_info.image_width_in_pixels, pPlane->image_info.image_width_in_pixels, 
         pPlane->image_info.image_height_in_lines, pPlane->image_info.image_height_in_lines );
    DBG_PRINTF_ERR
        ("LPT:  sysPlaneColor=%d, DataLength=0x%X=%d, LastData=%d\n",
         pPlane->sysPlaneColor, 
         pPlane->DataLength, pPlane->DataLength, pPlane->LastData );
    if ( pPlane->big_buffer != NULL )
    {
    DBG_PRINTF_ERR
                ("LPT:   UseCount=%d, DUPLEX=0x%04X, Data=0x%08X\n",
                 pPlane->UseCount, pPlane->DUPLEX, pPlane->big_buffer->data );
    }
    else
    {
    DBG_PRINTF_ERR
                ("LPT:   UseCount=%d, DUPLEX=0x%04X, Data=NULL (empty buffer)\n",
                 pPlane->UseCount, pPlane->DUPLEX );
    }
    /*
    DBG_PRINTF_ERR
        ("LPT:   Xd=0x%X=%d, Yd=0x%X=%d, L0=0x%X=%d, optionFlags=0x%02X\n",
         pPlane->Xd, pPlane->Xd, pPlane->Yd, pPlane->Yd,
         pPlane->L0, pPlane->L0, pPlane->optionFlags ); */ // REVISIT DAB
} // end PrintPlaneInfo()


//------------------------------------------------------------------------------
/** 
 * \brief Return a pointer to a string with the symbolic name of the given enum value.
 * 
 * \param planeType (PLANE_TYPE)
 * 
 * \return const char*
 * 
 **/
const char* to_string_plane_type( PLANE_TYPE planeType )
{
    switch ( planeType )
    {
        CASE_ENUM_RETURN_STRING( PACKED_RGB );
        CASE_ENUM_RETURN_STRING( PACKED_RGBX );
        CASE_ENUM_RETURN_STRING( PACKED_XRGB );
        CASE_ENUM_RETURN_STRING( GRAYSCALE );
        CASE_ENUM_RETURN_STRING( PLANAR_C);
        CASE_ENUM_RETURN_STRING( PLANAR_Y );
        CASE_ENUM_RETURN_STRING( PLANAR_M );
        CASE_ENUM_RETURN_STRING( PLANAR_K );
//        CASE_ENUM_RETURN_STRING( DELTA_HYBRID );
        CASE_ENUM_RETURN_STRING( DITHER );
        CASE_ENUM_RETURN_STRING( DITHERPARAMS );
        CASE_ENUM_RETURN_STRING( PHOTO_LAYOUT );
        DEFAULT_CASE_ENUM_RETURN_STRING( PLANE_TYPE );
    } // end switch ( planeType )
    return "bug";
} 


const char* to_string_document_type( DOCTYPE doc_type)
{

    switch ( doc_type )
    {
        CASE_ENUM_RETURN_STRING( e_NONE ); 
        CASE_ENUM_RETURN_STRING( e_DOCUMENT ); 
        CASE_ENUM_RETURN_STRING( e_ZJS_DOCUMENT ); 
       // CASE_ENUM_RETURN_STRING( e_DEMOPAGE ); 
       // CASE_ENUM_RETURN_STRING( e_MENUPAGE ); 
        CASE_ENUM_RETURN_STRING( e_CALPAGE ); 
        CASE_ENUM_RETURN_STRING( e_ALIGNMENTPAGE );
        CASE_ENUM_RETURN_STRING( e_CLEANINGPAGE );
        CASE_ENUM_RETURN_STRING( e_COPYPAGE );
        CASE_ENUM_RETURN_STRING( e_ERRORREPORT ); 
        CASE_ENUM_RETURN_STRING( e_SCANCALPAGE );
        CASE_ENUM_RETURN_STRING( e_FAX );
        CASE_ENUM_RETURN_STRING( e_PQPAGE );
        CASE_ENUM_RETURN_STRING( e_WIRELESSTEST );
        CASE_ENUM_RETURN_STRING( e_DIAGNOSTIC );
        CASE_ENUM_RETURN_STRING( e_PHOTOLAYOUT );
        CASE_ENUM_RETURN_STRING( e_DDPAGE );

        DEFAULT_CASE_ENUM_RETURN_STRING( DOCTYPE );
    }
    
    return "bug";
}

const char* to_string_print_quality( PRINT_QUALITY print_quality )
{
    switch ( print_quality )
    {
        CASE_ENUM_RETURN_STRING( PRINTQUALITY_NORMAL );
        CASE_ENUM_RETURN_STRING( PRINTQUALITY_DRAFT );
        CASE_ENUM_RETURN_STRING( PRINTQUALITY_BETTER );
        CASE_ENUM_RETURN_STRING( PRINTQUALITY_BEST );
        DEFAULT_CASE_ENUM_RETURN_STRING( PRINT_QUALITY );
    } 
    return "bug";
} 

#endif // defined( DEBUG )

/*
 ** Function Name: 
 *
 * PARAMETERS:
 *
 * DESCRIPTION: Will set the page instance counters all to the same value 
 *              as the top instance counter.
 *
 * RETURNS:
 *
 */
void SanitizeUseCounts(PAGE_DATA *PageHead)
{
    int i;
    // Make sure the use counts on all planes match the page use count.
    //UpdatePlanesVerifyPage(TempPage);
    for (i=0; i<NUM_MARKING_COLOR_CHANNELS; i++) 
    {
        UpdateCnts(&PageHead->planeDataPendingPrintList[i], PageHead->CurrentUseCount, ZERO_IS_NOT_ALLOWED);
    }
    FreeRasterFromPage(PageHead, CLEAR_IF_USECOUNT_IS_ZERO);

    return; 
}


