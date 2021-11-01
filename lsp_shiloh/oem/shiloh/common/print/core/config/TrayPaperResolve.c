/*
 *
 * ============================================================================
 * Copyright (c) 2007-2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file TrayPaperResolve.c
 *
 * \brief This file contains helper functions for the print manager and
 * paper trays
 *
 */
 
#include <pthread.h>
#include "ATypes.h"
#include "PrintMgr.h"
#include "TrayPaperResolve.h"
#include "logger.h"
#include "printvars_api.h"
#include "print_job_api.h"
#include "SysPaperDef.h"

/// pm submodule
#define DBG_PRFX "trayresolve: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 13 ) 

/***************************************************************************
 * Media description (for Engine)
 * 
 * bit 0~1 (Texture) : 0 = Smooth / 1 = Normal / 2 = Rough
 * bit 2~3 (Weight)  : 0 = light  / 1 = Normal / 2 = Heavy
 * bit 4~7 (Type)    : 0 = Plain  / 1 = Label  / 2 = Envelope  / 3 = CardStock  
 * 					   4 = Transparency  / 5 =Vinyl label  / 6 = Postcard / 7 = Recycled
 * (DB values should be '-1') : DB >>> 1 = Plain ~ 8 = Recycled
***************************************************************************/
void set_db_tray_media_description(PAGE_DATA* pPage, const mediacnfg_t* pMediaCnfg, trayenum_t etray)
{
	uint8_t dbPaperType;
	uint8_t dbPaperWeight;
	uint8_t dbPaperTexture;
	
	//pPage->ImageOption.darknessLevel = pMediaCnfg->mediaPrintDarkness;
	//pPage->DEFAULTSOURCE = pMediaCnfg->defaultSourceTray;
	
	//dbPaperType = pMediaCnfg->mediaType[pPage->DEFAULTSOURCE];
	dbPaperType = pMediaCnfg->mediaType[etray];
	
	DBG_PRINTF_DEBUG("TrayPaperResolve: PRINT SETTING : Darkness Level (%d), DefSourceTray (%d), MedType (%d)\n",pPage->ImageOption.darknessLevel, pPage->DEFAULTSOURCE, dbPaperType);
	switch(dbPaperType)
	{
		case TYPE_PLAIN:
			dbPaperWeight = pMediaCnfg->paperType.PlainPaper.Weight;
			dbPaperTexture = pMediaCnfg->paperType.PlainPaper.Texture;
			break;
		case TYPE_PAPERLABEL:
			dbPaperWeight = pMediaCnfg->paperType.PaperLabel.Weight;
			dbPaperTexture = pMediaCnfg->paperType.PaperLabel.Texture;
			break;
		case TYPE_ENVELOPE:
			dbPaperWeight = pMediaCnfg->paperType.Envelope.Weight;
			dbPaperTexture = pMediaCnfg->paperType.Envelope.Texture;
			break;
		case TYPE_CARDSTOCK:
			dbPaperWeight = pMediaCnfg->paperType.CardStock.Weight;
			dbPaperTexture = pMediaCnfg->paperType.CardStock.Texture;
			break;
		case TYPE_TRANSPARENCY:
			dbPaperWeight = pMediaCnfg->paperType.TransParency.Weight;
			dbPaperTexture = pMediaCnfg->paperType.TransParency.Texture;
			break;
		case TYPE_VINYLLABEL:
			dbPaperWeight = pMediaCnfg->paperType.VinylLabel.Weight;
			dbPaperTexture = pMediaCnfg->paperType.VinylLabel.Texture;
			break;
		case TYPE_POSTCARD:
			dbPaperWeight = pMediaCnfg->paperType.PostCard.Weight;
			dbPaperTexture = pMediaCnfg->paperType.PostCard.Texture;
			break;
		case TYPE_RECYCLED:
			dbPaperWeight = pMediaCnfg->paperType.Recycled.Weight;
			dbPaperTexture = pMediaCnfg->paperType.Recycled.Texture;
			break;
			
		default:
			dbPaperWeight = pMediaCnfg->paperType.PlainPaper.Weight;
			dbPaperTexture = pMediaCnfg->paperType.PlainPaper.Texture;
			break;
	}
	dbPaperType--;
	dbPaperWeight--;
	dbPaperTexture--;
	
	pPage->MediaDescription = (uint8_t)(dbPaperType << 4)+(uint8_t)(dbPaperWeight << 2)+(uint8_t)(dbPaperTexture);
	DBG_PRINTF_DEBUG("TrayPaperResolve: media_description : 0x%x (%d,%d,%d)\n",pPage->MediaDescription, dbPaperType, dbPaperWeight, dbPaperTexture);
}

void PrintMgrFindBestMatch(const printer_setup_t* pSetup, const mediacnfg_t* pCnfg, 
                           mediasize_t size, mediatype_t type, BESTMATCH* bestMatch, 
                           trayenum_t manualFeedTray, BOOL useAnyAvailable)
{
    //trayenum_t physicalTray1;
    trayenum_t tray;
    trayenum_t starttray;
    const INPUTTRAY* pTray = NULL;
    BOOL foundMatch = FALSE;
    UINT8 iter;
    UINT8 matchType = 0;
    mediasize_t findSize = MEDIASIZE_ANY;
    mediatype_t findType = MEDIATYPE_ANY;

    DBG_PRINTF_DEBUG("PM: PrintMgrFindBestMatch: enter\n");

    bestMatch->matchTypeEmpty = HaveNone;
    bestMatch->matchTypeNonEmpty = HaveNone;
    bestMatch->bestNonEmpty = TRAY_NONE;
    bestMatch->bestEmpty = TRAY_NONE;
    
    BOOL fMediaSizeA4orLT = FALSE;
    BOOL fSizeDoubleCheck = FALSE;

/*
    pTray = &pSetup->inputTrays[0]; // hardcode tray 0 to manual feed 

    if(pTray)
    {
        if(pTray->paperPresent)
        {
	    physicalTray1 = manualFeedTray;
	    
	    DBG_PRINTF_NOTICE("PM: physical tray 1: %d\n", physicalTray1 + 1 );

            //use tray if size and type set to any or if is a priority tray (always print from it
            //if it has paper in it). Don't prompt the user. 
            if((pCnfg->mediaSize[physicalTray1] == MEDIASIZE_ANY && 
                pCnfg->mediaType[physicalTray1] == MEDIATYPE_ANY) || 
               (pTray->properties & INPUTPROPERTY_PRIORITY))
            {
		// In this case we always print no matter what size or type and we don't prompt the user.
		DBG_PRINTF_NOTICE("PM: physical tray 1 has paper and is unconfigured use it without prompting\n");
		bestMatch->bestNonEmpty = physicalTray1;

		bestMatch->matchTypeNonEmpty = HaveSizeHaveType;
                bestMatch->bestEmpty = TRAY_NONE;
                foundMatch = TRUE;
            }
        }
    }
*/
	if(((size == MEDIASIZE_A4) || (size == MEDIASIZE_LETTER)) && (pCnfg->trayMatchOptionA4LT==1))
	{
		fMediaSizeA4orLT = TRUE;
		DBG_PRINTF_DEBUG("%s: A4/LT Size :%d \n",__FUNCTION__,size);
	}
	
    if(!foundMatch)
    {
		do
		{
			if(fMediaSizeA4orLT == FALSE)			fSizeDoubleCheck = FALSE;
			DBG_PRINTF_DEBUG("%s: Start !!! %d\n",__FUNCTION__,size);
			
        for(iter = 0; (iter < 4) && !foundMatch; iter++)
        {
            switch(iter)
            {
                case 0:
                    //first try to find exact match
                    DBG_PRINTF_NOTICE("PM: look for size: %ld, type: %ld\n", size, type);
                    findSize = size;
                    findType = type;
                    matchType = HaveSizeHaveType;
                    break;
                case 1:
                    //second try to match the correct size
                    DBG_PRINTF_NOTICE("PM: look for size: %ld, any type\n", size);
                    findSize = size;
                    findType = MEDIATYPE_ANY;
                    matchType = HaveSizeHaveType;
                    break;
                case 2:
                    //third try to match the correct type
                    DBG_PRINTF_NOTICE("PM: look for any size, type: %ld\n", type);
                    findSize = MEDIASIZE_ANY;
                    findType = type;
                    matchType = HaveSizeHaveType;
                    break;
                case 3:
                    //fourth match any non empty tray
                    DBG_PRINTF_NOTICE("PM: look for any size, any type\n");
                    findSize = MEDIASIZE_ANY;
                    findType = MEDIATYPE_ANY;
                    matchType = HaveSizeHaveType;
                    break;
            } // end switch (iter)

            //find any trays with matching size and type
            //Truth table (tray type table is the same)
            //   tray size     find size
            //   ANY            X          Match if X supported by tray
            //    X             Y          no match
            //    Y             X          no match
            //    X             X          Have Size Match
			// 
            // 
            // Note that the incoming job may be set to MEDIASIZE_UNKNOWN or to
            // MEDIATYPE_UNKNOWN. All logic that checks the job or page setting
            // for _ANY size or type must also check for _UNKNOWN size or type.
            // 
            // Note also that *TRAYS* must *NEVER* be *CONFIGURED* to _UNKNOWN
            // size or type!
            // 
			
            tray = starttray = InputTrayIteratorGetFirst();
            DBG_PRINTF_DEBUG("PM: InputTrayIteratorGetFirst : %d\n",tray);
            
            while ( (tray != TRAY_NONE) && !foundMatch )
            {
                if (CheckTray(tray,pSetup,pCnfg,size,type,bestMatch,findSize,findType,matchType))
                    foundMatch = true;
                else
                    tray = InputTrayIteratorGetNext(tray,starttray);
            } // end while ( (tray != TRAY_NONE) && !foundMatch )
            
            if( (iter == 3) && (fMediaSizeA4orLT == TRUE) && !foundMatch )
            {
				fMediaSizeA4orLT = FALSE;
				fSizeDoubleCheck = TRUE;
				DBG_PRINTF_DEBUG("%s: A4/LT Size : MediaSize %d\n",__FUNCTION__,size);
				if(size == MEDIASIZE_A4)
				{		
					size = MEDIASIZE_LETTER;
				}
				else if(size == MEDIASIZE_LETTER)	
				{
					size = MEDIASIZE_A4;
				}
				//iter = 0;
				DBG_PRINTF_DEBUG("%s: A4/LT Size : MediaSize Change to %d\n",__FUNCTION__,size);
			}
        } // end for (iter && !foundMatch)
		
		}while( fSizeDoubleCheck == TRUE );

        //if use any available is true and we didn't find a good non empty tray, we are
        //supposed to use any tray that has paper in it
        //
        // HOWEVER, we should at least prefer to pick from a tray that supports
        // the size and type specified.
        // - If there is such a tray, use it.
        // - If there is not such a tray, then pick from wherever the paper is.
        //
        if ( useAnyAvailable && (bestMatch->matchTypeNonEmpty == HaveNone) )
        {
            trayenum_t bestNonEmptyNonMatchingTray = TRAY_NONE;

            tray = starttray = InputTrayIteratorGetFirst();
            while (tray != TRAY_NONE)
            {
                pTray = &pSetup->inputTrays[tray];
                if (pTray->paperPresent)
                {
                    if ( PrintMgrCheckForSupportedSize(pTray, size) &&
                         PrintMgrCheckForSupportedType(pTray, type) )
                    {
                        DBG_PRINTF_DEBUG("PM: use any available: found supported paper in raw trayenum_t=%d\n", tray);
                        bestMatch->bestNonEmpty = tray;
                        bestMatch->matchTypeNonEmpty = HavePaper;
                        bestNonEmptyNonMatchingTray = TRAY_NONE; // Forget about any non-matching tray.
                        break;
                    }
                    else if (bestNonEmptyNonMatchingTray == TRAY_NONE)
                    {
                        bestNonEmptyNonMatchingTray = tray;
                    }
                } // end if (pTray->paperPresent)
                tray = InputTrayIteratorGetNext(tray,starttray);
            } // end while (tray != TRAY_NONE)

            if (bestNonEmptyNonMatchingTray != TRAY_NONE)
            {
                DBG_PRINTF_DEBUG("PM: use any available: found unsupported paper in raw trayenum_t=%d\n", bestNonEmptyNonMatchingTray);
                bestMatch->bestNonEmpty = bestNonEmptyNonMatchingTray;
                bestMatch->matchTypeNonEmpty = HavePaper;
            }

        } // end if (useAnyAvailable ... )

    } // end if (!foundMatch)

    DBG_PRINTF_DEBUG("PM: PrintMgrFindBestMatch: exit\n");
} // end PrintMgrFindBestMatch()



bool PrintMgrResolve(const printer_setup_t* pSetup, const mediacnfg_t* pMediaCnfg, 
                     STALLCHECKDATA* pStallData, PAGE_DATA* pPageObject, LASTPAGEINFO *lastPage )
{
    mediatype_t tmpType = pStallData->curPageType;
    mediasize_t tmpSize = pStallData->curPageSize;
    trayenum_t tmpTray  = pStallData->curPageTray;
    trayenum_t tmpEmptyTray  = pStallData->curEmptyTray;
    trayenum_t tmpOutTray = pStallData->curPageOutTray;
    BESTMATCH bestTrayMatch;
    const INPUTTRAY* pTray;
	uint32_t fpaperSizeDetect;
	mediasize_t tmpBackupSize;
	uint32_t domismatchcheck;
	
	//uint8_t mediaDescription;
	//uint8_t dbPaperType;
	//uint8_t dbPaperWeight;
	//uint8_t dbPaperTexture;
	
	static trayenum_t prevSourceTray = TRAY_NONE;
	static uint32_t prevJobNum = 0;
	
	DBG_PRINTF_DEBUG("PM: PrintMgrResolve: %d, %d\n",pPageObject->temp_InputTray, pPageObject->DEFAULTSOURCE);
	
	if(pPageObject->pMyJob->JobInfo == COPYJOB)
	{
		tmpType = MEDIATYPE_ANY;
		DBG_PRINTF_DEBUG("PM: PrintMgrResolve: COPYJOB. Force MEDIATYPE_ANY \n");
	}
	
	if((pMediaCnfg->traySettingPriority == 0) && (pPageObject->pMyJob->JobInfo == PRINTJOB))		// SettingPriority : Printer = 0 / Driver = 1
	{
		//set_db_tray_media_description(pPageObject, pMediaCnfg);
		pPageObject->ImageOption.darknessLevel = pMediaCnfg->mediaPrintDarkness;
		pPageObject->DEFAULTSOURCE = pMediaCnfg->defaultSourceTray;
	}
	
	if(pMediaCnfg->tonerSaveMode == 1)
	{
		pPageObject->ImageOption.tonerSave = pMediaCnfg->tonerSaveMode;
	}
	//else
	//{
		//// TODO : Don't care media type if the setting priority is 'driver'
		//DBG_PRINTF_DEBUG("PM: PrintMgrResolve: MEDIATYPE = %d \n",tmpType);
		//switch(tmpType)
		//{
			//case MEDIATYPE_PLAIN_PAPER:			pPageObject->MediaDescription=0x05;		break;			// Plain 		/ Normal 	/ Normal
			//case MEDIATYPE_TRANSPARENCY:		pPageObject->MediaDescription=0x45;		break;			// Transparency / Normal 	/ Normal
			//case MEDIATYPE_CARDSTOCK:			pPageObject->MediaDescription=0x35;		break;			// Cardstock 	/ Normal 	/ Normal
			//case MEDIATYPE_ENVELOPE:			pPageObject->MediaDescription=0x25;		break;			// Envelope 	/ Normal	/ Normal
			//case MEDIATYPE_HEAVY1:				pPageObject->MediaDescription=0x09;		break;			// Plain 		/ Heavy 	/ Normal
			//case MEDIATYPE_LABEL:				pPageObject->MediaDescription=0x15;		break;			// Label 		/ Normal 	/ Normal
			//case MEDIATYPE_LIGHTPAPER:			pPageObject->MediaDescription=0x00;		break;			// Plain 		/ Light 	/ Smooth
			//case MEDIATYPE_CARD_GLOSSY:			pPageObject->MediaDescription=0x65;		break;			// Post Card 	/ Normal	/ Normal
			//case MEDIATYPE_EXTRA_HEAVY:			pPageObject->MediaDescription=0x0a;		break;			// Plain 		/ Heavy 	/ Rough
			//case MEDIATYPE_HEAVY_GLOSSY:		pPageObject->MediaDescription=0x55;		break;			// Vinyllabel   / Normal	/ Normal
			//case MEDIATYPE_RECYCLED:			pPageObject->MediaDescription=0x75;		break;			// Recycled		/ Normal	/ Normal
			//default:							pPageObject->MediaDescription=0x05;		break;
		//}
	//}
	
	if( (tmpSize == MEDIASIZE_B6_JIS) || (tmpSize == MEDIASIZE_A6) || (tmpSize == MEDIASIZE_JAPANESE_POSTCARD) ||
	(tmpSize == MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED) || (tmpSize == MEDIASIZE_ENV_9) || (tmpSize == MEDIASIZE_ENV_10) ||
	(tmpSize == MEDIASIZE_ENV_MONARCH) || (tmpSize == MEDIASIZE_ENV_C5) || (tmpSize == MEDIASIZE_ENV_C6) ||
	(tmpSize == MEDIASIZE_ENV_DL) || (tmpSize == MEDIASIZE_ENV_B5) )
	{
		pPageObject->DEFAULTSOURCE = TRAY_1;
		DBG_PRINTF_DEBUG("PM: PrintMgrResolve : MPT Only Size !!! \n");
	}
	
    // Skeleton implementation
    
	domismatchcheck = true;
	
    pStallData->pageState |= PrintMgrGetSizeAndType(pMediaCnfg, tmpTray, 
                                                    pStallData->pageState, &tmpSize, 
                                                    &tmpType);
                                                    
    // Tray can check A4/A5/LT/LG/EXEC/B5 only
	if ((tmpSize == MEDIASIZE_A4) || (tmpSize == MEDIASIZE_A5) || (tmpSize == MEDIASIZE_LETTER) || (tmpSize == MEDIASIZE_LEGAL) || (tmpSize == MEDIASIZE_EXECUTIVE) || (tmpSize == MEDIASIZE_B5))
		fpaperSizeDetect = true;
	else
		fpaperSizeDetect = false;
		
	tmpBackupSize = tmpSize;
	
	// If the source tray is changed in the same job, give infomation to engine. (for tray3 pre-pick check)
	if((pPageObject->DEFAULTSOURCE != prevSourceTray) && (pPageObject->pMyJob->JobNum == prevJobNum))
	{
		pPageObject->fSourceTrayChg	= true;
		DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Source Tray will be changed in the same JOB (JobID = %d) !!! \n", prevJobNum);
	}
	else
	{
		pPageObject->fSourceTrayChg	= false;
	}
	DBG_PRINTF_DEBUG("PM: PrintMgrResolve : prevSourceTray %d curSourceTray %d prevJobNum %d curJobNum %d (TrayMatchOption = %d)!!! \n"
	, prevSourceTray, pPageObject->DEFAULTSOURCE, prevJobNum, pPageObject->pMyJob->JobNum, pMediaCnfg->trayMatchOptionA4LT);

	prevSourceTray = pPageObject->DEFAULTSOURCE;
	prevJobNum = pPageObject->pMyJob->JobNum;
	
    // find the best matching tray
    DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Requested Size = %d, tray auto detect = %d \n", tmpSize, fpaperSizeDetect);
    if(pPageObject->DEFAULTSOURCE == TRAY_ANY) {

			DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Tray Any \n");
			PrintMgrFindBestMatch(pSetup, pMediaCnfg, tmpSize, tmpType, &bestTrayMatch, pStallData->manualTray, pStallData->useAnyAvailable);
			//}
			//else
			//{
				//tmpTray = pPageObject->temp_InputTray;
				//tmpEmptyTray = pPageObject->temp_InputTray;
			//}

			if(bestTrayMatch.bestNonEmpty != TRAY_NONE)
			{
				tmpTray = bestTrayMatch.bestNonEmpty;
				tmpEmptyTray = TRAY_NONE;
			}
			else
			{
				// TODO: implement stall logic to handle case of no tray with paper
				// for now we assert
				//ASSERT(bestTrayMatch.bestNonEmpty != TRAY_NONE);
		   
				tmpTray = TRAY_NONE;
				if(bestTrayMatch.bestEmpty != TRAY_NONE)
				{
					tmpEmptyTray = bestTrayMatch.bestEmpty;
				}
				else
				{
					tmpEmptyTray = TRAY_NONE;
				}
			}
			DBG_PRINTF_DEBUG("PM: PrintMgrResolve : bestNonEmpty (%d) bestEmpty (%d)\n",tmpTray, tmpEmptyTray);
			if( (tmpTray == TRAY_NONE) && (tmpEmptyTray == TRAY_NONE) )
			{
				if(fpaperSizeDetect == false)
				{
					tmpSize = MEDIASIZE_UNKNOWN;
					PrintMgrFindBestMatch(pSetup, pMediaCnfg, tmpSize, tmpType, &bestTrayMatch, pStallData->manualTray, pStallData->useAnyAvailable);
					
					if(bestTrayMatch.bestNonEmpty != TRAY_NONE)
					{
						tmpTray = bestTrayMatch.bestNonEmpty;
						tmpEmptyTray = TRAY_NONE;
					}
					else
					{
						// TODO: implement stall logic to handle case of no tray with paper
						// for now we assert
						//ASSERT(bestTrayMatch.bestNonEmpty != TRAY_NONE);
				   
						tmpTray = TRAY_NONE;
						if(bestTrayMatch.bestEmpty != TRAY_NONE)
						{
							tmpEmptyTray = bestTrayMatch.bestEmpty;
						}
						else
						{
							tmpEmptyTray = TRAY_NONE;
						}
					}
					DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Retry Case : bestNonEmpty (%d) bestEmpty (%d)\n",tmpTray, tmpEmptyTray);
					if( (tmpTray == TRAY_NONE) && (tmpEmptyTray == TRAY_NONE) )
					{
					}
					else
					{
						domismatchcheck = false;
					}
				}
				
				if ((pPageObject->pMyJob->mismatch_continue == 0) && (domismatchcheck == true))
				{
					set_mismatch_tray(pMediaCnfg->defaultSourceTray);
					set_mismatch_size(tmpBackupSize);
					
					DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Size Mismatch %d %d %d %d requested size = %d (JobID=%d)\n", pMediaCnfg->mediaSize[TRAY_1], pMediaCnfg->mediaSize[TRAY_2]
					, pMediaCnfg->mediaSize[TRAY_3], pMediaCnfg->mediaSize[TRAY_4], tmpBackupSize, pPageObject->pMyJob->JobNum);
					return true;
				}
				else if (pPageObject->pMyJob->mismatch_continue == 1)
				{
					pTray = &pSetup->inputTrays[pMediaCnfg->defaultSourceTray];
					if(pTray->paperPresent)
					{
						tmpTray = pMediaCnfg->defaultSourceTray;
						tmpEmptyTray = TRAY_NONE;
					}
					else
					{
						tmpTray = TRAY_NONE;
						tmpEmptyTray = pMediaCnfg->defaultSourceTray;
					}
					DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Mismatch bestNonEmpty (%d) bestEmpty (%d)\n",tmpTray, tmpEmptyTray);
				}
			}
	}
	else
	{
		DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Tray Selected <%d> \n",pPageObject->DEFAULTSOURCE);
		pTray = &pSetup->inputTrays[pPageObject->DEFAULTSOURCE];
		
		if(pTray->paperPresent)
		{
			tmpTray = pPageObject->DEFAULTSOURCE;
			tmpEmptyTray = TRAY_NONE;
		}
		else
		{
			tmpTray = TRAY_NONE;
			tmpEmptyTray = pPageObject->DEFAULTSOURCE;
		}
		DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Tray Selected - tmpTray (%d) tmpEmptyTray (%d) mismatch_continue = %d\n",tmpTray, tmpEmptyTray, pPageObject->pMyJob->mismatch_continue);
		
		if((pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE] != tmpSize) && (pPageObject->pMyJob->mismatch_continue == 0))
		{
			if((fpaperSizeDetect == false) && (pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE] == MEDIASIZE_UNKNOWN))
			{
				// Unknown size exception
				// Do nothing
			}
			else
			{
				DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Check A4/LT Size <%d, %d> \n", pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE], tmpSize);
				if( (pMediaCnfg->trayMatchOptionA4LT==1) && 
				((pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE] == MEDIASIZE_A4) || (pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE] == MEDIASIZE_LETTER)) &&
				((tmpSize == MEDIASIZE_A4) || (tmpSize == MEDIASIZE_LETTER)) )
				{
					// A4/LT Size exception
					// Do nothing
				}
				else
				{
					set_mismatch_tray(pPageObject->DEFAULTSOURCE);
					set_mismatch_size(tmpBackupSize);
					DBG_PRINTF_DEBUG("PM: PrintMgrResolve : Size Mismatch (%d) %d requested size = %d (JobID=%d)\n",pPageObject->DEFAULTSOURCE
					, pMediaCnfg->mediaSize[pPageObject->DEFAULTSOURCE], tmpBackupSize, pPageObject->pMyJob->JobNum);
					return true;
				}
			}
		}
	}
	
	// Set MediaType for Engine ( type parsing ) 
	if(((pMediaCnfg->traySettingPriority == 0) && (pPageObject->pMyJob->JobInfo == PRINTJOB)) || (tmpType == MEDIATYPE_ANY))		// SettingPriority : Printer = 0 / Driver = 1
	{
		set_db_tray_media_description(pPageObject, pMediaCnfg, tmpTray);
	}
	else
	{
		// TODO : Don't care media type if the setting priority is 'driver'
		switch(tmpType)
		{
			case MEDIATYPE_PLAIN_PAPER:			pPageObject->MediaDescription=0x05;		break;			// Plain 		/ Normal 	/ Normal
			case MEDIATYPE_TRANSPARENCY:		pPageObject->MediaDescription=0x45;		break;			// Transparency / Normal 	/ Normal
			case MEDIATYPE_CARDSTOCK:			pPageObject->MediaDescription=0x35;		break;			// Cardstock 	/ Normal 	/ Normal
			case MEDIATYPE_ENVELOPE:			pPageObject->MediaDescription=0x25;		break;			// Envelope 	/ Normal	/ Normal
			case MEDIATYPE_HEAVY1:				pPageObject->MediaDescription=0x09;		break;			// Plain 		/ Heavy 	/ Normal
			case MEDIATYPE_LABEL:				pPageObject->MediaDescription=0x15;		break;			// Label 		/ Normal 	/ Normal
			case MEDIATYPE_LIGHTPAPER:			pPageObject->MediaDescription=0x00;		break;			// Plain 		/ Light 	/ Smooth
			case MEDIATYPE_CARD_GLOSSY:			pPageObject->MediaDescription=0x65;		break;			// Post Card 	/ Normal	/ Normal
			case MEDIATYPE_EXTRA_HEAVY:			pPageObject->MediaDescription=0x0a;		break;			// Plain 		/ Heavy 	/ Rough
			case MEDIATYPE_HEAVY_GLOSSY:		pPageObject->MediaDescription=0x55;		break;			// Vinyllabel   / Normal	/ Normal
			case MEDIATYPE_RECYCLED:			pPageObject->MediaDescription=0x75;		break;			// Recycled		/ Normal	/ Normal
			default:							pPageObject->MediaDescription=0x05;		break;
		}
	}
	
	DBG_PRINTF_DEBUG("PM: PrintMgrResolve: Final MEDIATYPE = 0x%02x !!!!!\n",tmpType);
    // make sure we have a reasonable out tray
    if(!(pStallData->pageState & HaveOutTray))
    {
        pStallData->pageState |= HaveOutTray;
        tmpOutTray = TRAY_1;
    }
    pStallData->curPageType = tmpType;
    pStallData->curPageSize = tmpSize;
    pStallData->curPageTray = tmpTray;
    pStallData->curEmptyTray = tmpEmptyTray;
    pStallData->curPageOutTray = tmpOutTray;

    return false;   // don't stall

} // end PrintMgrResolve()
