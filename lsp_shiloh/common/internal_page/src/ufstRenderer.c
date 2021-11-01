/****************************************************************************** 
 * ============================================================================
 * Copyright (c) 2007 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:  Product dependent functions to put marks on the page.
*/  
#include "string.h"
#include "memAPI.h"
#include "agConnectMgr.h"       // this needs to be in front of renderer.h
#include "agLinkedList.h"
#include "agprint.h"
#include "lassert.h"
#include "agLinkedList.h"
#include "logger.h"
#include "utf8.h"
#include "ufst_font_driver_ip.h"
#include "renderer.h"

#define CHARSIZE 8	        ///< Number of bits per byte	  
#define JUSTIFY_LEFT -1     ///< Force justify left

// 16.16 fixed point format round to int
#undef ROUND_16
#define ROUND_16( x ) (((x) + 8) >> 4)
//
// length is the length in bytes of the input buffer.
//          output is the length in bytes of the output buffer.
uint8_t *convert_1bpp_to_output(uint8_t *input, uint32_t BPP, uint32_t *length, uint32_t width, uint32_t Height)
{
    uint8_t *image;
    uint32_t Line, pixel, LineOffset;;
    *length = BPP * width;
    uint8_t pixval;
    if(*length % CHARSIZE)
        *length = *length/CHARSIZE + 1;
    else
        *length = *length/CHARSIZE;
    pixval = 0xff >> (CHARSIZE - BPP); // what to or in

    image = MEM_CALLOC(1, *length * Height);

    ASSERT(image != NULL);
    for(Line = 0; Line < Height; Line++)
    {
        LineOffset = Line * *length;
        for(pixel = 0; pixel < width; pixel++)
        {
            if(input[(LineOffset + pixel)/CHARSIZE] & 1<<(CHARSIZE-1 - pixel % CHARSIZE))
            {
                image[LineOffset + (pixel * BPP)/CHARSIZE] |=
                    pixval << (CHARSIZE/BPP-1 -(pixel * BPP)%CHARSIZE) * BPP;
            }
        }
    }
    return image;
}
static IF_STATE *font_if;
/**\brief Text renderer using UFST font driver 
 *
 */
void UFSTTextRenderer(RendererSession *pRSession, 
                      unsigned char *Strip, 
                      DISP_LIST *TempPage, 
                      RENDER_INPUT *RendInput, 
                      UINT32 Color, 
                      UINT32 Rotate)
{
    UINT32 StrLen, i;
    int YMax, YMin, XLoc, XMax;
    char *TempString = 0;
    UINT32 *TempString32 = 0;
    PIFBITMAP chBmp;
    static UINT32 curFont =-1;
    int32_t space_size=-1;
    int StartY, Offset, Len;
    int32_t str_len_pixels = 0;
    uint32_t space_char='A';

    if(!TempPage)   // check for null
        return;

    // First see if there is anything from this object on this color plane.
    //
    if ((((TEXT *)TempPage->Object)->Color & 1<<Color) == 0)
        return;      // nothing on this plane, go on to the next object.

    //Calculatethe pixel postion and span of the obj if necessary
    // This is a bounding box that covers the string.  This is used
    // to decide if a string is part of a given strip.  This calculation is
    // done once and the result is stored in the disp_list object.  
    //
    if (TempPage->OBJ_X_LEN==0xFFFFFFFF)    // has this calcuation been done?
    {  
        // We need to calculate pixel position and span of the text in the page
        // to know where to put it.

        TempString = ((TEXT *)TempPage->Object)->Data; //UTF8 string
        StrLen = UTF8StrLen((uint8_t *)TempString);

        TempString32 = MEM_MALLOC((StrLen+1)*4);

        UTF8ToU32(TempString32, (uint8_t *) TempString);   // convert the string.


        curFont = ((TEXT *)TempPage->Object)->FONT;
        // if something about the font has changed, update here.
        ufst_font_driver_ip_setup_font(&font_if, ((TEXT *)TempPage->Object)->lang, 
                                       ((TEXT *)TempPage->Object)->FONT, false );

        // figure the ymax and ymin values for the string.
        //
        YMax = 0;
        YMin = 40000;
        XMax = 0;
        //
        // For space we take the size of the space_char and use that for spacing.
        //
        getCharBmp(font_if, space_char, (PPIFBITMAP)&chBmp);
        space_size = char_escape(chBmp);
        //
        // scan the string looking the the max y size of the object.
        // also look for the max y offset for setting the cap.
        // If this is rotated we need to figure the length of the
        // string in pixels so we can tell if we are in this
        // stripe.
        //
        for (i = 0; i < StrLen; i++)
        {
            // just move our horizontal position for a space or non-breaking-space character
            if ((TempString32[i]==' ') || (TempString32[i]==0x00A0))
            {
                XMax += space_size;
                str_len_pixels += space_size;
                continue;
            }
            else if (TempString32[i]==13 ||TempString32[i]==10 )
                continue;

            // get the character from the font renderer.
            //
            getCharBmp(font_if, TempString32[i], (PPIFBITMAP)&chBmp);
            str_len_pixels += char_escape(chBmp);

            if (YMax < (TempPage->OBJ_Y + (chBmp->black_depth- ROUND_16(chBmp->yorigin))))
                YMax =  (TempPage->OBJ_Y + (chBmp->black_depth- chBmp->yorigin/16));

            // dprintf("black_dep=%d, yorigin/16=%d, YMax=%d\n", chBmp->black_depth, chBmp->yorigin/16, YMax);

            if (YMin > ((int)TempPage->OBJ_Y- (int)(chBmp->yorigin/16)))
                YMin = (int)TempPage->OBJ_Y - (int)(chBmp->yorigin/16);
            if (YMin < 0)
                YMin = 0;

            //
            // XMax is the length of the string in pixels.  Only used for
            // landscape printing.
            //  
            if (Rotate)
                XMax += chBmp->depth + 8;  //8 is spacing used

        }
        if (!Rotate)
        {
            TempPage->OBJ_X_LEN = YMin;
            TempPage->OBJ_Y_LEN = YMax;
        }
        else
        {
            TempPage->OBJ_X_LEN =XMax; 
        }

        // support for center left and right justification of strings.
        if (((TEXT *)TempPage->Object)->justify > 0)
            TempPage->OBJ_X -= str_len_pixels;  // x is right justify point.
        else if (((TEXT *)TempPage->Object)->justify == 0)
        {

            if (TempPage->OBJ_X > 0 && 
                TempPage->OBJ_X > str_len_pixels / 2) // x is center justify point.
                TempPage->OBJ_X -=  str_len_pixels / 2;
        }
        // else x is left justify point. 

        // this text is now treated as left justified from here on out
        ( ( TEXT * )TempPage->Object )->justify = JUSTIFY_LEFT;

    }
    else
    {
        // The bounding box calculation has been done before.  Get the
        // results and initialize the correct variables.
        //
        if (!Rotate)
        {
            //Get the text obj pixel start and end position
            YMin = TempPage->OBJ_X_LEN;
            YMax = TempPage->OBJ_Y_LEN;
        }
        else
        {
            XMax = TempPage->OBJ_X_LEN;

        }
    }
    //
    // See if any part of this string is in this strip.
    //

    if (!Rotate && ((pRSession->CurY > YMax) || ((pRSession->CurY + RendInput->OutBufLPS) < YMin)))
    {
        if (TempString32)
            MEM_FREE_AND_NULL(TempString32);
        return;      // this string is not in this stripe at all.
    }

    //
    // If we are doing landscape, check to see if the text is in this strip.
    //
    if (Rotate && ((pRSession->CurY > (TempPage->OBJ_X + XMax)) || 
                   ((pRSession->CurY + RendInput->OutBufLPS) < TempPage->OBJ_X)))
    {
        if (TempString32)
            MEM_FREE_AND_NULL(TempString32);
        return;
    }

    //Read the string if we haven't done it already
    if (TempString==0)
    {
        TempString = ((TEXT *)TempPage->Object)->Data; //UTF8 string
        StrLen = UTF8StrLen((uint8_t *)TempString);
        TempString32 = MEM_MALLOC((StrLen+1)*4); // *4 since this is a 32 bit value.
        UTF8ToU32(TempString32, (uint8_t *)TempString);
        curFont = ((TEXT *)TempPage->Object)->FONT;
        // see if something about this font has changed and update if needed.
        //
        ufst_font_driver_ip_setup_font(&font_if, ((TEXT *)TempPage->Object)->lang, 
                                       ((TEXT *)TempPage->Object)->FONT, Rotate );
    }
    //
    // get the size of the space for use if the string has a space.
    // 

    getCharBmp(font_if, space_char, (PPIFBITMAP)&chBmp);
    space_size = char_escape(chBmp);
    //
    // Now put each character into the strip.
    // This works by looking at each character and seeing if 
    // any part of that character is in this strip.  If it
    // is then copy that part into the strip.
    //
    XLoc = 0;      // this is the x location in pixels within 
                   // the current string.
                   //
    for (i = 0; i < StrLen; i++)
    {
        DISP_LIST *TempDispList;
        GRAPHIC *TempGraphic;

        // just move our horizontal position for a space or non-breaking-space character
        if ((TempString32[i]==' ') || (TempString32[i]==0x00A0))
        {
            XLoc += space_size;
            continue;
        }
        else if (TempString32[i]==13 ||TempString32[i]==10 ) // if eol character, ignore it.
            continue;

        // Get the bitmap of the character.
        //
        getCharBmp(font_if, TempString32[i], (PPIFBITMAP)&chBmp);

        // the character exists in the table,  See if any 
        // of it is located in this stripe.
        // See if any of this character is in the stripe.
        if ((!Rotate &&
             ( ((int)pRSession->CurY < (int)(TempPage->OBJ_Y + chBmp->black_depth- chBmp->yorigin/16))&&
               ( (int)(pRSession->CurY+ RendInput->OutBufLPS) > (int)(TempPage->OBJ_Y - chBmp->yorigin/16)) 
             )
            )
            ||
            ( Rotate && !(( (pRSession->CurY+RendInput->OutBufLPS) < (TempPage->OBJ_X + XLoc)) ||
                          (( pRSession->CurY > (TempPage->OBJ_X + XLoc + chBmp->black_depth))))
            )
           )
        {
            //
            // Part of this character is in this stripe.  Put it in.
            //
            // Figure the Y location where the character starts.
            // This is relative to the start of the page.
            //
            if (!Rotate)
            {
                StartY = TempPage->OBJ_Y - chBmp->yorigin/16;
            }
            else
            {
                StartY = TempPage->OBJ_X + XLoc;
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
            Len = chBmp->black_depth - Offset;        // how many lines left to put
            //
            // Limit the text to this stripe.
            // The character is limited to
            // the stripe height here.
            //
            if (((StartY + Len)> (pRSession->CurY + RendInput->OutBufLPS)))
            {
                Len = pRSession->CurY + RendInput->OutBufLPS - StartY;
            }
            //
            // Make the numbers relative to the start of the stripe.
            // We don't care where we are on the page,
            // only where we are in the stripe.
            // bias back to stripe relative.
            //
            StartY -= pRSession->CurY;
            //
            // We are putting something on this stripe.  Make it dirty.
            //
            pRSession->Dirty = 1;
            //
            // Now we know where to put the part of the character needed.
            // Make a graphics object to do that so we can use the standard
            // copyobj to put it down.
            // Fill the graphic object with the information
            //
            //
            // We do not need to rotate, that was done in the font renderer if required
            // however we need to make a graphic
            // object and fill it in with the character information.
            //
            TempGraphic = (GRAPHIC *) MEM_CALLOC(1, sizeof(GRAPHIC)+ sizeof(DISP_LIST));
            ASSERT(TempGraphic != NULL);    // error check.
            TempGraphic->X_DIMENSION = chBmp->width*8;
            TempGraphic->Y_DIMENSION = chBmp->depth;
            TempGraphic->LENGTH_X = chBmp->width;

            // Put the bitmap into the correct plane for this color.
            *(((UINT32 *) &(TempGraphic->K_OR_R_PLANE))+Color) =  (UINT32)(chBmp->bm);
            //
            // The display list is as was passed to us.
            //
            TempDispList = (DISP_LIST *)((UINT32)TempGraphic + sizeof(GRAPHIC));
            memcpy(TempDispList, TempPage, sizeof(DISP_LIST));
            //
            // Make some modifications to put the character where required.
            //
            TempDispList->OBJ_X += XLoc + chBmp->xorigin/16;
            if (Rotate)
            {
                TempDispList->OBJ_X = TempDispList->OBJ_Y;
                TempDispList->OBJ_Y = TempPage->OBJ_X + XLoc;
            }
            //Need to clear the OBJ_X_LEN and OBJ_Y_LEN since we don't stretch characters
            //
            TempDispList->OBJ_X_LEN=0;
            TempDispList->OBJ_Y_LEN=0;
            //
            // The character is 1 bit data.  Convert it into the correct bpp.  
            //
            if (RendInput->BPP != 1)
            {
                *(((UINT32 *) &(TempGraphic->K_OR_R_PLANE))+Color) = 
                (uint32_t)convert_1bpp_to_output((uint8_t *)chBmp->bm, RendInput->BPP, 
                                                 &TempGraphic->LENGTH_X, TempGraphic->X_DIMENSION,
                                                 TempGraphic->Y_DIMENSION);

            }
            CopyObj(Strip, TempGraphic, Color, TempDispList, StartY, Len, Offset, 
                    pRSession->PIXELLINE*RendInput->BPP/CHARSIZE, 0, RendInput->BPP,
                    get_pixel_stride(RendInput->packing), get_pixel_offset(RendInput->packing)+Color,
                    RendInput->data_type);
            // Free the previously allocated scratch memory.
            // This means some characters will be rotated
            // a large number of times.  This can be changed
            // to improve efficiency.  I did it because I 
            // wanted to insure that I would not run out of
            // memory and the thing is fast enough anyway.
            //
            if (RendInput->BPP != 1)
            {
                memFree((uint8_t *)*(((UINT32 *) &(TempGraphic->K_OR_R_PLANE))+Color));
            }
            MEM_FREE_AND_NULL(TempGraphic);
        }
        //
        // This character is finished, space to the next one.
        XLoc += char_escape(chBmp);         
    }
    // done with this pass.  If we have a string allocated, free it.
    if (TempString32)
        MEM_FREE_AND_NULL(TempString32);

    return;
}


