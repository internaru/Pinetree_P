/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file engPaperHandling.c
 *
 * \brief This file contains source code for paper-handling logic that is
 *        common to all print engines and mechs.
 **/

#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>
#include "ATypes.h"
#include "lassert.h"

#include "print_job_types.h"
#include "agprint.h"
#include "engPaperHandlingAPI.h"
#include "logger.h"
#include "printvars_api.h"


//------------------------------------------------------------------------------
// File '#define's
//------------------------------------------------------------------------------

#define ENGDBG_ERROR (DBG_ERROR|DBG_OUTPUT)
#define ENGDBG_LEVEL (DBG_SOFT|DBG_OUTPUT)
#define ENGDBG_LOUD  (DBG_LOUD|DBG_OUTPUT)


//------------------------------------------------------------------------------
// File Global Variables
//------------------------------------------------------------------------------

bool gPrinterHasPaperSensor = true;


//------------------------------------------------------------------------------
// Static Forward Declarations
//------------------------------------------------------------------------------

static void     UpdatePageCountsCommon(const PAGE_DATA* pPage);
static void     UpdatePagePixelCountsCommon(const PAGE_DATA* pPage);


//------------------------------------------------------------------------------
// { Begin Object to convert between physical input tray number and trayenum_t.
//------------------------------------------------------------------------------

// Initialize these arrays in EngPlatGetPrinterSetup().
static trayenum_t PhysicalTrayNumberToTRAYENUM[NUM_INPUTS+1]; // Indexed by one-relative int 1..N.
static int      TRAYENUMToPhysicalTrayNumber[NUM_INPUTS];   // Indexed by trayenum_t; TRAY_1 = 0.

//------------------------------------------------------------------------------
// FUNCTION NAME: ConstructPhysicalTrayNumberToTRAYENUMObject
/**
 * \brief Construct the lookup tables for EngPhysicalTrayNumberToTRAYENUM() and
 *        EngTRAYENUMToPhysicalTrayNumber() using the settings of
 *        INPUTPROPERTY_PHYSICAL_TRAY_<N> in printer_setup_t.
 **/
void ConstructPhysicalTrayNumberToTRAYENUMObject(const printer_setup_t* pPrinterSetup)
{
    int         oneRelativePhysicalTrayNumber;
    trayenum_t    trayEnum;

    // Fill in or scrub lookup table used by EngTRAYENUMToPhysicalTrayNumber()
    // by making one pass through printer_setup_t.
    for (trayEnum=TRAY_1; trayEnum<NUM_INPUTS; trayEnum++)
    {

        TRAYENUMToPhysicalTrayNumber[trayEnum] = trayEnum;
    } // end for (trayEnum)


    // Scrub lookup table used by EngPhysicalTrayNumberToTRAYENUM().
    for (oneRelativePhysicalTrayNumber=0;
         oneRelativePhysicalTrayNumber<NUM_INPUTS+1;
         oneRelativePhysicalTrayNumber++)
    {
        PhysicalTrayNumberToTRAYENUM[oneRelativePhysicalTrayNumber] = TRAY_NONE;
    }

    // Fill in lookup table used by EngPhysicalTrayNumberToTRAYENUM() by making
    // one pass through the values already stored in the other lookup table.
    //
    // If multiple trayenum_t values correspond to the same physical tray number,
    // then we store the highest trayenum_t value in this table, as desired.
    for (trayEnum=TRAY_1; trayEnum<NUM_INPUTS; trayEnum++)
    {
        if (TRAYENUMToPhysicalTrayNumber[trayEnum] != -1)
        {
            PhysicalTrayNumberToTRAYENUM[TRAYENUMToPhysicalTrayNumber[trayEnum]] = trayEnum;
        }
    } // end for (trayEnum)

} // end ConstructPhysicalTrayNumberToTRAYENUMObject()

//------------------------------------------------------------------------------
// FUNCTION NAME: EngPhysicalTrayNumberToTRAYENUM
/**
 * \brief Convert a one-relative physical tray number of a user-accessible
 *        input tray to the corresponding trayenum_t.
 *
 * \param oneRelativePhysicalTrayNumber (int) one-relative physical tray number
 *        of a user-accessible input tray.
 *
 * \return trayenum_t enum value corresponding to the user-accessible input
 *         oneRelativePhysicalTrayNumber.
 *         If there is no corresponding tray, return the value TRAY_NONE.
 *
 * Typical products have two trayenum_t values corresponding to physical tray
 * number one.
 * - TRAY_1 is typically the ManualFeedTray, and
 * - TRAY_2 is typically the MultiPurposeTray.
 * - For such products, for input value 1, this function returns TRAY_2.
 *
 * Implemented using a lookup table initialized in EngPlatGetPrinterSetup().
 * Alternative is to access e_PrinterSetup in DataStore, which is slower and
 * raises the possibility of deadlocks.
 *
 * See also:
 * - int EngTRAYENUMToPhysicalTrayNumber(trayenum_t inputTRAYENUM)
 * - trayenum_t JobMgrTranslateTray(INPUTTYPE input)
 **/
trayenum_t EngPhysicalTrayNumberToTRAYENUM(int oneRelativePhysicalTrayNumber)
{
    trayenum_t returnTRAYENUM = TRAY_NONE;

    if ( (oneRelativePhysicalTrayNumber >= 1) &&
         (oneRelativePhysicalTrayNumber <= NUM_INPUTS) )
    {
        returnTRAYENUM = PhysicalTrayNumberToTRAYENUM[oneRelativePhysicalTrayNumber];
    }

    //DPRINTF(ENGDBG_LEVEL, ("ENG: EngPhysicalTrayNumberToTRAYENUM(%d)=%d\n", oneRelativePhysicalTrayNumber, returnTRAYENUM));
    return returnTRAYENUM;
} // end EngPhysicalTrayNumberToTRAYENUM()

//------------------------------------------------------------------------------
// FUNCTION NAME: EngTRAYENUMToPhysicalTrayNumber
/**
 * \brief Convert a trayenum_t value to the corresponding one-relative physical
 *        tray number of a user-accessible input tray.
 *
 * \param input_tray_enum (trayenum_t) enum value to look up.
 *
 * \return int one-relative physical tray number of a user-accessible input tray
 *         corresponding to input_tray_enum.
 *         If there is no corresponding tray, return the value -1.
 *
 **/
int EngTRAYENUMToPhysicalTrayNumber(trayenum_t input_tray_enum )
{
    int physical_tray_number;

    /* NUM_INPUTS is size of array - TRAYENUMToPhysicalTrayNumber */
    ASSERT( input_tray_enum < NUM_INPUTS );
    physical_tray_number = TRAYENUMToPhysicalTrayNumber[input_tray_enum];

    //DPRINTF(ENGDBG_LEVEL, ("ENG: EngTRAYENUMToPhysicalTrayNumber(%d)=%d\n", input_tray_enum, physical_tray_number));
    return physical_tray_number;
} // end EngTRAYENUMToPhysicalTrayNumber()

//------------------------------------------------------------------------------
// FUNCTION NAME: eng_get_have_more_than_one_physical_input_tray
/**
 * \brief Tells whether the print engine has more than one user-accessible
 *        physical input tray.
 */
bool eng_get_have_more_than_one_physical_input_tray( void )
{
	UINT16 i;
	UINT16 physicalTrayCount = 0;
	printer_setup_t  printerSetup;

	printvar_get_printersetup(&printerSetup);

	for( i = 0; i < NUM_INPUTS; i++ )
	{
		if( printerSetup.inputTrays[i].properties && INPUTPROPERTY_PHYSICAL_TRAY_MASK )
		{
			physicalTrayCount++;
		}
	}

	if( physicalTrayCount > 1 )
	{
		DPRINTF(ENGDBG_LEVEL,("ENG: MULTIPLE INPUT TRAYS! %d\n",physicalTrayCount));
	}
	return ( physicalTrayCount > 1 );

	} // end eng_get_have_more_than_one_physical_input_tray()

//------------------------------------------------------------------------------
// FUNCTION NAME: eng_get_have_only_one_physical_input_tray
/**
 * \brief Tells whether the print engine has just one user-accessible physical
 *        input tray.
 */
bool eng_get_have_only_one_physical_input_tray( void )
{
	UINT16 i;
	UINT16 physicalTrayCount = 0;
	printer_setup_t printerSetup;

	printvar_get_printersetup(&printerSetup);


	for( i = 0; i < NUM_INPUTS; i++ )
	{
		if( printerSetup.inputTrays[i].properties && INPUTPROPERTY_PHYSICAL_TRAY_MASK )
		{
			physicalTrayCount++;
		}

	}
	if( physicalTrayCount == 1 )
	{
		DPRINTF(ENGDBG_LEVEL,("ENG: Physical Trays = %d!\n", physicalTrayCount));
	}
	return ( physicalTrayCount == 1 );
	
} // end eng_get_have_only_one_physical_input_tray()

//------------------------------------------------------------------------------
// } End   Object to convert between physical input tray number and TRAYENUM.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// { BEGIN OBJECT: TRAYENUM_TRAY_1_TRAY_2_Coincide
//------------------------------------------------------------------------------

static bool gEngTRAYENUM_TRAY_1_TRAY_2_Coincide = false;

bool EngGetTRAYENUM_TRAY_1_TRAY_2_Coincide(void)
{
    return gEngTRAYENUM_TRAY_1_TRAY_2_Coincide;
} // end EngGetTRAYENUM_TRAY_1_TRAY_2_Coincide()

void EngSetTRAYENUM_TRAY_1_TRAY_2_Coincide(bool newValue)
{
    gEngTRAYENUM_TRAY_1_TRAY_2_Coincide = newValue;
} // end EngSetTRAYENUM_TRAY_1_TRAY_2_Coincide()

//------------------------------------------------------------------------------
// } END   OBJECT: TRAYENUM_TRAY_1_TRAY_2_Coincide
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// FUNCTION NAME: EngMEDIATYPEIsSymmetric
/**
 * \brief true if input mediatype_t is symmetric; false if not.
 *
 * \param inputMEDIATYPE (mediatype_t) Media type in question.
 * 
 * \return bool Symmetry of inputMEDIATYPE.
 * \retval true inputMEDIATYPE is symmetric.
 * \retval false inputMEDIATYPE is asymmetric.
 *
 * At this time, we assume GLOSSY media types are symmetric, i.e., they have
 * equally glossy surfaces on both sides of the sheet.
 **/
bool EngMEDIATYPEIsSymmetric(mediatype_t inputMEDIATYPE)
{
    bool retVal = true;

    switch (inputMEDIATYPE)
    {
    case MEDIATYPE_LABEL:
    case MEDIATYPE_ENVELOPE:
    case MEDIATYPE_ENVELOPE_10:
    case MEDIATYPE_HEAVY_ENVELOPE:
    case MEDIATYPE_LETTERHEAD:
    case MEDIATYPE_PREPRINTED:
    case MEDIATYPE_PREPUNCHED:
        retVal = false;
        break;

    default:
        break;
    } // end switch (inputMEDIATYPE)

    return retVal;
} // end EngMEDIATYPEIsSymmetric()


//------------------------------------------------------------------------------
// FUNCTION NAME: EngPaperPresentInSomeInputTray
bool EngPaperPresentInSomeInputTray(void)
{
    return true;
}
//------------------------------------------------------------------------------


#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

//------------------------------------------------------------------------------
// FUNCTION NAME: EngGetAutoDuplexerHasMedia
/**
 **/
bool EngGetAutoDuplexerHasMedia(void)
{
   ASSERT(0); // must provide real implementation of this routine 
   return false;

} // end EngGetAutoDuplexerHasMedia()


//------------------------------------------------------------------------------
// { BEGIN OBJECT: AutoDuplexerInputTRAYENUM
//------------------------------------------------------------------------------

static trayenum_t gEngAutoDuplexerInputTRAYENUM = TRAY_3;

//----------------------------------------------------------
// FUNCTION NAME: EngGetAutoDuplexerInputTRAYENUM
/**
 * \brief Return trayenum_t value for duplexer as an input tray.
 *        TRAY_NONE if no automatic duplexer is present.
 *
 * \return trayenum_t value for duplexer as an input tray.
 *
 * \author Paul Rolig 2007/06/18
 **/
trayenum_t EngGetAutoDuplexerInputTRAYENUM(void)
{
    return gEngAutoDuplexerInputTRAYENUM;
} // end EngGetAutoDuplexerInputTRAYENUM()

//----------------------------------------------------------
// FUNCTION NAME: EngSetAutoDuplexerInputTRAYENUM
/**
 * \brief Set the trayenum_t value for duplexer as an input tray.
 *
 * \return void
 * \param newValue (trayenum_t) Value to set.
 *
 * \author Paul Rolig 2007/06/18
 **/
void EngSetAutoDuplexerInputTRAYENUM(trayenum_t newValue)
{
    gEngAutoDuplexerInputTRAYENUM = newValue;
} // end EngSetAutoDuplexerInputTRAYENUM()

//------------------------------------------------------------------------------
// } END   OBJECT: AutoDuplexerInputTRAYENUM
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// { BEGIN OBJECT: AutoDuplexerOutputTRAYENUM
//------------------------------------------------------------------------------

static trayenum_t gEngAutoDuplexerOutputTRAYENUM = TRAY_3;

//----------------------------------------------------------
// FUNCTION NAME: EngGetAutoDuplexerOutputTRAYENUM
/**
 * \brief Return trayenum_t value for duplexer as an output tray/bin.
 *        TRAY_NONE if no automatic duplexer is present.
 *
 * \return trayenum_t value for duplexer as an output tray/bin.
 *
 * \author Paul Rolig 2007/06/18
 **/
trayenum_t EngGetAutoDuplexerOutputTRAYENUM(void)
{
    return gEngAutoDuplexerOutputTRAYENUM;
} // end EngGetAutoDuplexerOutputTRAYENUM()

//----------------------------------------------------------
// FUNCTION NAME: EngSetAutoDuplexerOutputTRAYENUM
/**
 * \brief Set the trayenum_t value for duplexer as an output tray/bin.
 *
 * \return void
 * \param newValue (trayenum_t) Value to set.
 *
 * \author Paul Rolig 2007/06/18
 **/
void EngSetAutoDuplexerOutputTRAYENUM(trayenum_t newValue)
{
    gEngAutoDuplexerOutputTRAYENUM = newValue;
} // end EngSetAutoDuplexerOutputTRAYENUM()

//------------------------------------------------------------------------------
// } END   OBJECT: AutoDuplexerOutputTRAYENUM
//------------------------------------------------------------------------------

#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)


//------------------------------------------------------------------------------
// { BEGIN OBJECT: HaveAutoDuplexer
//------------------------------------------------------------------------------

static bool gEngHaveAutoDuplexer = true;

//----------------------------------------------------------
// FUNCTION NAME: EngGetHaveAutoDuplexer
/**
 * \brief Return true if and only if there is an automatic duplexer installed.
 *
 * \return bool Truth value of automatic duplexer's presence.
 *
 * \author Paul Rolig 2007/05/31
 **/
bool EngGetHaveAutoDuplexer(void)
{
    return gEngHaveAutoDuplexer;
} // end EngGetHaveAutoDuplexer()

/// true if the engine want the front side of a duplex first.
///
bool EngDuplexerFrontSideFirst()
{
	/// normally lasers send back side first
	/// return false;
	///
        /// Some printers want the front side first.
	return true;
}
//----------------------------------------------------------
// FUNCTION NAME: EngSetHaveAutoDuplexer
/**
 * \brief Set the value of whether or not there is an automatic duplexer installed.
 *
 * \return void
 * \param newValue (bool) Value to set.
 *
 * \author Paul Rolig 2007/05/31
 **/
void EngSetHaveAutoDuplexer(bool newValue)
{
    gEngHaveAutoDuplexer = newValue;
} // end EngSetHaveAutoDuplexer()

//------------------------------------------------------------------------------
// } END   OBJECT: HaveAutoDuplexer
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// { BEGIN OBJECT: HaveBinFullSensor
//------------------------------------------------------------------------------

static bool gEngHaveBinFullSensor = true;

//----------------------------------------------------------
// FUNCTION NAME: EngGetHaveBinFullSensor
/**
 * \brief Return true if and only if there is a bin full sensor installed.
 *
 * \return bool Truth value of bin full sensor's presence.
 *
 * \author Paul Rolig 2008/02/13
 **/
bool EngGetHaveBinFullSensor(void)
{
    return gEngHaveBinFullSensor;
} // end EngGetHaveBinFullSensor()

//----------------------------------------------------------
// FUNCTION NAME: EngSetHaveBinFullSensor
/**
 * \brief Set the value of whether or not there is a bin full sensor installed.
 *
 * \return void
 * \param newValue (bool) Value to set.
 *
 * \author Paul Rolig 2008/02/13
 **/
void EngSetHaveBinFullSensor(bool newValue)
{
    gEngHaveBinFullSensor = newValue;
} // end EngSetHaveBinFullSensor()

//------------------------------------------------------------------------------
// } END   OBJECT: HaveBinFullSensor
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// { BEGIN OBJECT: FaceDownOutputTRAYENUM
//------------------------------------------------------------------------------

/**
 * \brief Initialized to TRAY_1 by default, because all our products so far have
 *        had a FaceDown output tray/bin designated by this trayenum_t value.
 *
 * If some future product has no such bin, then its setup code must call
 * EngSetFaceDownOutputTRAYENUM(TRAY_NONE).
 **/
static trayenum_t gEngFaceDownOutputTRAYENUM = TRAY_1;

//----------------------------------------------------------
// FUNCTION NAME: EngGetFaceDownOutputTRAYENUM
/**
 * \brief Return trayenum_t value for face down output tray/bin.
 *        TRAY_NONE if no such tray/bin is present.
 *
 * \return trayenum_t value for face down output tray/bin.
 *
 * \author Paul Rolig 2008/04/28
 **/
trayenum_t EngGetFaceDownOutputTRAYENUM(void)
{
    return gEngFaceDownOutputTRAYENUM;
} // end EngGetFaceDownOutputTRAYENUM()

//----------------------------------------------------------
// FUNCTION NAME: EngSetFaceDownOutputTRAYENUM
/**
 * \brief Set the trayenum_t value for face down output tray/bin.
 *
 * \return void
 * \param newValue (trayenum_t) Value to set.
 *
 * \author Paul Rolig 2008/04/28
 **/
void EngSetFaceDownOutputTRAYENUM(trayenum_t newValue)
{
    gEngFaceDownOutputTRAYENUM = newValue;
} // end EngSetFaceDownOutputTRAYENUM()

//------------------------------------------------------------------------------
// } END   OBJECT: FaceDownOutputTRAYENUM
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// FUNCTION NAME: EngMediaIsDuplexable
/**
 * \brief Determines whether the given MediaSize and MediaType are duplexable,
 *        not only on account of the media itself, but also considering
 *        whether there even is a duplexer on this device, size fits in the
 *        duplexer, etc.
 * 
 * \param inMediaSize (mediasize_t)
 * \param inMediaType (mediatype_t)
 * 
 * \return bool true if all conditions say "Go!" and false if there is any
 *         reason at all that this media cannot be duplexed at this time.
 */
bool EngMediaIsDuplexable( mediasize_t inMediaSize, 
                           mediatype_t inMediaType )
{
#if defined( HAVE_AUTOMATIC_DUPLEXER_SUPPORT )
    bool mediaIsDuplexable = EngGetHaveAutoDuplexer();

    //DPRINTF(ENGDBG_LEVEL, ("EngMediaIsDuplexable on account of EngGetHaveAutoDuplexer: %s\n",
    //                       ToStringCond( mediaIsDuplexable ) ));
# if defined( e_ManualDuplex )
    /**
     * Don't AutoDuplex if the user is trying to ManualDuplex!
     */
    {
        uint32_t val;

        printGetVarManualDuplex(&val);

        if ( mediaIsDuplexable )
            if (val )
                mediaIsDuplexable = false;
    }

    //DPRINTF(ENGDBG_LEVEL, ("EngMediaIsDuplexable on account of ManualDuplex: %s\n",
    //                       ToStringCond( mediaIsDuplexable ) ));
# endif // defined( e_ManualDuplex )

    if ( mediaIsDuplexable )
    {
        mediaIsDuplexable = EngTraySupportsMEDIASIZE( EngGetAutoDuplexerInputTRAYENUM(), inMediaSize );

        /**
         * Ignore custom width and height. So far none of our products can
         * duplex custom media.
         */

        //DPRINTF(ENGDBG_LEVEL, ("EngMediaIsDuplexable on account of mediasize_t: %s\n",
        //                       ToStringCond( mediaIsDuplexable ) ));

        if ( mediaIsDuplexable )
        {
            mediaIsDuplexable = EngTraySupportsMEDIATYPE( EngGetAutoDuplexerOutputTRAYENUM(), inMediaType );

            //DPRINTF(ENGDBG_LEVEL, ("EngMediaIsDuplexable on account of mediatype_t: %s\n",
            //                       ToStringCond( mediaIsDuplexable ) ));
        } // end if ( mediaIsDuplexable )

	//if ( !( inMediaSize == A4 || inMediaSize == LETTER ))
	//     mediaIsDuplexable = false;
	     // transparency?

    } // end if ( mediaIsDuplexable )

#else  // !defined( HAVE_AUTOMATIC_DUPLEXER_SUPPORT )
    bool mediaIsDuplexable = false;

#endif // !defined( HAVE_AUTOMATIC_DUPLEXER_SUPPORT )

    DPRINTF(ENGDBG_LEVEL, 
            ("EngMediaIsDuplexable(mediasize_t=0x%X=%d, mediatype_t=0x%X=%d): %s\n",
             inMediaSize, inMediaSize,
             inMediaType, inMediaType, ToStringCond( mediaIsDuplexable ) ));
    return mediaIsDuplexable;
} // end EngMediaIsDuplexable()



//------------------------------------------------------------------------------
// FUNCTION NAME: EngTraySupportsMEDIASIZE
/**
 * \brief Look up whether or not the given tray supports the given mediasize_t.
 *
 * \param inputTRAYENUM (trayenum_t) Tray in question. Caller may specify TRAY_ANY
 *        to find out if there is any tray at all that supports it.
 *
 * \param inputMEDIASIZE Media size in question.
 *
 * \return bool true if supported; false if not.
 **/
bool EngTraySupportsMEDIASIZE(trayenum_t inputTRAYENUM, mediasize_t inputMEDIASIZE)
{

    return true;
} // end EngTraySupportsMEDIASIZE()


//------------------------------------------------------------------------------
// FUNCTION NAME: EngTraySupportsMEDIATYPE
/**
 * \brief Look up whether or not the given tray supports the given mediatype_t.
 *
 * \param inputTRAYENUM (trayenum_t) Tray in question. Caller may specify TRAY_ANY
 *        to find out if there is any tray at all that supports it.
 *
 * \param inputMEDIATYPE Media type in question.
 *
 * \return bool true if supported; false if not.
 **/
bool EngTraySupportsMEDIATYPE(trayenum_t inputTRAYENUM, mediatype_t inputMEDIATYPE)
{
    return true;
} // end EngTraySupportsMEDIATYPE()


//------------------------------------------------------------------------------
// FUNCTION NAME: UpdateStatisticsPageDoneCommon
/** 
 * \brief Update statistics associated with a page getting done printing.
 *        Common for all products.
 * 
 * \param pMsg - (MESSAGE*) Pointer to message as received by the job manager.
 * 
 * \return void
 * 
 * \author: Paul Rolig 2008/07/03
 **/
void UpdateStatisticsPageDoneCommon(const MESSAGE* pMsg)
{
    PAGE_DATA* pPage = (PAGE_DATA*)pMsg->param3;

    ASSERT(IS_PAGE_DATA(pPage));

    /**
     * \note Some kinds of pages are not counted in these statistics.
     *       For example, calibration and cleaning pages are not counted.
     *       We don't count any pages if we are running in Paperless mode.
     */
    if (   (pPage->DocType == e_CALPAGE)
        || (pPage->DocType == e_CLEANINGPAGE))
    {
        DPRINTF(ENGDBG_LEVEL, ("UpdateStatisticsPageDoneCommon: Do not count pPage->DocType=%d.\n", pPage->DocType));
        return;
    }

    switch (pMsg->msgType)
    {
    case MSG_PAGEDONE:          // param3 might point to a front or a back.
	    if (pPage->sheet->FrontPage) {
		    UpdatePageCountsCommon(pPage->sheet->FrontPage);
		    UpdatePagePixelCountsCommon(pPage->sheet->BackPage);
	    }
	    if (pPage->sheet->BackPage) {
		    UpdatePageCountsCommon(pPage->sheet->BackPage);
		    UpdatePagePixelCountsCommon(pPage->sheet->BackPage);
	    }
        break;

    default:
        break;
    } // end switch (pMsg->msgType)
} // end UpdateStatisticsPageDoneCommon()


//------------------------------------------------------------------------------
// FUNCTION NAME: UpdatePageCountsCommon
/** 
 * \brief Update page counts. Common for all products.
 * 
 * \param pPage - (const PAGE_DATA*) Pointer to page to tally.
 * 
 * \return void
 * 
 * \author: Paul Rolig 2008/07/03
 **/
static void UpdatePageCountsCommon(const PAGE_DATA* pPage)
{
 //   PAGECOUNTS  totalPageCounts;

    /**
     * Update overall page counts when any page gets done.
     */
#ifdef TODO  // was COLOR_ENG
    if (pPage->page_output_color_mode == e_Color)
    {
        uint32_t colorIndex;

        for (colorIndex = COLOR_CYAN; colorIndex < NUM_MARKING_COLOR_CHANNELS; colorIndex++)
        {
            totalPageCounts.pageCount[colorIndex]++;
        }
    } // end if (pPage->PageType == e_Color)
#endif // ifdef COLOR_ENG


    /**
     * Update simplex/duplex page counts based on duplex mode of page.
     */

#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

    if (GetAutoDuplex(pPage->DUPLEX))
    {
        /**
         * Update duplex sheet counts only when a front page gets done.
         */
        if (!GetBackSide(pPage->DUPLEX))
        {

        //    tDSVarData  dsSheetCountDuplex;
#if 0
            uint32_t      sheetCountDuplex;
            dsSheetCountDuplex.name = e_PageCountDuplex; //REVISIT TODO: rename to: e_SheetCountDuplex;
            dsSheetCountDuplex.pVar = &sheetCountDuplex;
            DSGetVar(&dsSheetCountDuplex);
            sheetCountDuplex++;
            DSSetVar(&dsSheetCountDuplex);
            DPRINTF(ENGDBG_LEVEL, ("UpdatePageCountsCommon: increment e_SheetCountDuplex\n"));
#endif
        }
    }
    else // Simplex.
#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
    {
    } // end else Simplex.

} // end UpdatePageCountsCommon()


//------------------------------------------------------------------------------
// FUNCTION NAME: UpdatePagePixelCountsCommon
/** 
 * \brief Update pixel counts from a given page. Common for all products.
 * 
 * \param pPage - (const PAGE_DATA*) Pointer to page whose pixels we are to tally.
 * 
 * \return void
 * 
 * \author: Paul Rolig 2008/07/03
 **/
static void UpdatePagePixelCountsCommon(const PAGE_DATA* pPage)
{


} // end UpdatePagePixelCountsCommon()


#if defined(DEBUG)

//------------------------------------------------------------------------------
// FUNCTION NAME: EngPaperHandlingPrintState
/**
 * \brief Callback for debug code to print state information to debug output.
 **/
void EngPaperHandlingPrintState(void)
{
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gPrinterHasPaperSensor=%s\n", ToStringCond(gPrinterHasPaperSensor)));
#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gEngAutoDuplexerInputTRAYENUM=%d\n", gEngAutoDuplexerInputTRAYENUM));
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gEngAutoDuplexerOutputTRAYENUM=%d\n", gEngAutoDuplexerOutputTRAYENUM));
#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gEngHaveAutoDuplexer=%s\n", ToStringCond(gEngHaveAutoDuplexer)));
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gEngHaveBinFullSensor=%s\n", ToStringCond(gEngHaveBinFullSensor)));
    DPRINTF(ENGDBG_LOUD, ("ENG STATE: gEngFaceDownOutputTRAYENUM=%d\n", gEngFaceDownOutputTRAYENUM));

} // end EngPaperHandlingPrintState()

#endif // defined(DEBUG)

//------------------------------------------------------------------------------

