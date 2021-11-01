/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2007-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:  Function to put marks on the page.
*/  
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "posix_ostools.h"
#include "agRouter.h"
#include "memAPI.h"
#include "agMessage.h"
#include "agprint.h"
#include "agJobUtils.h"
#include "lassert.h"
#include "agLinkedList.h"
#include "renderer.h"  
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "list.h"
#include "internal_page_config.h"
#include "error_types.h"
#include "dma_buffer.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"

static int renderer_cmd_raster_cb(int argc, char *argv[]);
static int renderer_cmd_line_cb(int argc, char *argv[]);
static int renderer_cmd_text_cb(int argc, char *argv[]);
static int renderer_cmd_print_text_cb(int argc, char *argv[]);
static const char *renderer_cmd_raster_desc;
static const char *renderer_cmd_raster_usage;
static const char *renderer_cmd_raster_notes;
static const char *renderer_cmd_line_desc;
static const char *renderer_cmd_line_usage;
static const char *renderer_cmd_line_notes;
static const char *renderer_cmd_text_desc;
static const char *renderer_cmd_text_usage;
static const char *renderer_cmd_text_notes;
static const char *renderer_cmd_print_text_desc;
static const char *renderer_cmd_print_text_usage;
static const char *renderer_cmd_print_text_notes;

#endif

                            
unsigned char *Fonts;   ///< Pointer to the font tables.

/** Renderer use following object renderers for renderering */
static OBJ_RENDERER renderers[]=
{ 
    {e_Text, NULL},          // this must be filled out by setobjectrenderer to select 
                             // font driver, NULL, nothing selected
    {e_Graphics,     GraphicsRenderer}, 
    {e_LinkGraphics, LinkGraphicsRenderer}, 
    {e_DrawLine,     LineRenderer}, 
    {0,0} 
};

/**
 * \brief Use this function the setup a customized renderer for an object type.
 * This is used to the config section to specify special renderers.
 * \param[in] type The type of renderer to replace
 * \param[in] renderer The callback renderer for this type
 * \returns int 
 * \retval 0 success
 * \retval 1 failure or assert
 *
 */

int setObjectRenderer(OBJ_TYPE type, RENDERER_FUN renderer)
{
    int i;
    i = 0;
    while(renderers[i].renderer || renderers[i].type)
    {
       
       if(renderers[i].type == type)
       {
           renderers[i].renderer = renderer;
           break;
       }
       i++;
    }
    ASSERT(renderers[i].renderer);
    if(!renderers[i].renderer)
        return 1;

    return 0;
}


/**
 * Function name: CopyObj
 *
 * \brief Copy a given object into the strip.
 *
 * Description: Given the parameters, copy the object into the strip
 *
 * \param Strip Where to put the image data.
 * \param Object The graphic object to put in the strip.
 * \param Plane The color plane to place the object.
 * \param DispObj The info about the object where to put it.
 * \param Start Where the object starts in the strip
 * \param Length the length to put into the strip.
 * \param ObjOffset Where to start Y in the strip
 * \param LineLength length in pixels of a line in the strip
 * \param XOffset where to start in the object.
 * \param pixel_stride This is used for packing.  Size of the pixel ie 4 for rgbx packed, 3 for rgb, 1 for planar
 * \param[in] pixel_offset The offset used for packing.  ie if pixel_stride is 4 this is which pixel to store
 * 
 * Author: 
 *
 * Notes: 
 *
 **/
void CopyObj(unsigned char *Strip, GRAPHIC *Object, UINT32 Plane, DISP_LIST *DispObj, UINT32 Start, 
             UINT32 Length, UINT32 ObjOffset, UINT32 LineLength,
              UINT32 XOffset, UINT32 BPP, uint8_t pixel_stride, uint8_t pixel_offset, rend_byte_type_t data_type)
{
    UINT32 Line, XStartByte, Shift, ObjOff, XLen, CurrentXPtr, XObjLen;
    int XLength;
    UINT32 CurXByte;
    UINT32 CurY;
    OBJ_OPERATION Operation;
    unsigned char Residue, *ObjPtr, LastBitMask;
    UINT32 NumBits, NeededBits, ResidueBits;
    UINT32 SlantDirection = 0;      // 1 slants up, 0 slants down

    // change the operation based on the data type.  
    // The objects coming are 0 for clear, 1 for full intensity.  With rgb it is
    // the opposite so the operation must be changed to match.
    Operation = DispObj->Operation;

    for (Line = Start; Line < (Start + Length); Line++)
    {
        Residue = 0;      // the amount to carry into the next byte.
        CurrentXPtr = 0;
        CurY = (Line - Start + ObjOffset) % Object->Y_DIMENSION;         // find the line that we are working on.
        //
        // Find the length of the area to put in.
        //
        XLength = DispObj->OBJ_X_LEN*BPP;
        if (XLength == 0) 
            XLength = Object->X_DIMENSION * (SINT32) BPP;
        if ((XLength+DispObj->OBJ_X*BPP) > (LineLength * BPP))
            XLength = LineLength * BPP - DispObj->OBJ_X * BPP;
        while (XLength > 0)
        {
            bool no_bytes; /* no complete bytes of data existed */
            //
            // Find out how much of the object to put down this time around.
            //
            XObjLen = XLength;
            if (XObjLen > Object->X_DIMENSION*BPP)
                XObjLen = Object->X_DIMENSION*BPP;
            //
            // Figure how much to shift the byte before doing the operation.
            //
            Shift = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset) % CHARSIZE;         // number of extra bits.
            //
            // Figure where we need to start in the strip.
            //
            XStartByte = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset)*pixel_stride / CHARSIZE + //move to correct column
                         (Line * ((LineLength*BPP)/8) * pixel_stride); //move to correct row
            //
            XLen = (XObjLen + Shift) / CHARSIZE;
            LastBitMask = (XObjLen + Shift) % CHARSIZE;
            //
            // Get the address of the plane to get the data from.
            //
            ObjPtr = (unsigned char *) *(((UINT32 *) &(Object->K_OR_R_PLANE)) + Plane);     // address of plane of object
            //
            // Now put the bytes into the strip.
            //
            ObjOff = CurY * Object->LENGTH_X;           // must init here in case XLen is 0.
            no_bytes = XLen?false:true;
            for (CurXByte = 0; CurXByte < XLen; CurXByte++)
            {
                //
                // If I am in here I have at least 8 bits to put down.
                //
                ObjOff = CurY * Object->LENGTH_X + CurXByte;       // get the address of our buffer location
                ResidueBits = 0;
                SlantDirection = 0;
                if((CurXByte + (DispObj->OBJ_X*BPP)/CHARSIZE)  >= LineLength)   // we hit the end of the line, we are done.
                    break;      

                //
                // Now put the bits into the strip.
                //
                switch (Operation)
                {
                    case e_diag_up:
                        SlantDirection = 1;
                    case e_diag_down:
                        {
                            // Figure the number of bits we need to set this up.
                            if (!SlantDirection)
                                NeededBits = DispObj->OBJ_Y_LEN - CurY;     // the number of bits needed at the start
                            else
                                NeededBits = CurY;

                            if (!CurrentXPtr && ((CurXByte == 0) || 
                                                 ((CurXByte-1) * CHARSIZE + (CHARSIZE - Shift)) < NeededBits))
                            {
                                //
                                // Do the diagonal mapping.
                                //
                                if (0 == CurXByte)
                                    NumBits = 0;        // this is the number that have been done
                                else
                                    NumBits = (CurXByte-1) * CHARSIZE + (CHARSIZE - Shift);   

                                NeededBits -= NumBits;          // NeededBits are how many are left to mask
                                NumBits = (CHARSIZE - Shift) > NeededBits?NeededBits:(CHARSIZE-Shift);
                                Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= 0xff<<(CHARSIZE - NumBits);      // wipe out excess data
                                Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= Residue | ((ObjPtr[ObjOff] & (0xff >> NumBits)) >> Shift);
                                //
                                // Figure the number of bits to save in the residue
                                ResidueBits = Shift > (NeededBits - (CHARSIZE - Shift))?(NeededBits - (CHARSIZE - Shift)):Shift;
                                ResidueBits += NumBits;
                                break;
                            }
                        }
                        // We want to fall through to the or here.  Do not put anything here.
                    case e_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_and:
                        //
                        // If we do an and the not used byts must be 1 not zero.  Change them here.
                        //
                        if (CurXByte == 0)
                            Residue = 0xff<<(CHARSIZE - Shift);
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_not_and:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    case e_not_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    case e_not_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    default:
                        break;
                }
                //
                // Get the stuff from the current byte that needs to be put in the next byte of the strip.
                //
                Residue = ((ObjPtr[ObjOff] & (0xff >> ResidueBits)) & (0xff >> (CHARSIZE - Shift))) << (CHARSIZE - Shift);
            }

            //
            // Write the last data into the buffer.
            //
            if (((int)LastBitMask - (int)Shift) > 0)    // if we go into the next word, get the bits.
            {
                if(no_bytes)
                {
                  // no bytes have been read/written, get the current offset
                  Residue |= ObjPtr[ObjOff]>>Shift;
                }
                else
                {
                  Residue |= ObjPtr[ObjOff+1]>>Shift;
                }
            } 
            
            if (LastBitMask)
            {
                switch (Operation)
                {
                    case e_diag_up:
                        SlantDirection = 1;
                    case e_diag_down:
                        {

                            // Figure the number of bits we need to set this up.
                            if (!SlantDirection)
                                NeededBits = DispObj->OBJ_Y_LEN - CurY;     // the number of bits needed at the start
                            else
                                NeededBits = CurY;

                            if (!CurrentXPtr && ((CurXByte == 0)
                                                 || ((CurXByte-1) * CHARSIZE + (CHARSIZE - Shift)) < NeededBits))
                            {
                                //
                                // Do the diagonal mapping.
                                //
                                if (0 == CurXByte)
                                    NumBits = 0;        // this is the number that have been done
                                else
                                    NumBits = (CurXByte-1) * CHARSIZE + (CHARSIZE - Shift);   

                                NeededBits -= NumBits;          // NeededBits are how many are left to mask
                                NumBits = (CHARSIZE - Shift) > NeededBits?NeededBits:(CHARSIZE-Shift);
                                Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= 0xff<<(CHARSIZE - NumBits);      // wipe out excess data
                                Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= Residue | ((ObjPtr[ObjOff] & (0xff >> NumBits)) >> Shift);
                                //
                                break;
                            }
                        }
                        // We want to fall through to the or here.  Do not put anything here.
                    case e_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= (Residue ) & ~(0xff >> (LastBitMask));
                        break;
                    case e_and:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= ((Residue ) & ~(0xff>> (LastBitMask))) | (0xff>>LastBitMask);
                        break;
                    case e_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= (Residue ) & ~(0xff>> (LastBitMask));
                        break;
                    case e_not_and:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] &= ~((Residue) & ~(0xff>> (LastBitMask)));
                        break;
                    case e_not_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= ~(Residue & ~(0xff>> (LastBitMask)));
                        break;
                    case e_not_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= ~(Residue & ~(0xff>> (LastBitMask)));
                        break;
                    default:
                        break;
                }
                Residue = 0;
            }
            //
            XLength -= XObjLen;
            CurrentXPtr += XObjLen;
        }
    }
}

/**
 * Function name: CopyLinkObj
 *
 * \brief Copy a linked object into strip.
 * This takes a graphic object object and puts the stuff into strip.
 *
 * Description: 
 *
 * \param Strip The destination strip for the data.
 * \param Object The Graphic object to put in the strip.
 * \param Plane The color of the plane to place the data.
 * \param DispObj The location info for the object.
 * \param Start Which row in strip to start putting the object.
 * \param Length How many rows to put in this strip.
 * \param ObjOffset Number of rows to space into the source buffer.
 * \param LineLength the length of a line in Strip.
 * \param XOffset How much to space in before putting down x values.  Should be 0 here.
 * \param BPP Number of bits per pixel.
 * 
 * Author: 
 *
 * Notes: 
 *
 **/
void CopyLinkObj(unsigned char *Strip, LINKGRAPHIC *Object, UINT32 Plane, DISP_LIST *DispObj, UINT32 Start, 
             UINT32 Length, UINT32 ObjOffset, UINT32 LineLength, UINT32 XOffset, UINT32 BPP,
                 uint8_t pixel_stride, uint8_t pixel_offset, rend_byte_type_t data_type)
{
    UINT32 Line, XStartByte, Shift, ObjOff, XLen, CurrentXPtr, XObjLen;
    SINT32 XLength;
    UINT32 CurXByte;
    UINT32 CurY;
    unsigned char Residue, *ObjPtr, LastBitMask;
    OBJ_OPERATION Operation;
    UINT32 NumBits, NeededBits, ResidueBits;
    UINT32 SlantDirection = 0;      // 1 slants up, 0 slants down
    LINK_MEMBER *CurrentLink;
    UINT32 LineStrip, StripeCount;

    //
    // Get the address of the plane to get the data from.
    //
    if(Object->PlaneRotated[Plane])
        CurrentLink = agGetHead(&Object->ROPlane[Plane]);
    else
        CurrentLink = agGetHead(&Object->InPlane[Plane]);
    //
    // Space down to the correct strip
    //
    LineStrip = CurrentLink->Param;
    while((LineStrip) < ObjOffset)
    {
        CurrentLink = CurrentLink->NextLink;
        ASSERT(CurrentLink != NULL);
        LineStrip += CurrentLink->Param;
    }
    //
    // We are now pointing to the stripe we need to start placing in this strip
    // Make sure the offset is relative to the start of this input stripe.
    //
    ObjOffset -= LineStrip - CurrentLink->Param;
    LineStrip = 0;                  // number of lines that are output
    StripeCount = ObjOffset;        // the count within the current stripe.
    ObjPtr = (unsigned char *) CurrentLink->Data;


    // change the operation based on the data type.  
    // The objects coming are 0 for clear, 1 for full intensity.  With rgb it is
    // the opposite so the operation must be changed to match.
    Operation = DispObj->Operation;

    for (Line = Start; Line < (Start + Length); Line++)
    {
        Residue = 0;      // the amount to carry into the next byte.
        CurrentXPtr = 0;
        if((StripeCount++) >= CurrentLink->Param)
        {
            //
            // We have moved on to another stripe.  Update everything.
            //
            LineStrip += CurrentLink->Param;
            CurrentLink = CurrentLink->NextLink;
            ASSERT(CurrentLink != NULL);
            ObjPtr = (unsigned char *) CurrentLink->Data;
            StripeCount = 1;
        }
        CurY = Line - Start + ObjOffset-LineStrip;         // find the line that we are working on.
        //
        // Find the length of the area to put in.
        //
        XLength = DispObj->OBJ_X_LEN*BPP;
        if (XLength == 0) 
            XLength = (SINT32) Object->X_DIMENSION * (SINT32) BPP;
        if ((XLength+DispObj->OBJ_X*BPP) > (LineLength * BPP * CHARSIZE))
            XLength = LineLength * CHARSIZE - DispObj->OBJ_X * BPP;
        while (XLength > 0)
        {
            //
            // Find out how much of the object to put down this time around.
            //
            XObjLen = XLength;
            //
            // Figure how much to shift the byte before doing the operation.
            //
            Shift = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset) % CHARSIZE;         // number of extra bits.
            //
            // Figure where we need to start in the strip.
            //
            XStartByte = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset)*pixel_stride / CHARSIZE + Line * LineLength*pixel_stride;
            //
            XLen = (XObjLen + Shift) / CHARSIZE;
            LastBitMask = (XObjLen + Shift) % CHARSIZE;

            //
            // Now put the bytes into the strip.
            //
            ObjOff = CurY * Object->LENGTH_X;           // must init here in case XLen is 0.
            for (CurXByte = 0; CurXByte < XLen; CurXByte++)
            {
                //
                // If I am in here I have at least 8 bits to put down.
                //
                ObjOff = CurY * Object->LENGTH_X + CurXByte;       // get the address of our buffer location
                ResidueBits = 0;
                SlantDirection = 0;
                //
                // Now put the bits into the strip.
                //
                switch (Operation)
                {
                    case e_diag_up:
                        SlantDirection = 1;
                    case e_diag_down:
                        {
                            // Figure the number of bits we need to set this up.
                            if (!SlantDirection)
                                NeededBits = DispObj->OBJ_Y_LEN - CurY;     // the number of bits needed at the start
                            else
                                NeededBits = CurY;

                            if (!CurrentXPtr && ((CurXByte == 0) || 
                                                 ((CurXByte-1) * CHARSIZE + (CHARSIZE - Shift)) < NeededBits))
                            {
                                //
                                // Do the diagonal mapping.
                                //
                                if (0 == CurXByte)
                                    NumBits = 0;        // this is the number that have been done
                                else
                                    NumBits = (CurXByte-1) * CHARSIZE + (CHARSIZE - Shift);   

                                NeededBits -= NumBits;          // NeededBits are how many are left to mask
                                NumBits = (CHARSIZE - Shift) > NeededBits?NeededBits:(CHARSIZE-Shift);
                                Strip[CurXByte*pixel_stride +XStartByte+pixel_offset] &= 0xff<<(CHARSIZE - NumBits);      // wipe out excess data
                                Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] |= Residue | ((ObjPtr[ObjOff] & (0xff >> NumBits)) >> Shift);
                                //
                                // Figure the number of bits to save in the residue
                                ResidueBits = Shift > (NeededBits - (CHARSIZE - Shift))?(NeededBits - (CHARSIZE - Shift)):Shift;
                                ResidueBits += NumBits;
                                break;
                            }
                        }
                        // We want to fall through to the or here.  Do not put anything here.
                    case e_or:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] |= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_and:
                        //
                        // If we do an and the not used byts must be 1 not zero.  Change them here.
                        //
                        if (CurXByte == 0)
                            Residue = 0xff<<(CHARSIZE - Shift);
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] &= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_xor:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] ^= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case e_not_and:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] &= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    case e_not_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    case e_not_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= ~(Residue | (ObjPtr[ObjOff]>>Shift));
                        break;
                    default:
                        break;
                }
                //
                // Get the stuff from the current byte that needs to be put in the next byte of the strip.
                //
                Residue = ((ObjPtr[ObjOff] & (0xff >> ResidueBits)) & (0xff >> (CHARSIZE - Shift))) << (CHARSIZE - Shift);

            }

            //
            // Write the last data into the buffer.
            //
            if (((int)LastBitMask - (int)Shift) > 0)    // if we go into the next word, get the bits.
                Residue |= ObjPtr[ObjOff+1]>>Shift;

            if (LastBitMask)
            {
                switch (Operation)
                {
                    case e_diag_up:
                        SlantDirection = 1;
                    case e_diag_down:
                        {

                            // Figure the number of bits we need to set this up.
                            if (!SlantDirection)
                                NeededBits = DispObj->OBJ_Y_LEN - CurY;     // the number of bits needed at the start
                            else
                                NeededBits = CurY;

                            if (!CurrentXPtr && ((CurXByte == 0)
                                                 || ((CurXByte-1) * CHARSIZE + (CHARSIZE - Shift)) < NeededBits))
                            {
                                //
                                // Do the diagonal mapping.
                                //
                                if (0 == CurXByte)
                                    NumBits = 0;        // this is the number that have been done
                                else
                                    NumBits = (CurXByte-1) * CHARSIZE + (CHARSIZE - Shift);   

                                NeededBits -= NumBits;          // NeededBits are how many are left to mask
                                NumBits = (CHARSIZE - Shift) > NeededBits?NeededBits:(CHARSIZE-Shift);
                                Strip[CurXByte+XStartByte] &= 0xff<<(CHARSIZE - NumBits);      // wipe out excess data
                                Strip[CurXByte + XStartByte] |= Residue | ((ObjPtr[ObjOff] & (0xff >> NumBits)) >> Shift);
                                //
                                break;
                            }
                        }
                        // We want to fall through to the or here.  Do not put anything here.
                    case e_or:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] |= (Residue ) & ~(0xff >> (LastBitMask));
                        break;
                    case e_and:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] &= ((Residue ) & ~(0xff>> (LastBitMask))) | (0xff>>LastBitMask);
                        break;
                    case e_xor:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] ^= (Residue ) & ~(0xff>> (LastBitMask));
                        break;
                    case e_not_and:
                        Strip[CurXByte*pixel_stride + XStartByte + pixel_offset] &= ~((Residue) & ~(0xff>> (LastBitMask)));
                        break;
                    case e_not_or:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] |= ~(Residue & ~(0xff>> (LastBitMask)));
                        break;
                    case e_not_xor:
                        Strip[CurXByte*pixel_stride+XStartByte+pixel_offset] ^= ~(Residue & ~(0xff>> (LastBitMask)));
                        break;
                    default:
                        break;
                }
                Residue = 0;        // clear out the residue for the next line.
            }
            //
            XLength -= XObjLen;
            CurrentXPtr += XObjLen;
        }
    }
}


/**
 * \brief return the number of pixels between different pixels.  
 * This is used to get how far to space to get to the next location
 * for the very next pixel.  This allows packing of the data.
 * \param[in] pack Enum specifying the packing of the data.
 * \returns uint8_t
 * \retval 1, 3, or 4 depending on packing.
 * asserts on failure.
 */
uint8_t get_pixel_stride(ipage_output_data_packing_t pack)
{
    uint8_t value = 1;
    switch(pack)
    {
        case IPAGE_PLANAR_INTERLEAVED:
        case IPAGE_PLANAR_ORDERED:
            value = 1;
            break;
        case IPAGE_RGB_PACK:
            value = 3;
            break;
        case IPAGE_RGBX_PACK:
        case IPAGE_XRGB_PACK:
            value = 4;
            break;
        default:
            ASSERT(0);
    }
    return value;

}
/**
 * \brief Return the offset within a packing to start the pixel placement
 * This is used to figure how many pixels to skip before starting to to store
 * pixels.  Handles packing of rgbx and xrgb
 * \param[in] pack An enum specifying the packing of the data.
 * \returns uint8_t 
 * \retval 0 or 1 depending on input
 * asserts on failure
 */
uint8_t get_pixel_offset(ipage_output_data_packing_t pack)
{
    uint8_t value = 0;
    switch(pack)
    {
        case IPAGE_PLANAR_ORDERED:
        case IPAGE_PLANAR_INTERLEAVED:
        case IPAGE_RGB_PACK:
        case IPAGE_RGBX_PACK:
            value = 0;
            break;
        case IPAGE_XRGB_PACK:
            value = 1;
            break;
        default:
            ASSERT(0);
    }
    return value;
}

/*This can be used to cancel an on-going rendereing*/
static int isCancelled = 0;
static int callerId = -1;
static int isBusy=0;
pthread_t renderer_tid;     // renderer thread id
static char *RendererStack = 0;

//#define RSTACK_SIZE 1024 * 12
#define RSTACK_SIZE (MAX((1024 * 12), POSIX_MIN_STACK_SIZE))

/*\brief Terminate the ongoing renderering.
 *
 * Call this function to terminate a ongoing renderering.  After calling function, wait until
 * renderer is no longer busy before any clean up.
 *
 */
void  terminateRenderering()
{
   isCancelled = 1;

}

/** Start the renderering and create the renderer thread if necessary
 *
 * This function needs to be called on every page.
 *
 * \param RendInput  The RENDER_INPUT that contains the renderering job.
 *
 * \param modId      Caller Module ID, renderer will send a MSG_RENDER_COMPLETE msg to this
 *                   msg queue when it is finished.
 *
 */
int startRenderer(RENDER_INPUT *RendInput, int modId)
{
    if(isBusy)
        return -1;

    if(!RendererStack)
    {
        RendererStack = MEM_MALLOC(RSTACK_SIZE);
        if(!RendererStack)
            return -1;
    }

    isBusy = 1;
    isCancelled = 0;
    //Remember who is the caller.
    callerId = modId;
    posix_create_thread(&renderer_tid, ((void* (*)(void*))Renderer), RendInput, "renderer", RendererStack, RSTACK_SIZE, POSIX_THR_PRI_NORMAL);

    return 0;
}

/**\brief Check to see if renderer is busy
 *
 * \return 0 Not busy, others Busy
 *
 * */
int isRendererBusy()
{
    return isBusy;
}
/**\brief Delete renderer thread.
 *
 *  Only call this function if you don't need render any more.
 *
 *
 */
int deleteRenderer()
{
    if(isBusy)
        return 1;

    ASSERT(RendererStack != NULL);
    if(!pthread_join(renderer_tid, NULL))
    {
        MEM_FREE_AND_NULL(RendererStack);
        //mark it as empty
        callerId= -1;

        // no error to return
        return 0;
    }

    return 1;
}


/**
 * Function name: Renderer
 *
 * \brief  Take a display list and mark a page and send to the printer.
 * When processing the display list there are 4 different types of graphics
 * allowed.
 * 1.  Graphic  A bit map block that is put at a specific position.  This also
 *          allows a streatching of the image.  It will simply duplicate the
 *          image any given amount of times to fill the streatch value.
 * 2.  Text  A string is passed to the renderer and the string font is placed 
 *          on the page.
 * 3.  DrawLine  Draw a line from x1,y1 to x2,y2.  
 * 4.  Linked graphics.  The object to place on the page consists of a linked
 *          list of data.
 * For Portrait printing the orgin is located in the upper left corner.  
 * For Landscape printing the orgin is located in the lower left corner.
 *
 *
 * \param RendInput A structure with renderer control information.
 *
 **/
void* Renderer(RENDER_INPUT *RendInput)
{
    DISP_LIST   *MyList, *TempPage;
    UINT32 iColor, Color;
    BOOL StringsPresent = FALSE;
    LINK_MEMBER *CurrentStrip[NUM_MARKING_COLOR_CHANNELS];
    UINT32 Rotate;
    PAGEINFO   Info;
    int i;
    uint32_t num_colors=0;
    bool strip_dirty = false;

//    BigBuffer_t* big_buffer = NULL;
    unsigned char* Strip = NULL;

    RendererSession RSession;
    RSession.PIXELLINE = RendInput->XSize/RendInput->BPP;
    Rotate = RendInput->Rotation;

    switch(RendInput->data_type)
    {
        case REND_DATA_MONO:
            num_colors = 1;
            break;
        case REND_DATA_RGB:
            num_colors = 3;
            break;
        case REND_DATA_CMYK:
            /* We need 6 color planes for "CMYK" due to the planes in the engine */
            num_colors = 6;
            break;
        default:
            ASSERT(0);
            break;
    }
    // Zero all structs that we pass around, so members that are not
    // explicitly set below will not contain total garbage.
    memset(&Info, 0, sizeof(PAGEINFO));

    // Setup the print structures 
    //
    Info.BitsPerPixel = RendInput->BPP;
    Info.PixelsPerLine = RSession.PIXELLINE;
    Info.Video_X = Info.PixelsPerLine;
    Info.Video_Y = RendInput->YSize;
    Info.X_Resolution = RendInput->X_Resolution;
    Info.Y_Resolution = RendInput->Y_Resolution;
    Info.copies = RendInput->Copies;
    Info.RawData = TRUE;
    Info.TonerSaveMode = FALSE;


    // 
    // See if we have any strings in the list.
    // This is done to see if we need to bother unpacking the fonts.
    // If there are no strings, we can then ignore the fonts.
    // Also look for line objects and clear the counters.
    MyList = RendInput->Page;
    while(MyList != NULL)
    {
        if(MyList->Type == e_Text)
        {
            StringsPresent = TRUE;
            MyList->OBJ_X_LEN = 0xFFFFFFFF;  //Mark the field as un-calculated starting pixel position for the text in the page
            MyList->OBJ_Y_LEN = 0xFFFFFFFF;  //Mark the field as un-calculated ending pixel position for the text in the page
            //break;      
        }
        //
        // if a line object, zero out its value.
        if(MyList->Type == e_DrawLine)
        {
            if(MyList->Object != NULL)
            {
                ((LINE *)MyList->Object)->DotCount = 0;
            }
        }
        MyList = MyList->NextObj;
    }
    if(StringsPresent)
    {
        internal_pages_font_init(RendInput->language, RendInput->X_Resolution, RendInput->Y_Resolution);
    }
   
    //
    //  Get the display list into MyList.  Sorted by y order might make things
    // faster.  Then after you are past the end of a item it can be removed.
    //
    MyList= RendInput->Page;
    //
    //
    RSession.YLen = RendInput->YSize;    // get the size of the input.
    //
    // If we are generating the stripe, set the output lines per stripe.
    //
    if (!RendInput->GenerateStripe)
    {
        //
        // Get the starting point of the output strips.
        //
        for(Color = 0; Color < NUM_MARKING_COLOR_CHANNELS; Color++)
        {
            CurrentStrip[Color] = agGetHead(&RendInput->Output[Color]);
        }
    }

    RSession.Dirty = 1;          // tells me if we have something on this page.
    //
    // Scan through the strips putting the objects into the strips.
    //
    iColor = 0;
    do
    {
        RSession.CurY = 0;                   // the line we are starting on.
        while (RSession.CurY < RendInput->YSize)
        {
            //
            // For each color put in the stuff.
            //
            do
            {
                switch(RendInput->color_order[iColor])
                {
                    case IPAGE_CYAN:
                        Color = COLOR_CYAN;
                        break;
                    case IPAGE_MAGENTA:
                        Color = COLOR_MAGENTA;
                        break;
                    case IPAGE_YELLOW:
                        Color = COLOR_YELLOW;
                        break;
                    default:
                        ASSERT( IPAGE_BLACK == RendInput->color_order[iColor] );
                        Color = COLOR_BLACK;
                        break;
                }

                //
                // Get a pointer to the output buffer.
                //
                if (!RendInput->GenerateStripe)
                {
                    if (RendInput->packing == IPAGE_PLANAR_INTERLEAVED ||
                        RendInput->packing == IPAGE_PLANAR_ORDERED)
                    {
                        //
                        // if this is planar packing then we get current settings.
                        ASSERT(CurrentStrip[Color] != NULL);
                        Strip = (unsigned char *) CurrentStrip[Color]->Data;    // get the pointer to the current strip.
                        ASSERT(Strip != NULL);
                    }
                    else
                    {
                        // This is packed.  We use the place located at color==0 for our packed plane
                        Strip = (unsigned char *) CurrentStrip[0]->Data;
                    }
                }
                else
                {
                    //
                    // Since we are generating the strip, allocate the memory and
                    // initialize it.
                    //
                    if (RSession.Dirty)
                    {
                        // if we are doing packed data we only allocate data the first time through.
                        if (((RendInput->packing == IPAGE_PLANAR_INTERLEAVED) || (RendInput->packing == IPAGE_PLANAR_ORDERED)) || 
                            (iColor == 0))
                        {
                            uint32_t strip_size = (RendInput->OutBufLPS * RSession.PIXELLINE* RendInput->BPP)/CHARSIZE;

                            do
                            {
                                Strip = (unsigned char *)MEM_MALLOC_ALIGN_LIMITED( mlimiter_by_name("ipage_print"), strip_size*get_pixel_stride(RendInput->packing),
                                                                                   cpu_get_dcache_line_size());
                                if(!Strip)
                                    posix_sleep_ms(20);
                            } while (Strip == NULL);
                            strip_dirty = false;
                            //
                            // Clear out the strip
                            //
                            if (RendInput->data_type == REND_DATA_RGB)
                            {
                                memset(Strip, 0xff, strip_size*get_pixel_stride(RendInput->packing));
                            }
                            else
                            {
                                memset (Strip,0,strip_size*get_pixel_stride(RendInput->packing));
                            }

                            RSession.Dirty = 1;
                        }
                    }
                }
                //
                // Initialize the pointers to the boundry of the strip.
                //
                RSession.CurX = 0;

                TempPage = MyList;  // initialize the display list pointer.
                //
                // Now scan through each object seeing if any part of that object is on
                // this strip.  If it is put that part in.
                //

                while (TempPage != NULL) // When to end this rendering, when done!
                {
                    i = 0;
                    while (renderers[i].renderer||renderers[i].type)
                    {
                        if (renderers[i].renderer != NULL)
                        {

                            if (renderers[i].type == TempPage->Type && renderers[i].renderer)
                            {
                                if (!isCancelled)
                                    renderers[i].renderer(&RSession,Strip, TempPage, RendInput, Color, Rotate, &strip_dirty, iColor);
                                break;
                            }
                        }
                        i++;
                    }
                    //
                    // Go to the next object in the display list.
                    //
                    TempPage = TempPage->NextObj;
                }
                //
                // At this point everything that goes into this stripe is there.
                // Decide what to do with the stripe.
                //
                if (RendInput->GenerateStripe)
                {
                    // if we are packing we hold off sending the strip till we have finished all
                    // the colors.
                    //
                    if (((RendInput->packing == IPAGE_PLANAR_INTERLEAVED) ||
                         (RendInput->packing == IPAGE_PLANAR_ORDERED)) ||
                        (iColor == (num_colors-1)))
                    {
                        //
                        // The stripe is finished, send it to the engine.
                        //
                        STRIPINFO stripInfo;

                        // Zero all structs that we pass around, so members that are not
                        // explicitly set below will not contain total garbage.
                        memset(&stripInfo, 0, sizeof(STRIPINFO));

                        stripInfo.color = Color;
                        DPRINTF( (DBG_LOUD|DBG_OUTPUT) ,("Rend color %d\n",Color));
                        if (RendInput->packing != IPAGE_PLANAR_ORDERED &&
                            RendInput->packing != IPAGE_PLANAR_INTERLEAVED)
                        {
                            stripInfo.color = 0;  // for rgb data pass it in the k plane.
                        }

                        stripInfo.lastStrip = ((RSession.CurY + RendInput->OutBufLPS) >=(RendInput->YSize))?TRUE:FALSE;
                        stripInfo.numLines = RendInput->OutBufLPS;

                        if(RendInput->data_type == REND_DATA_RGB)
                        {
                            if ( RendInput->packing == IPAGE_RGB_PACK )
                            {
                                stripInfo.stripDataType = PACKED_RGB;
                            }
                            else if ( RendInput->packing == IPAGE_XRGB_PACK )
                            {
                                stripInfo.stripDataType = PACKED_XRGB;
                            }
                            else
                            {
                                ASSERT( RendInput->packing == IPAGE_RGBX_PACK );
                                stripInfo.stripDataType = PACKED_RGBX;
                            }
                        }
                        else
                        {
                            stripInfo.stripDataType = GRAYSCALE;
                        }
                        stripInfo.bufSize = ((RendInput->OutBufLPS * RSession.PIXELLINE* RendInput->BPP)/CHARSIZE) *get_pixel_stride(RendInput->packing);

                        if(RSession.Dirty && strip_dirty)
                        {
                            stripInfo.big_buffer = dma_buffer_adopt( Strip, stripInfo.bufSize );
                        }
                        else
                        {
                            /* use big_buffer to know if strip is empty or not */
                            stripInfo.big_buffer = NULL;
                            // Free the strip data allocation because it has nothing on it.
                            MEM_FREE_AND_NULL(Strip);
                        }

                        stripInfo.compType = PLANEDATA_COMPTYPE_NONE;
                        stripInfo.X_Resolution = Info.X_Resolution; 
                        stripInfo.Y_Resolution = Info.Y_Resolution; 

                        Info.copies = RendInput->Copies;
                        SendStrip(&Info, &stripInfo, RendInput->Dest, mlimiter_by_name("ipage_print"));
                    }
                }
                else
                {
                    if (RendInput->packing == IPAGE_PLANAR_INTERLEAVED ||
                        RendInput->packing == IPAGE_PLANAR_ORDERED)
                    {
                        //
                        // We are not sending the stripe to the engine so we 
                        // Go to the next stripe to fill.
                        //
                        CurrentStrip[Color] = CurrentStrip[Color]->NextLink;
                        // an assert above checks to insure this is valie.
                    }
                    if (((RendInput->packing != IPAGE_PLANAR_INTERLEAVED) &&
                         (RendInput->packing != IPAGE_PLANAR_ORDERED)) &&
                        (iColor == num_colors-1))
                    {
                        // packed are always stored in 0
                        CurrentStrip[0] = CurrentStrip[0]->NextLink;
                    }
                }
            } while (RendInput->packing != IPAGE_PLANAR_ORDERED && ++iColor < num_colors);
            // Update the number of lines.
            RSession.CurY += RendInput->OutBufLPS;   
            if(RendInput->packing != IPAGE_PLANAR_ORDERED)
                iColor = 0;
        }
        //
        // if we are not doing ordered, the colors start over here.
    } while (++iColor < num_colors && RendInput->packing == IPAGE_PLANAR_ORDERED);
    //
    // The page is finished, clean up everything.
    //

    //Calling each object render one more time with NULL object to force a cleaning up.
    i = 0;
    while(renderers[i].renderer || renderers[i].type)
    {
          if(renderers[i].renderer)
              renderers[i].renderer(&RSession,Strip, 0, RendInput,0, Rotate, &strip_dirty, iColor);
          i++;
    }

    // If we rotated into some memory, release it.  This is currently looking
    // for rotations done for the linkgraphic type.  So scan through the display
    // list looking for any linkgraphis type objects and release their
    // rotated memory.
    // 
    TempPage = MyList;
    while(TempPage != NULL)
    {
        if(TempPage->Type == e_LinkGraphics)
        {
            LINKGRAPHIC *TempGraphic;

            TempGraphic = (LINKGRAPHIC *) TempPage->Object;
            if(TempGraphic->PlaneRotated[0] == TRUE)
            {
                LINK_MEMBER *TempLink, *TheLink;
                TempLink = agGetHead(&TempGraphic->ROPlane[0]);
                while(TempLink != NULL)
                {
                    TheLink = TempLink;
                    TempLink = TempLink->NextLink;
                    MEM_FREE_AND_NULL(TheLink);
                }
            }
            TempGraphic->PlaneRotated[0] = FALSE;
        }
        TempPage = TempPage->NextObj;

    }
    //
    // If the strip was not used, release it.
    //
    if(RendInput->GenerateStripe)
    {
        if (RSession.Dirty == 0)
        {
            MEM_FREE_AND_NULL(Strip);
        }
    }
    if(StringsPresent)
        internal_pages_font_halt();
    isBusy = 0;
    return 0;
}
 





/**
 * \brief renderer for graphic objects
 * Take a graphic block and copy it into the strip at a given location.
 * \param[in] pRSession Session information
 * \param[in] TempPage The information on the graphic object
 * \param[in] Strip The strip in which to place the data.
 * \param[in] RendInput Information on the strip, size, resolution etc.
 * \param[in] Color Which color plane to place the object
 * \param[in] Rotate true if landscape, false if portrait.
 * \param[out] strip_dirty true if the strip was modified, false otherwise.
 * \returns void 
 *
 */
void GraphicsRenderer(RendererSession *pRSession, 
                      unsigned char *Strip, 
                      DISP_LIST *TempPage, 
                      RENDER_INPUT *RendInput, 
                      UINT32 Color, 
                      UINT32 Rotate, 
                      bool *strip_dirty,
                      uint32_t color_index )
{
    UINT32 Offset;  // Offset in pixels, is how far into the graphic to index when starting the copy.
    int StartY, Len;
    uint32_t pixel_offset = get_pixel_offset(RendInput->packing);
    // 
    // if we are packed we have to add in the color for correct packing.
    if(RendInput->packing != IPAGE_PLANAR_INTERLEAVED &&
       RendInput->packing != IPAGE_PLANAR_ORDERED)
    {
        pixel_offset += color_index;
    }

    if (!TempPage)
        return;

    // see if there is anything in this graphic.
    //
    if (((GRAPHIC *)TempPage->Object)->LENGTH_X == 0)
        return;

    // First see if there is anything from this object on this color plane.
    //
    if (*(((UINT32 *)(&((GRAPHIC *)TempPage->Object)->K_OR_R_PLANE))+Color) == 0)
        return;      // nothing on this plane, go on to the next object.
    if (!Rotate)
    {
        // Non-rotated graphics.
        //
        // See if this object is in this strip, check the beginning
        //
        if (TempPage->OBJ_Y >= (pRSession->CurY + RendInput->OutBufLPS))
            return;
        //
        // See if the end of the object is before this stripe
        //    
        if (TempPage->OBJ_Y_LEN == 0)
        {
            if ((((((GRAPHIC *) TempPage->Object)->Y_DIMENSION + TempPage->OBJ_Y) <= pRSession->CurY)))
                return;
        }
        else
        {
            if ((((TempPage->OBJ_Y_LEN + TempPage->OBJ_Y) <= pRSession->CurY)))
                return;
        }
        // 
        // Some part of this object is in this strip 
        // figure out how much of the object and where in the strip it goes.
        //
        // Assume the start of the object is in the strip.
        StartY = TempPage->OBJ_Y;
        Offset = 0;
        //
        // Now see if the start is before the stripe, if so adjust.
        if(StartY < pRSession->CurY)
        {
            // start is before the stripe, adjust
            Offset = pRSession->CurY - StartY;
            StartY = pRSession->CurY;
        }
        //
        // The length is determined by if we are stretching or just the object length
        //
        // This assumes the end of the object is in the stip.
        //
        if(TempPage->OBJ_Y_LEN == 0)
        {
            Len = ((GRAPHIC *)TempPage->Object)->Y_DIMENSION - Offset;
        } else
        {
            Len = TempPage->OBJ_Y_LEN - Offset;
        }
        //
        // See if the end is past the end of the strip, if so adjust the length
        //
        if((StartY + Len) > (pRSession->CurY + RendInput->OutBufLPS))
        {
            Len = pRSession->CurY + RendInput->OutBufLPS - StartY;
        }
        // StartY is relative to the start of the page.  Want it relative to the
        // start of the strip.  Make that adjustment here.
        //
        StartY -= pRSession->CurY;

        //
        // We have now figured out the location of the object, copy it to the stripe.
        //
        pRSession->Dirty = 1;   // since we got here the strip will be dirty
        *strip_dirty = true;

        //
        // Put the object into the stripe.
        //
        CopyObj(Strip, (GRAPHIC *)TempPage->Object, Color, TempPage, StartY, Len, Offset, 
                (pRSession->PIXELLINE*RendInput->BPP),0, RendInput->BPP,
                get_pixel_stride(RendInput->packing), pixel_offset, RendInput->data_type);
        return;
    }
    else
    {
        // Rotated graphics.
        //
        // See if this object is in this strip, check the beginning
        //
        if (((pRSession->YLen - TempPage->OBJ_X - TempPage->OBJ_X_LEN) > (pRSession->CurY + RendInput->OutBufLPS)))
            return;
        //
        // See if the end of the object is before this stripe
        //    
        if(TempPage->OBJ_X_LEN == 0)
        {
            if ((((GRAPHIC *) TempPage->Object)->X_DIMENSION + (pRSession->YLen - TempPage->OBJ_X))<= pRSession->CurY)
                return;
        }
        else
        {
            if ((pRSession->YLen - TempPage->OBJ_X) <= pRSession->CurY)
                return;
        }
        // 
        // Some part of this object is in this strip 
        // Figure out how much of the object is in the strip and where it goes.
        //
        // First assume the start of the object is in the strip
        //
        StartY = (pRSession->YLen - TempPage->OBJ_X) - pRSession->CurY;
        Offset = 0;
        // Now check the assumption if not correct adjust.
        if(StartY < pRSession->CurY)
        {
            StartY = pRSession->CurY;
            Offset = pRSession->CurY - StartY;
        }
        //
        // Now set up the length, first assume the end of the object is in this strip
        //
        if (TempPage->OBJ_X_LEN == 0)
        {
            //
            // The graphic is not stretched here.
            //
            Len = ((GRAPHIC *) TempPage->Object)->X_DIMENSION - (pRSession->CurY - (pRSession->YLen - TempPage->OBJ_X));
        }
        else
        {
            //
            // The graphic is stretched.
            //
            Len = pRSession->CurY - (pRSession->YLen - TempPage->OBJ_X- TempPage->OBJ_X_LEN);
        }
        //
        // Now see if the assumption above is correct, if not fix it.
        //
        if((StartY + Len) > (pRSession->CurY + RendInput->OutBufLPS))
        {
            Len = pRSession->CurY + RendInput->OutBufLPS - StartY;
        }

        //
        // Make the numbers relative to the start of the stripe.
        // We don't care where we are on the page,
        // only where we are in the stripe.
        // bias back to stripe relative.
        //
        StartY -=  pRSession->CurY;
        //
        // We have now figured out the location of the object, copy it to the stripe.
        //
        pRSession->Dirty = 1;
        *strip_dirty = true;

        GRAPHIC *TempGraphic;
        DISP_LIST *TempDispList;

        //
        // This is a rotated graphic.  We must change the
        // x and y parameters to rotate the image.
        //
        UINT32 Size;
        LINK_MEMBER InLink, OutLink;

        Size = ((GRAPHIC *) TempPage->Object)->Y_DIMENSION/8;  // x size of object
        if ((((GRAPHIC *)TempPage->Object)->Y_DIMENSION % 8) != 0)
            Size++;
        //
        //  Now make the buffer 4 byte aligned.
        // 
        if((Size % 4) != 0)
            Size = (Size & 0xfffffffc) + 4;


        TempGraphic = (GRAPHIC *) MEM_CALLOC(1, ((GRAPHIC *)TempPage->Object)->X_DIMENSION  * 
                                           Size+sizeof(GRAPHIC));
        ASSERT(TempGraphic);
        TempGraphic->Y_DIMENSION = ((GRAPHIC *)TempPage->Object)->X_DIMENSION;
        TempGraphic->X_DIMENSION = ((GRAPHIC *)TempPage->Object)->Y_DIMENSION;
        TempGraphic->LENGTH_X = Size;
        *((UINT32 *)(((UINT32)(&TempGraphic->K_OR_R_PLANE)) + Color)) = 
        (UINT32)(((UINT32) TempGraphic) + sizeof(GRAPHIC));
        InLink.NextLink = NULL;
        InLink.Param = 100000;      // everything is in this stripe
        InLink.Data = *(&(((GRAPHIC *)TempPage->Object)->K_OR_R_PLANE)+ Color);
        OutLink.NextLink = NULL;
        OutLink.Data = (void *) *((UINT32 *)((&TempGraphic->K_OR_R_PLANE) + Color));
        // Now rotate the graphic image.
        //
        rotate(TempGraphic->Y_DIMENSION, TempGraphic->X_DIMENSION, 
                 ((GRAPHIC *)TempPage->Object)->LENGTH_X, &InLink, &OutLink, 
                 RendInput->BPP,RendInput->ImagesRotation);
        // build up a display list.
        TempDispList = MEM_CALLOC(1, sizeof(DISP_LIST));
        ASSERT(TempDispList);
        TempDispList->OBJ_X = pRSession->PIXELLINE - TempPage->OBJ_Y - TempPage->OBJ_Y_LEN;  // PIXELLINE moves the origin.
        TempDispList->OBJ_Y = pRSession->YLen - TempPage->OBJ_X - TempPage->OBJ_X_LEN;
        TempDispList->OBJ_X_LEN = TempPage->OBJ_Y_LEN;
        TempDispList->OBJ_Y_LEN = TempPage->OBJ_X_LEN;
        TempDispList->Operation = TempPage->Operation;
        //
        // Put the object into the stripe.
        //
        CopyObj(Strip, TempGraphic, Color, TempDispList, StartY, Len, Offset, (pRSession->PIXELLINE*RendInput->BPP),0, RendInput->BPP,
                get_pixel_stride(RendInput->packing), pixel_offset, RendInput->data_type);
        MEM_FREE_AND_NULL(TempGraphic);
        MEM_FREE_AND_NULL(TempDispList);
        return;

    }
}





/**
 * \brief Line object renderer
 * Take a line object and draw in the strip the line requested.
 * \param[in] pRSession The session info 
 * \param[in] TempPage The display list entry for the line draw.
 * \param[in] RendInput The paramaters on strip etc.
 * \param[in] Color The color plane on which to place the data.
 * \param[in] Rotate true to place rotated, else not.  Used for landscape printing.
 * \param[out] strip_dirty true if the strip was modified, false otherwise.
 *
 */
void LineRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            UINT32 Color, 
                            UINT32 Rotate,
                            bool *strip_dirty,
                            uint32_t color_index)
{
    SINT32 LineNumber;
    UINT32 LineWidth, MyLineWidth;
    UINT32 pixel_stride = get_pixel_stride(RendInput->packing);
    UINT32 pixel_offset = get_pixel_offset(RendInput->packing);
    SINT32 X1, Y1, X2, Y2;
    SINT32 TX1,TY1,TX2,TY2;
    FLOAT Slope;
    SINT32 MyX=0, X=0, NextX=0;


    if( NULL == TempPage )
    {
        return;
    }

    if( NULL != (LINE *)TempPage->Object )
    {
        if(!((1<<Color) & ((LINE *)TempPage->Object)->Color))
            return;
    }
    //
    // if this is packed, put the color value into the offset
    //
    if(RendInput->packing != IPAGE_PLANAR_ORDERED &&
       RendInput->packing != IPAGE_PLANAR_INTERLEAVED)
    {
        pixel_offset += color_index;
    }
    //
    // Check the 4 cases.
    // 1.  Not in stripe
    // 2.  Contained within stripe
    // 3.  Begininning before stripe
    // 4.  End out of stripe.
    // For each case adjust the x1,x2,y1,y2 parameters 
    // to match.

    //
    // get the x and y values in order from the disp object
    //
    if (!Rotate)
    {
        TY1 = TempPage->OBJ_Y;
        TX1 = TempPage->OBJ_X;
        TY2 = TempPage->OBJ_Y_LEN;
        TX2 = TempPage->OBJ_X_LEN;
        //
        // We want the values in order of high to low. 
        // do that here.
        if (TY1 > TY2)
        {
            // Swap the values.
            TY1 = TY2;
            TX1 = TX2;
            TY2 = TempPage->OBJ_Y;
            TX2 = TempPage->OBJ_X;
        }

    }
    else
    {
        //
        // Rotated image, adjust the x and y
        // values to match what they need to be for landscape.
        TY1 = pRSession->YLen - TempPage->OBJ_X;
        TX1 = pRSession->PIXELLINE - TempPage->OBJ_Y;  // where orgin is moved from upper l to lower l.
        TY2 = pRSession->YLen - TempPage->OBJ_X_LEN;
        TX2 = pRSession->PIXELLINE - TempPage->OBJ_Y_LEN;
        //
        // Make sure the order is correct.
        if (TY1 > TY2)
        {
            TY1 = TY2;
            TX1 = TX2;
            TY2 = pRSession->YLen - TempPage->OBJ_X;
            TX2 = pRSession->PIXELLINE - TempPage->OBJ_Y;
        }
    }
    //
    // Check to make sure we are within range.
    //
    if (TX1 < 0) TX1 = 0;
    if (TX2 < 0) TX2 = 0;
    if (TY1 < 0) TY1 = 0;
    if (TY2 < 0) TY2 = 0;
    if (TX1 >  pRSession->PIXELLINE) TX1 =  pRSession->PIXELLINE;
    if (TX2 >  pRSession->PIXELLINE) TX2 =  pRSession->PIXELLINE;
    if (TY1 > pRSession->YLen) TY1 = pRSession->YLen;
    if (TY2 > pRSession->YLen) TY2 = pRSession->YLen;
    //
    // To put down different thickness lines, write LineWidth number
    // of lines next to each other.  To do this we set the starting
    // point by finding a line perpendicular to our current line,
    // and going a single pixel in that direction and set the
    // starting point.  The other end copies the offsets set
    // in the first point.
    //
    // Figure out the width of the line
    //
    if(TempPage->Object == NULL)
        MyLineWidth = 1;
    else
        MyLineWidth = ((LINE *)TempPage->Object)->LineWidth;

    for (LineWidth = 0;LineWidth < MyLineWidth; LineWidth++)
    {
        float B;        // this is the y intercept
        float NewSlope; // This is the slope of the perp line
        UINT32 PixCount=0;
        //
        // Figure the slope of the line to place.
        //
        Slope = ((float) TY2 - (float) TY1)/((float) TX2 - (float)TX1);
        //
        // Figure the slope of a line perpendicular to our line
        NewSlope = -1/Slope;    // the slope of a perp line.
        //
        // Special cases are where the line is vertical and
        // horzointal.  Take care of the vertical case here.
        if(TX2 == TX1)
            NewSlope = 0.0;
        //
        // Figure the Y intercept for our perpendicular line.
        // The line equation I am using is y = mx + b
        B = TY1 - NewSlope * TX1;  // B = y - mx
        X1 = TX1 + LineWidth;   // space x direction.
        //
        // if y1 = y2 is the 2nd special case of horziontal lines.
        if (TY1 != TY2)
        {
            //
            // Find the new Y by solving the line equation.
            // the .5 is added for rounding. again y = mx+b
            //
            Y1 = NewSlope * X1 + B + 0.5;
            //
            // Limit the offset to the linewidth.
            if (abs(Y1- TY1) > LineWidth)
            {
                //
                // Go in the correct direction for the line.
                //
                if(Y1 > 0)
                    Y1 = TY1 + LineWidth;
                else
                    Y1 = TY1 - LineWidth;
            }
        } else
        {
            // The line is horziontal, move vertical
            Y1 = TY1 + LineWidth;
            X1 = TX1;
        }

        //
        // Offset the other point by the same amount.
        //
        X2 = TX2 + (X1 - TX1);
        Y2 = TY2 + (Y1 - TY1);

        //
        // See if the object is in this stripe.
        if ((Y2 < pRSession->CurY) || (Y1 >= (pRSession->CurY + (int)RendInput->OutBufLPS)))
            return;      // all done. object before stripe.

        // We have decided this object has something within this stripe.
        //
        // Now see if the beginning is before the stripe.
        // and adjust the x1,y1 values to be the
        // values at the stripe boundry.
        //
        if (Y1 < pRSession->CurY)
        {
            // The beginning is before.  Adjust the parameters.
            X1 = (pRSession->CurY - Y1) / Slope + X1;
            Y1 = pRSession->CurY;
        }
        //
        // Now see if the end is past the end of the stripe.
        // and adjust the values to be correct for the end
        // of the stripe.
        //
        if (Y2 > (pRSession->CurY+RendInput->OutBufLPS))
        {
            // we are past the end of the stripe.
            // We ignore X2, since it is not used.
            //
            Y2 = pRSession->CurY + RendInput->OutBufLPS -1;
        }
        //
        // If we are putting down a single dot, make the slope 0
        // so it will work correctly.
        //
        if ((Y1 == Y2) && (X1 == X2))
            Slope = 0;

        if((TempPage->Object != NULL) &&
           (((LINE *)TempPage->Object)->LineObj != NULL))
            PixCount = ((LINE *)TempPage->Object)->DotCount;

        //
        // Now put the line into the stripe.
        // If we have a slope that is very steep we
        // have to set many bits on a given line.
        //

        for (LineNumber = Y1 - pRSession->CurY; LineNumber <= Y2 - pRSession->CurY; LineNumber++)
        {
            UINT32 PixelValue;
            uint32_t array_index, mask_value;

            //
            // Calculate the number of bits to
            // set on this line.
            //
            if (Slope == 0)
            {
                MyX = X1;
                NextX = X2;
            }
            else
            {
                MyX = X1 + (LineNumber - Y1 + pRSession->CurY) / Slope;
                NextX = X1 + (LineNumber + 1 - Y1 + pRSession->CurY) / Slope;
            }
            // See which one is bigger and swap if needed.
            if (MyX > NextX)
            {
                X = NextX;
                NextX = MyX;
                MyX = X;
            }
            //
            // Figure the amount to put down with each pixel
            // 
            PixelValue = 0xff >> (8 - RendInput->BPP);
            //
            // Now we need to set all the bits from MyX to
            // NextX on this line.
            //
            // If they are the same then we want at least one set so increment nextx
            // if this is not done there are holes in many lines.
            //
            if(MyX == NextX)
                NextX++;
            for (X = MyX; X < NextX; X++)
            {
                //
                // Pixel value shift of value is this:
                // pixel location   shift required  bpp
                //  0                   7           1
                //  1                   6           1
                //  2                   5           1
                // etc
                //  0                   6           2
                //  1                   4           2
                //  2                   2           2
                //  3                   0           2

                //  0                   4           4
                //  1                   0           4
                //  0                   0           8
                // so figure the shif required does (8-bpp) - ((pixelloc * bpp)%8)
                mask_value = (PixelValue<<((8 - RendInput->BPP) - ((UINT32)(X * RendInput->BPP) % 8)));
                // select the correct byte in the strip to make the changes.
                array_index = (UINT32)((LineNumber *  (pRSession->PIXELLINE*RendInput->BPP)/CHARSIZE + 
                                       (X * RendInput->BPP) / CHARSIZE)) * pixel_stride + pixel_offset;
                if((TempPage->Object == NULL) ||
                   (((LINE *)TempPage->Object)->LineObj == NULL))
                {
                    if(RendInput->data_type == REND_DATA_RGB)
                    {

                        Strip[array_index] &= ~mask_value;
                    } else
                    {
                        Strip[array_index] |= mask_value;
                    }

                    PixCount++;

                } else
                {
                    UINT8 Value;
                    // 
                    // Put down a patterened line.
                    //
                    Value = (((LINE *)TempPage->Object)->LineObj->Template)[PixCount/8];
                    if(Value & (1<<(8-(PixCount % 8))))
                    {
                        if(RendInput->data_type == REND_DATA_RGB)
                        {
                            Strip[array_index] &= ~mask_value;
                        } else
                        {
                            Strip[array_index] |= mask_value;
                        }
                    }

                    PixCount++;
                    if(PixCount >= ((LINE *)TempPage->Object)->LineObj->TemplateLength)
                        PixCount = 0;
                }
            }
        }   // the line is finished, do the next line.
        pRSession->Dirty = 1;
        *strip_dirty = true;
    }

    if(TempPage->Object != NULL)
    {
        ((LINE *)TempPage->Object)->DotCount += NextX - MyX +1;
        if((((LINE *)TempPage->Object)->LineObj != NULL) &&((LINE *)TempPage->Object)->DotCount >= ((LINE *)TempPage->Object)->LineObj->TemplateLength)
            ((LINE *)TempPage->Object)->DotCount %= ((LINE *)TempPage->Object)->LineObj->TemplateLength;
    }

    return;

}


/**
 * \brief Link Object Renderer 
 * A link object is a raster blob that is presented as a series of strips.  
 * This usually happens when we are placing scanned images onto a page.
 * So this render works like the graphic render except it takes a striped
 * input image.
 * \param[in] pRSession Session information
 * \param[in/out] Strip The data block into which data is to be placed.
 * \param[in] TempPage Info on the graphic object to place.
 * \param[in] RendInput Info on the rendering progress, size, etc.
 * \param[in] Color The color plane on which to place the data
 * \param[in] Rotate if true, do landscape printing, else portrait.
 * \param[out] strip_dirty true if the strip was modified, false otherwise.
 * \returns void
 *
*/ 
void LinkGraphicsRenderer(RendererSession *pRSession, 
                        unsigned char *Strip, 
                        DISP_LIST *TempPage, 
                        RENDER_INPUT *RendInput, 
                        UINT32 Color, 
                        UINT32 Rotate,
                        bool *strip_dirty,
                        uint32_t color_index)
{

    UINT32 XDim,YDim;
    LINKGRAPHIC TempGraphic;
    int Len, StartY, Offset;

    //dbg_printf("LinkGraphicsRenderer...\n");

    if(!TempPage)
        return;
    // see if there is anything in this graphic.
    //
    if (((LINKGRAPHIC *)TempPage->Object)->LENGTH_X == 0)
        return;
    //
    // Get the correct x and y dimensions.
    XDim = ((LINKGRAPHIC *)TempPage->Object)->X_DIMENSION;
    YDim = ((LINKGRAPHIC *)TempPage->Object)->Y_DIMENSION;
    if(RendInput->ImagesRotation != e_NoRotate)
    {
        XDim = ((LINKGRAPHIC *)TempPage->Object)->Y_DIMENSION;
        YDim = ((LINKGRAPHIC *)TempPage->Object)->X_DIMENSION;
                                
    }

    // Offset is how far into the graphic to index when starting the copy.
    //

    // First see if there is anything from this object on this color plane.
    //
    if (((LINKGRAPHIC *)TempPage->Object)->InPlane[Color].LinkHead == NULL)
        return;      // nothing on this plane, go on to the next object.


    if (!Rotate)
    {
        // Non-rotated graphics.
        //
        // See if this object is in this strip, check the beginning
        //
        if (TempPage->OBJ_Y >= (pRSession->CurY + RendInput->OutBufLPS))
            return;
        //
        // See if the end of the object is before this stripe
        //    
        if (TempPage->OBJ_Y_LEN == 0)
        {
            if ((YDim + TempPage->OBJ_Y) <= pRSession->CurY)
                return;
        }
        else
        {
            if ((((TempPage->OBJ_Y_LEN + TempPage->OBJ_Y) <= pRSession->CurY)))
                return;
        }
        // 
        // Some part of this object is in this strip put it there.
        //
        pRSession->Dirty = 1;
        *strip_dirty = true;
        StartY = TempPage->OBJ_Y;
        Offset = 0;
        //
        // Now see if the start is before the stripe, if so adjust.
        if(StartY < pRSession->CurY)
        {
            // start is before the stripe, adjust
            Offset = pRSession->CurY - StartY;
            StartY = pRSession->CurY;
        }
        //
        // The length is determined by if we are stretching or just the object length
        //
        if(TempPage->OBJ_Y_LEN == 0)
        {
            Len = ((GRAPHIC *)TempPage->Object)->Y_DIMENSION - Offset;
        } else
        {
            Len = TempPage->OBJ_Y_LEN - Offset;
        }
        //
        // See if we overflowed the strip length, if so adjust
        //
        if((StartY + Len) > (pRSession->CurY + RendInput->OutBufLPS))
        {
            Len = pRSession->CurY + RendInput->OutBufLPS - StartY;
        }
        // StartY is relative to the start of the page.  Want it relative to the
        // start of the strip.  Make that adjustment here.
        //
        StartY -= pRSession->CurY;
    }
    else
    {
        // Rotated graphics.
        //
        // See if this object is in this strip, check the beginning
        //
        if (((pRSession->YLen - TempPage->OBJ_X - TempPage->OBJ_X_LEN) > (pRSession->CurY + RendInput->OutBufLPS)))
            return;
        //
        // See if the end of the object is before this stripe
        //    
        if(TempPage->OBJ_X_LEN == 0)
        {
            if ((XDim + (pRSession->YLen - TempPage->OBJ_X))<= pRSession->CurY)
                return;
        }
        else
        {
            if ((pRSession->YLen - TempPage->OBJ_X) <= pRSession->CurY)
                return;
        }
        // 
        // Some part of this object is in this strip put it there.
        //
        pRSession->Dirty = 1;
        *strip_dirty = true;
        StartY = (pRSession->YLen - TempPage->OBJ_X) - pRSession->CurY;
        Offset = 0;
        if(StartY < pRSession->CurY)
        {
            StartY = pRSession->CurY;
            Offset = pRSession->CurY - StartY;
        }

        // Here we know that the graphic starts before the stripe and the end
        // is in the stripe.
        //
        if (TempPage->OBJ_X_LEN == 0)
        {
            //
            // The graphic is not stretched here.
            //
            Len = XDim - (pRSession->CurY - (pRSession->YLen - TempPage->OBJ_X));
        }
        else
        {
            //
            // The graphic is stretched.
            //
            Len = pRSession->CurY - (pRSession->YLen - TempPage->OBJ_X- TempPage->OBJ_X_LEN);
        }

        if((StartY + Len) > (pRSession->CurY + RendInput->OutBufLPS))
        {
            Len = pRSession->CurY + RendInput->OutBufLPS - StartY;
        }

        //
        // Make the numbers relative to the start of the stripe.
        // We don't care where we are on the page,
        // only where we are in the stripe.
        // bias back to stripe relative.
        //
        StartY -=  pRSession->CurY;
    }
    
    // If the object needs to be rotated, do it here.
    // This will save the rotated image so that we only need to rotate it once.
    //
    if (!(((LINKGRAPHIC *)TempPage->Object)->PlaneRotated[Color]) && (RendInput->ImagesRotation != e_NoRotate))
    {
	//dbg_printf("=-=-=- Rotate image...\n");

        UINT32 Rx,Ry;
        LINKGRAPHIC *MyGraphic;

        MyGraphic = (LINKGRAPHIC *) TempPage->Object;
        // Rotate the image.

        MyGraphic->RotLPS = 100;      // do 100 lines per stripe.
        //
        // Allocate room for the rotation.
        //
        for (Ry = 0; Ry < NUM_MARKING_COLOR_CHANNELS; Ry++)
            agLinkInit(&MyGraphic->ROPlane[Ry]);

        Rx = (XDim * RendInput->BPP)/CHARSIZE;

        // If we slopped over, add one to size.
        if (((XDim * RendInput->BPP) % CHARSIZE) != 0)
            Rx++;

        if((Rx & 0x3) != 0)
            Rx = (Rx & 0xfffffffc) + 0x4;
        //
        // Take 100 lines or less
        //
        if(YDim < MyGraphic->RotLPS)
                MyGraphic->RotLPS = YDim;

        for (Ry = 0; Ry < (YDim/MyGraphic->RotLPS+1); Ry++)
        {
            LINK_MEMBER *TempLink;

            TempLink = (LINK_MEMBER *)MEM_MALLOC_LIMITED(mlimiter_by_name("ipage_print"),
                       sizeof(LINK_MEMBER)+ Rx*MyGraphic->RotLPS+4); // +4 to take care of roundofs.
            ASSERT(TempLink != NULL);
            TempLink->NextLink = NULL;
            TempLink->Data = (void *)((UINT32) TempLink + sizeof(LINK_MEMBER));
            TempLink->Param = MyGraphic->RotLPS;  // set number of lines in this buffer.
            memset(TempLink->Data, 0, Rx * MyGraphic->RotLPS);        // clear the strip
            agAddTail(&MyGraphic->ROPlane[Color], TempLink);
        }
        MyGraphic->PlaneRotated[Color] = TRUE;

        //
        // We now have the destination location.
        // Rotate the thing.
        rotate(MyGraphic->X_DIMENSION, MyGraphic->Y_DIMENSION, 
	       MyGraphic->LENGTH_X, agGetHead(&MyGraphic->InPlane[Color]),
	       agGetHead(&MyGraphic->ROPlane[Color]),
               RendInput->BPP, RendInput->ImagesRotation);
       
        MyGraphic->LENGTH_X = Rx;
    }

    //
    // Set up the object graphic object with the info needed for the renderer
    //
    TempGraphic = *((LINKGRAPHIC *) TempPage->Object);
    TempGraphic.X_DIMENSION = XDim;
    TempGraphic.Y_DIMENSION = YDim;
    //
    // Set up the line length.
    //
    if(!TempGraphic.PlaneRotated[Color])
    {
        TempGraphic.LENGTH_X = (XDim*RendInput->BPP) / 8;
        if(((XDim* RendInput->BPP) % 8) != 0)
             TempGraphic.LENGTH_X++;
    }

    // Everything is ready to go.  Put in the object.
    //
    //dbg_printf("CopyLinkObject...\n");

    CopyLinkObj(Strip, &TempGraphic, Color, TempPage, StartY, 
              Len, Offset, (pRSession->PIXELLINE*RendInput->BPP)/CHARSIZE,0, 
	      RendInput->BPP, get_pixel_stride(RendInput->packing),
	      get_pixel_offset(RendInput->packing)+Color, 
	      RendInput->data_type);
    return;
}

/**
 *
 * \brief Initialize data for the renderer
 * This reads in the patches and de-compresses them in preperation for the 
 * renderer to start.
 *
 */
void renderer_init(void)
{
#ifdef HAVE_CMD
    int cmd_res;
    cmd_res = cmd_register_cmd( "rend",
                                NULL,
                                "renderer commands",
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "rend",
                                   "raster",
                                   renderer_cmd_raster_desc,
                                   renderer_cmd_raster_usage,
                                   renderer_cmd_raster_notes,
                                   renderer_cmd_raster_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "rend",
                                   "line",
                                   renderer_cmd_line_desc,
                                   renderer_cmd_line_usage,
                                   renderer_cmd_line_notes,
                                   renderer_cmd_line_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "rend",
                                   "text",
                                   renderer_cmd_text_desc,
                                   renderer_cmd_text_usage,
                                   renderer_cmd_text_notes,
                                   renderer_cmd_text_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "rend",
                                   "printtext",
                                   renderer_cmd_print_text_desc,
                                   renderer_cmd_print_text_usage,
                                   renderer_cmd_print_text_notes,
                                   renderer_cmd_print_text_cb );
    ASSERT( CMD_OK == cmd_res );


#endif
}

static const char *renderer_cmd_raster_desc = "render a raster";
static const char *renderer_cmd_raster_usage = "<x orgin> <y orgin> <x stretch> <y stretch>";
static const char *renderer_cmd_raster_notes = NULL;
static const char *renderer_cmd_line_desc = "render a line";
static const char *renderer_cmd_line_usage = "<x1> <y1> <x2> <y2>";
static const char *renderer_cmd_line_notes = NULL;
static const char *renderer_cmd_text_desc = "render the specified string";
static const char *renderer_cmd_text_usage = "<x> <y> <string>";
static const char *renderer_cmd_text_notes = "x and y make up the starting location of the text in pixels, string must be a valid null terminated string";
static const char *renderer_cmd_print_text_desc = "print the specified string on the page starting at x,y (pixel location)";
static const char *renderer_cmd_print_text_usage = "<x> <y> <string>";
static const char *renderer_cmd_print_text_notes = "x and y make up the starting location of the text in pixels, string must be a valid null terminated string";

/**
 * Function name: ShutDownRenderer
 *
 * \brief Free up the patch memory used by the renderer
 *
 *
 **/
void ShutDownRenderer(void)
{
}

#ifdef HAVE_CMD
static int renderer_cmd_raster_cb(int argc, char *argv[])
{
    uint32_t pos;
    uint32_t i;
    uint32_t stride;
    uint32_t NumStripes;
    uint32_t offset;
    uint32_t line;
    RENDER_INPUT RendInput;
    DISP_LIST dispList;
    LINK_MEMBER *tempLink;
    GRAPHIC Graphic;
    char *buffer;
    char Line[]={0xff, 0xfe, 0xfd, 0xfb, 0xfa, 0xf9};
    char Line1[]={0xfc, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4};

    if ( argc != 5 )
    {
        return CMD_USAGE_ERROR;
    }

    memset(&RendInput, 0, sizeof(RENDER_INPUT));

    // Test rgb
    RendInput.packing = IPAGE_RGB_PACK;  // do rgbx pixels.
    RendInput.data_type = REND_DATA_RGB;
    stride = 3;
    // setup desired text
    memset(&Graphic, 0, sizeof(Graphic));
    Graphic.X_DIMENSION = 3;
    Graphic.Y_DIMENSION = 2;
    Graphic.LENGTH_X = 3;
    Graphic.K_OR_R_PLANE = Line;
    Graphic.C_OR_G_PLANE = Line1;

    // put desired text into a display list
    dispList.Object = &Graphic;
    dispList.Type = e_Graphics;
    dispList.Operation = e_or; 
    dispList.OBJ_X = 1;
    str_mkint(argv[1], &dispList.OBJ_X);
    dispList.OBJ_Y = 1;
    str_mkint(argv[2], &dispList.OBJ_Y);
    dispList.OBJ_X_LEN = 0;
    str_mkint(argv[3], &dispList.OBJ_X_LEN);
    dispList.OBJ_Y_LEN = 0;
    str_mkint(argv[4], &dispList.OBJ_Y_LEN);
    dispList.NextObj = NULL;
    // setup the page to use our display list
    RendInput.GenerateStripe = FALSE; // use existing strip-list & don't send to printer
    RendInput.BPP = 8;
    RendInput.Copies = 1;
    RendInput.ImagesRotation = e_NoRotate;
    RendInput.OutBufLPS = 25;
    RendInput.X_Resolution = 600;
    RendInput.Y_Resolution = 600;
    RendInput.Rotation = e_Portrait;
    RendInput.XSize = 25* RendInput.BPP;  // use strip width as page width
    RendInput.YSize = 50; // use strip height as page height
    RendInput.Page = &dispList;
    agLinkInit(&RendInput.Output[0]);
    pos = RendInput.BPP * RendInput.XSize* RendInput.OutBufLPS * stride; 
    i = RendInput.YSize / RendInput.OutBufLPS;
    // get the data buffers.
    while(i-->0)
    {
        tempLink = MEM_MALLOC_LIMITED(mlimiter_by_name("ipage_print"), pos+sizeof(LINK_MEMBER));
        tempLink->NextLink = NULL;
        tempLink->Data = (uint8_t *) tempLink + sizeof(LINK_MEMBER);
        if(RendInput.data_type == REND_DATA_RGB)
        {
            memset(tempLink->Data, 0xff, pos);
        } else
        {
            memset(tempLink->Data, 0, pos);
        }
        agAddTail(&RendInput.Output[0], tempLink);           
    }

    Renderer(&RendInput);
    // Write it out
    //
    tempLink = agGetHead(&RendInput.Output[0]);

    for (NumStripes = 0; NumStripes < RendInput.YSize / RendInput.OutBufLPS; NumStripes++)
    {
        buffer = tempLink->Data;
        for (line = 0; line < RendInput.OutBufLPS; line++)
        {
            cmd_printf("\n%lu ", line);
            offset = (line* RendInput.XSize * stride) / RendInput.BPP;
            for (pos = 0; pos < RendInput.XSize/RendInput.BPP; pos++)
            {
                for (i = 0; i < stride; i++)
                {
                    cmd_printf("%2.2x",buffer[offset + pos*stride + i]);
                }
                cmd_printf(" ");
            }
        }
        cmd_printf("\nStripe %lu\n",NumStripes);
        tempLink = tempLink->NextLink;
        memFree(agDelHead(&RendInput.Output[0]));
    }

    return CMD_OK;
}

static int renderer_cmd_line_cb(int argc, char *argv[])
{
    uint32_t pos;
    uint32_t i;
    uint32_t stride;
    uint32_t NumStripes;
    uint32_t offset;
    uint32_t line;
    RENDER_INPUT RendInput;
    DISP_LIST lineObj;
    LINK_MEMBER *tempLink;
    LINE myLine;
    char *buffer;

    if ( argc != 5 )
    {
        return CMD_USAGE_ERROR;
    }

    // Test rgb
    stride = 3;
    memset(&lineObj, 0, sizeof(DISP_LIST));
    memset(&myLine, 0, sizeof(LINE));
    myLine.Color = 0xff;
    myLine.LineWidth = 1;

    lineObj.NextObj = NULL;
    // put desired text into a display list
    lineObj.Type = e_DrawLine;
    lineObj.Operation = e_or; 
    lineObj.Object = &myLine;

    lineObj.OBJ_X = 1;
    str_mkint(argv[1], &lineObj.OBJ_X);
    lineObj.OBJ_Y = 1;
    str_mkint(argv[2], &lineObj.OBJ_Y);
    lineObj.OBJ_X_LEN = 0;
    str_mkint(argv[3], &lineObj.OBJ_X_LEN);
    lineObj.OBJ_Y_LEN = 0;
    str_mkint(argv[4], &lineObj.OBJ_Y_LEN);
    lineObj.NextObj = NULL;
    // setup the page to use our display list
    RendInput.GenerateStripe = FALSE; // use existing strip-list & don't send to printer
    RendInput.BPP = 8;
    RendInput.data_type = REND_DATA_RGB;
    RendInput.Copies = 1;
    RendInput.ImagesRotation = e_NoRotate;
    RendInput.OutBufLPS = 25;
    RendInput.X_Resolution = 600;
    RendInput.Y_Resolution = 600;
    RendInput.Rotation = e_Portrait;
    RendInput.XSize = 25*RendInput.BPP;  // use strip width as page width
    RendInput.YSize = 50; // use strip height as page height
    RendInput.Page = &lineObj;
    RendInput.packing = IPAGE_RGB_PACK;  // do rgbx pixels.
    agLinkInit(&RendInput.Output[0]);
    pos = RendInput.BPP * RendInput.XSize* RendInput.OutBufLPS * stride; 
    i = RendInput.YSize / RendInput.OutBufLPS;
    // get the data buffers.
    while(i-->0)
    {
        tempLink = MEM_MALLOC_LIMITED(mlimiter_by_name("ipage_print"), pos+sizeof(LINK_MEMBER));
        tempLink->NextLink = NULL;
        tempLink->Data = (uint8_t *) tempLink + sizeof(LINK_MEMBER);
        if(RendInput.data_type == REND_DATA_RGB)
        {
            memset(tempLink->Data, 0xff, pos);
        } else
        {
            memset(tempLink->Data, 0, pos);
        }
        agAddTail(&RendInput.Output[0], tempLink);           
    }

    Renderer(&RendInput);
    // Write it out
    //
    tempLink = agGetHead(&RendInput.Output[0]);

    for (NumStripes = 0; NumStripes < RendInput.YSize / RendInput.OutBufLPS; NumStripes++)
    {
        buffer = tempLink->Data;
        for (line = 0; line < RendInput.OutBufLPS; line++)
        {
            cmd_printf("\n%lu ", line);
            offset = (line* RendInput.XSize * stride)/RendInput.BPP;
            for (pos = 0; pos < RendInput.XSize/RendInput.BPP; pos++)
            {
                for (i = 0; i < stride; i++)
                {
                    cmd_printf("%2.2x",buffer[offset + pos*stride + i]);
                }
                cmd_printf(" ");
            }
        }
        cmd_printf("\nStripe %lu\n",NumStripes);
        tempLink = tempLink->NextLink;
        memFree(agDelHead(&RendInput.Output[0]));
    }

    return CMD_OK;
}

static int renderer_cmd_text_cb(int argc, char *argv[])
{
    uint32_t pos;
    uint32_t i;
    uint32_t stride;
    uint32_t NumStripes;
    uint32_t offset;
    uint32_t line;
    RENDER_INPUT RendInput;
    DISP_LIST dispList;
    TEXT textObj;
    LINK_MEMBER *tempLink;
    char *buffer;

    if ( argc != 4 )
    {
        return CMD_USAGE_ERROR;
    }

    // Test rgb
    RendInput.packing = IPAGE_RGB_PACK;  // do rgbx pixels.
    stride = 3;
    // setup desired text
    memset(&textObj, 0, sizeof(textObj));
    textObj.Color = 0xff;
    textObj.FONT = 16;  // set the point size.
    textObj.lang = LANG_ENGLISH;

    // put desired text into a display list
    dispList.Object = &textObj;
    dispList.Type = e_Text;
    dispList.Operation = e_or; 
    dispList.OBJ_X = 1;
    str_mkint(argv[1], &dispList.OBJ_X);
    dispList.OBJ_Y = 1;
    str_mkint(argv[2], &dispList.OBJ_Y);
    dispList.OBJ_X_LEN = 0;
    dispList.OBJ_Y_LEN = 0;
    dispList.NextObj = NULL;
    textObj.Data = argv[3];   // string to output
    // setup the page to use our display list
    RendInput.GenerateStripe = FALSE; // use existing strip-list & don't send to printer
    RendInput.BPP = 8;
    RendInput.data_type = REND_DATA_RGB;
    RendInput.Copies = 1;
    RendInput.ImagesRotation = e_NoRotate;
    RendInput.OutBufLPS = 25;
    RendInput.X_Resolution = 600;
    RendInput.Y_Resolution = 600;
    RendInput.Rotation = e_Landscape;
    RendInput.XSize = 25* RendInput.BPP;  // use strip width as page width
    RendInput.YSize = 250; // use strip height as page height
    RendInput.Page = &dispList;
    RendInput.language = LANG_ENGLISH;
    agLinkInit(&RendInput.Output[0]);
    pos = RendInput.BPP * RendInput.XSize* RendInput.OutBufLPS * stride; 
    i = RendInput.YSize / RendInput.OutBufLPS;
    // get the data buffers.
    while(i-->0)
    {
        tempLink = MEM_MALLOC(pos+sizeof(LINK_MEMBER));
        tempLink->NextLink = NULL;
        tempLink->Data = (uint8_t *) tempLink + sizeof(LINK_MEMBER);
        if(RendInput.data_type == REND_DATA_RGB)
        {
            memset(tempLink->Data, 0xff, pos);
        } else
        {
            memset(tempLink->Data, 0, pos);
        }
        agAddTail(&RendInput.Output[0], tempLink);           
    }

    Renderer(&RendInput);
    // Write it out
    //
    tempLink = agGetHead(&RendInput.Output[0]);

    for (NumStripes = 0; NumStripes < RendInput.YSize / RendInput.OutBufLPS; NumStripes++)
    {
        buffer = tempLink->Data;
        for (line = 0; line < RendInput.OutBufLPS; line++)
        {
            cmd_printf("\n%lu ", line);
            offset = (line* RendInput.XSize * stride)/RendInput.BPP;
            for (pos = 0; pos < RendInput.XSize/RendInput.BPP; pos++)
            {
                for (i = 0; i < stride; i++)
                {
                    cmd_printf("%2.2x",buffer[offset + pos*stride + i]);
                }
                cmd_printf(" ");
            }
        }
        cmd_printf("\nStripe %lu\n",NumStripes);
        tempLink = tempLink->NextLink;
        memFree(agDelHead(&RendInput.Output[0]));
    }


    return CMD_OK;
}

static int renderer_cmd_print_text_cb(int argc, char *argv[])
{
    uint32_t stride;
    RENDER_INPUT RendInput;
    DISP_LIST dispList;
    TEXT textObj;

    if ( argc != 4 )
    {
        return CMD_USAGE_ERROR;
    }

    // Test rgb
    RendInput.packing = IPAGE_RGBX_PACK;  // do rgbx pixels.
    stride = 4;
    // setup desired text
    memset(&textObj, 0, sizeof(textObj));
    textObj.Color = 0xff;
    textObj.FONT = 16;  // set the point size.
    textObj.lang = LANG_ENGLISH;

    // put desired text into a display list
    dispList.Object = &textObj;
    dispList.Type = e_Text;
    dispList.Operation = e_or; 
    dispList.OBJ_X = 1;
    str_mkint(argv[1], &dispList.OBJ_X);
    dispList.OBJ_Y = 1;
    str_mkint(argv[2], &dispList.OBJ_Y);
    dispList.OBJ_X_LEN = 0;
    dispList.OBJ_Y_LEN = 0;
    dispList.NextObj = NULL;
    textObj.Data = argv[3];   // string to output
    // setup the page to use our display list
    RendInput.GenerateStripe = true; // use existing strip-list & don't send to printer
    RendInput.BPP = 8;
    RendInput.data_type = REND_DATA_RGB;
    RendInput.Copies = 1;
    RendInput.ImagesRotation = e_NoRotate;
    RendInput.OutBufLPS = 100;
    RendInput.X_Resolution = 600;
    RendInput.Y_Resolution = 600;
    RendInput.Rotation = e_Portrait;
    RendInput.XSize = RendInput.X_Resolution * 5;  // use strip width as page width
    RendInput.YSize = RendInput.Y_Resolution * 5; // use strip height as page height
    RendInput.Page = &dispList;
    agLinkInit(&RendInput.Output[0]);

    Renderer(&RendInput);

    return CMD_OK;
}

#endif // #ifdef HAVE_CMD 

 


#ifdef DEBUG
/****************************************************************************
 *
 * $Function: StripToSerial $
 *
 * $Description: $
 *    This function outputs a representation of the input strip data to the
 *    serial port.  The output is "stacks" 128 bits chunks of strip width.
 *
 * $Parameters: pointer to the strip data
 *              # bits/pixel for this strip (1 and 2 bpp supported)
 *              width of the input strip
 *              height of the input strip $
 *
 * $Return:     None $
 *
 * $Errors:     None $
 *
 *
 ****************************************************************************/
void StripToSerial(char *stripData, int pixelDepth, int stripWidth, int stripHeight)
{
    int i,j,k;

    // break strip into 128 bit chunks
    for ( k = 0; k < ( ( stripWidth * pixelDepth + 127 ) / 128 ); k++ ) // round up for possible partial chunk
    {
        for(i=0; i<stripHeight; i++)
        {
            // output each row in this chunk of the strip
            for(j=0; j<128; j++)
            {
                // terminate early on partial chunk
                if ( ( k * 128 + j ) >= ( stripWidth * pixelDepth ) )
                    break;

                if ( stripData[ ( i * stripWidth * pixelDepth + k * 128 + j ) >> 3 ] & ( 0x80 >> ( j % 8 ) ) )
                {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT,("*"));
                }
                else
                {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT,("."));
                }
            }
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("\n"));
        }

        // add a separator between chunks
        for(j=0; j<128; j++)
        {
            // terminate early on partial chunk
            if ( ( k * 128 + j ) >= ( stripWidth * pixelDepth ) )
                break;

            DPRINTF(DBG_SOFT|DBG_OUTPUT,("#"));
        }
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("\n"));
    }
}
#endif // #ifdef DEBUG
