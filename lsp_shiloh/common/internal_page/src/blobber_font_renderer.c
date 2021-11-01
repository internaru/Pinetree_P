
/*
 *
 * ============================================================================
 * Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/  


#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "agRouter.h"
#include "memAPI.h"
#include "agLinkedList.h"
#include "lassert.h"
#include "renderer.h"  
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "list.h"
#include "internal_page_config.h"

#define CHARSIZE 8  // number of bits in a byte
#define RAM_SIGNATURE 0x4652414E    //Added to verify whether fonts were picked from RAM or ROM. 

/**\brief Old fashion blobber based text renderer.
 *
 * \param[in] TempPgae  Object to renderering. An NULL signals the end of renderering
*
*/
void BlobberTextRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            UINT32 Color, 
                            UINT32 Rotate,
                            bool *strip_dirty,
                            uint32_t color_index)
{
    UINT32 StrLen, i;
    int YMax, YMaxOff, YMinY, XLoc, XMax;
    char *TempString;
    unsigned char *Font;
    int StartY, Offset, Len;
    static unsigned char *Fonts = 0;

    if (!TempPage) //An NULL TempPage is for signaling end of renderering, do some cleaning work
    {
        if (!Fonts)   //make sure there is cleaning to do
            return;

        if ((UINT32)*(UINT32*)Fonts!=RAM_SIGNATURE)  //Do release if font is from ROM
            MEM_FREE_AND_NULL(Fonts);

        Fonts = 0;
        return;
    }

    if(!Fonts)  
    {
        // We have strings and need the fonts.
        // Unpack the fonts into the array
   //     Fonts = RomMgr(e_Font,0);  // Get the fonts out of the rom and into memory.
        if (Fonts == NULL)
        {
            DPRINTF( (DBG_SOFT | DBG_OUTPUT), ("REND: Unable to get Fonts!\n"));
            //ASSERT(0);
            return;
        }
    }

    // First see if there is anything from this object on this color plane.
    //
    if ((((TEXT *)TempPage->Object)->Color & 1<<Color) == 0)
      return ;      // nothing on this plane, go on to the next object.
    //
    // Initialize some variables
    //
    if ((UINT32)*(UINT32*) Fonts==RAM_SIGNATURE)    //If fonts obtained from RAM, we do need to move the Font pointer
    {
        Font = Fonts;
    }
    else
    {

        Font = ((UINT32)((UINT32 *)Fonts)[((TEXT *)TempPage->Object)->FONT])+ Fonts; 
        if (Font == Fonts)
        {
            DPRINTF( (DBG_SOFT | DBG_OUTPUT),("Font specified that is not supported\n"));
            return;
        }
    }

    TempString = ((TEXT *)TempPage->Object)->Data;

    // figure the ymax and ymin values for the string.
    //
    StrLen = strlen(TempString);
    YMax = -4000000;
    YMaxOff = -4000000;
    YMinY = 4000000;
    XMax = 0;
    //
    // scan the string looking the the max y size of the object.
    // also look for the max y offset for setting the cap.
    // If this is rotated we need to figure the length of the
    // string in pixels so we can tell if we are in this
    // stripe.
    //
    for (i = 0; i < StrLen; i++)
    {
        int TCH, TOf;
        if (TempString[i] == ' ')    // if this is a space, skip this stuff
        {
            XMax += ((FONT_FORMAT *)&Font[((UINT32 *)Font)['1']])->SpaceLength;
            continue;
        }
        TCH = ((FONT_FORMAT *)&Font[((UINT32 *)Font)[(UINT32)TempString[i]]])->CharHeight;
        if (YMax < TCH)
            YMax =  TCH;
        TOf = ((FONT_FORMAT *)&Font[((UINT32 *)Font)[(UINT32)TempString[i]]])->TopOffset;    
        if (YMaxOff < TOf)
            YMaxOff = TOf;
        if (YMinY < (TCH - TOf))
            YMinY = TCH - TOf;
        //
        // XMax is the length of the string in pixels.  Only used for
        // landscape printing.
        //
        XMax += ((FONT_FORMAT *)&Font[((UINT32 *)Font)[(UINT32)TempString[i]]])->CharSpacing;
    }
        // Now see if anything is in this stripe.  This check is for potrait prints
    if (!Rotate && (((pRSession->CurY > (TempPage->OBJ_Y + YMinY)) || 
            ((pRSession->CurY + RendInput->OutBufLPS) < (TempPage->OBJ_Y - YMaxOff)))))
        return;      // this string is not in this stripe at all.

    //
    // If we are doing landscape, check to see if the text is in this strip.
    //
    if (Rotate && ((pRSession->CurY > (pRSession->YLen - TempPage->OBJ_X)) || 
            ((pRSession->CurY + RendInput->OutBufLPS) < (pRSession->YLen - TempPage->OBJ_X - XMax))))
        return;

    //
    // Now put each character into the strip.
    // This works by looking at each character and seeing if 
    // any part of that character is in this strip.  If it
    // is then copy that part into the strip.
    //
    XLoc = 0;      // this is the location in pixels within 
    // the current string.
    for (i = 0; i < StrLen; i++)
    {
        FONT_FORMAT *TempChar;

        if (TempString[i] == ' ')
        {
            // Print a space
            // Since space is not a character the font tables do not have a
            // space.  So grab the spacing for a "l" character and use that
            // for the space width.
            //
            XLoc += ((FONT_FORMAT *)&Font[((UINT32 *)Font)['1']])->SpaceLength;  // get space length from character "1"
            continue;
        }
        // Get a pointer to the character to place.
        TempChar = (FONT_FORMAT *) &Font[((UINT32 *)Font)[(UINT32)TempString[i]]];  // This is the pointer to the char to print

        if (TempChar != (FONT_FORMAT *) Font)    // see if the char exists in table
        {
            DISP_LIST *TempDispList;
            GRAPHIC *TempGraphic;
            // the character exists in the table,  See if any 
            // of it is located in this stripe.
            // See if any of this character is in the stripe.
            if ((!Rotate && 
                    ((pRSession->CurY < TempPage->OBJ_Y + TempChar->CharHeight - TempChar->TopOffset) &&
                    ((pRSession->CurY + RendInput->OutBufLPS) > (TempPage->OBJ_Y - TempChar->TopOffset)))) ||
                ( Rotate && 
                    !(( (pRSession->CurY+RendInput->OutBufLPS) < (pRSession->YLen - TempPage->OBJ_X - TempChar->CharWidth - XLoc)) ||
                    (( pRSession->CurY > (pRSession->YLen - TempPage->OBJ_X - XLoc))))))
            {
            //
            // Part of this character is in this stripe.  Put it in.
            //
            // Figure the Y location where the character starts.
            //
            if (!Rotate)
            {
               StartY = TempPage->OBJ_Y - TempChar->TopOffset;
            }
            else
            {
                StartY = pRSession->YLen - TempPage->OBJ_X - XLoc- TempChar->CharWidth;
            }
            //
            // Now figure the offset into the character.  
            // This makes sure that StartY and the
            // Character start are lined up.
            //
            Offset = 0; // offset into character.
            if ( (StartY < pRSession->CurY))
            {
                // The start of the character is before the stripe.
                //
                Offset = pRSession->CurY - StartY;    // how far to space into the character.
                StartY = pRSession->CurY;
            }
            //
            // Now that we know the start we need 
            // to find the length of the part of
            // the character that is in this stripe.
            //
            if (!Rotate)
                Len = TempChar->CharHeight - Offset;        // how many lines left to put
            else
                Len = TempChar->CharWidth - Offset;
            //
            // Limit the text to this stripe.
            // The character is limited to
            // the stripe height here.
            //
            if (((StartY + Len)> ( pRSession->CurY + RendInput->OutBufLPS)))
            {
                Len =  pRSession->CurY + RendInput->OutBufLPS - StartY;
            }
            //
            // Make the numbers relative to the start of the stripe.
            // We don't care where we are on the page,
            // only where we are in the stripe.
            // bias back to stripe relative.
            //
            StartY -=  pRSession->CurY;
            //
            // We are putting something on this stripe.  Make it dirty.
            //
            pRSession->Dirty = 1;
            *strip_dirty = true;
            //
            // Fill the graphic object with the information
            //

            if (Rotate)
            {
                    //
            // The character needs to be rotated.  Do that here
            // and put the results into the stripe.
            //
                UINT32 Size;
                LINK_MEMBER InLink, OutLink;

                Size = TempChar->CharHeight/8;  // x size of object
                if ((TempChar->CharHeight % 8) != 0)
                    Size++;
                //
                //  Now make the buffer 4 byte aligned.
                // 
                if((Size % 4) != 0)
                    Size = (Size & 0xfffffffc) + 4;
                    //
                    // get the memory for the rotated character.
                TempGraphic = (GRAPHIC *) MEM_CALLOC(1, TempChar->CharWidth  * Size 
                                                   + sizeof(GRAPHIC));
                ASSERT(TempGraphic != NULL);
                    // Fill in the graphic object values.
                TempGraphic->Y_DIMENSION = TempChar->CharWidth;
                TempGraphic->X_DIMENSION = TempChar->CharHeight;
                TempGraphic->LENGTH_X = Size;
                *((UINT32 *)(((UINT32)(&TempGraphic->K_OR_R_PLANE)) + Color)) = 
                (UINT32)((UINT32) TempGraphic + sizeof(GRAPHIC));
                // Build the linked list for the rotate.
                // InLink is input OutLink is output.
                //
                InLink.NextLink = NULL;
                InLink.Param = 10000;       // everything is in this stripe
                InLink.Data = (void *)((UINT32) TempChar) + sizeof(FONT_FORMAT);
                OutLink.NextLink = NULL;
                OutLink.Data = (void *) *((UINT32 *)(&TempGraphic->K_OR_R_PLANE) + Color);
                OutLink.Param = 1000;   // number of lines in this buffer
                // Rotate the character.
                rotate(TempGraphic->Y_DIMENSION, TempGraphic->X_DIMENSION, 
                         TempChar->LineLength, &InLink, &OutLink, RendInput->BPP, 
                         RendInput->ImagesRotation);
                //
                // build the display list item with the info from the initial
                // display list.
                // This is junk, just allocate and free later DB
                TempDispList = (DISP_LIST *)(*((UINT32 *)(((UINT32)(&TempGraphic->K_OR_R_PLANE)) 
                                                          + Color)) + Size*TempChar->CharWidth);
                TempDispList->OBJ_X = pRSession->PIXELLINE - (TempPage->OBJ_Y + TempChar->TopOffset);
                TempDispList->OBJ_Y = pRSession->YLen - TempPage->OBJ_X;
                TempDispList->OBJ_X_LEN = 0; //TempPage->OBJ_Y_LEN;
                TempDispList->OBJ_Y_LEN = 0; //TempPage->OBJ_X_LEN;
                TempDispList->Operation = TempPage->Operation;
            }
            else
            {         
                //
                // We do not need to rotate, however we need to make a graphic
                // object and fill it in with the character information.
                //
                TempGraphic = (GRAPHIC *) MEM_CALLOC(1,sizeof(GRAPHIC)+ sizeof(DISP_LIST)); 
                ASSERT(TempGraphic != NULL);    // error check.
                TempGraphic->X_DIMENSION = TempChar->CharWidth;
                TempGraphic->Y_DIMENSION = TempChar->CharHeight;
                TempGraphic->LENGTH_X = TempChar->LineLength;
                *(((UINT32 *) &(TempGraphic->K_OR_R_PLANE))+Color) = 
                                (UINT32) ((unsigned char *)TempChar)+ sizeof(FONT_FORMAT);
            //
            // The display list is as was passed to us.
            //
                TempDispList = (DISP_LIST *)((UINT32)TempGraphic + sizeof(GRAPHIC));
                memcpy(TempDispList, TempPage, sizeof(DISP_LIST));
                TempDispList->OBJ_X += XLoc;
                TempDispList->OBJ_X_LEN = 0; //TempPage->OBJ_Y_LEN;
                TempDispList->OBJ_Y_LEN = 0; //TempPage->OBJ_X_LEN;
            }
          
            // Put the character into the strip.
            CopyObj(Strip, TempGraphic, Color, TempDispList,StartY, Len, Offset, (pRSession->PIXELLINE*RendInput->BPP)/CHARSIZE, 0, RendInput->BPP,
                    get_pixel_stride(RendInput->packing), Color, RendInput->data_type);
            // Free the previously allocated scratch memory.
            // This means some characters will be rotated
            // a large number of times.  This can be changed
            // to improve efficiency.  I did it because I 
            // wanted to insure that I would not run out of
            // memory and the thing is fast enough anyway.
            //
            MEM_FREE_AND_NULL(TempGraphic);
        }
    }
    // This character is finished, space to the next one.
    XLoc += TempChar->CharSpacing;   
   }
  return;
}

