/***********************************************************
* (c) Copyright 2008 Marvell International Ltd.
*
*               Marvell Confidential
* ==========================================================
*/

/******************************************************************************
 *
 * Description:  Rotate a image that in given to us the direction specified.
                The output is put in destination.
 *
 *****************************************************************************/


// #include "tie.h" //Rajat Harlalka, added for tie instruction

#include <stddef.h>
#include <stdarg.h>
#include "memAPI.h"
#include "logger.h"
#include "lassert.h"
#include "agLinkedList.h"
#include "renderer.h"

#define ROTSIZE 32       ///< Size of the rotation primitive.


/**
 * Function name: rotate90
 *
 * \brief Rotate single bit byte alligned data from source to destination
 *
 * Description: This will align byte aligned data from source to destination.  It will work with
 *              number of pixels.  The output is byte aligned.  Also if the input
 *              is divisible by 8 in both rows and columns then this can also
 *              rotate in place.
 *
 * \param HorzPixels Number of pixels in the horizantal direction
 * \param Rows The number of Rows in the image.
 * \param ByteWidth The number of bytes in a row of the image.
 * \param Source A linked list of the source image.
 * \param Destination A linked list of the data buffers for the rotated image.
 * \param BPP Number of bits/pixel in the image.
 * \param Direction An enum to specify the direction of rotation.
 *
 * Author: Dirk Bodily
 *
 * Notes:
 *
 **/
void rotate(UINT32 HorzPixels, UINT32 Rows, UINT32 ByteWidth,LINK_MEMBER  *Source,
             LINK_MEMBER *Destination, UINT8 BPP, ROTATE_DIR Direction)
{
    UINT32 NumberOfHBlocks;
    UINT32 NumberOfVBlocks;
    UINT32 OutPutWidthBytes;
    UINT8 *TempOutput;
    UINT32 OutputRowCnt;
    UINT32 ExtraCol;        ///< Number of extra columns at the right
    UINT32 ExtraRows;       ///< number of extra rows at the bottom.
    UINT32 Mask=0;          ///< Bit mask for pulling and placing pixels.
    LINK_MEMBER *CurrentStripe, *OutputStripe;


    // Figure the number of blocks of size 8x8 we have in this image.
    //
    NumberOfHBlocks = HorzPixels / (ROTSIZE/BPP);
    NumberOfVBlocks = Rows / (ROTSIZE/BPP);
    ExtraCol = HorzPixels % (ROTSIZE/BPP);
    ExtraRows = Rows % (ROTSIZE/BPP);
    OutputRowCnt = 0;       // start at the beginning.
    //
    // Figure the output width in bytes.
    //
    OutPutWidthBytes = (Rows * BPP) / 8;
    if(((Rows * BPP) % 8) != 0)
        OutPutWidthBytes++;     // make room for extra.
    if((OutPutWidthBytes & 0x3) != 0)
        OutPutWidthBytes = (OutPutWidthBytes & 0xfffffffc) + 0x4;

    //
    // If we have more than 1 bpp then we do not have
    // any hardware help and should do the rotation in the first section.
    //
  //  if(BPP > 1)       // add this back in if you want to use the #if 0 code below.
    {
        ExtraRows = 0;
        NumberOfHBlocks = 0;
    }
    //
    // If either the row or col is not 8 long then the other size must be zero.
    if(NumberOfHBlocks == 0)
        NumberOfVBlocks = 0;
    if(NumberOfVBlocks == 0)
    {
        ExtraCol = HorzPixels;
        NumberOfHBlocks = 0; // limit them.
    }

    //
    // get the mask.  This is used to select the bits of the word to move.
    // it also selects the number of bits we are interested in moving.
    //
    switch(BPP)
    {
        case 1:
            Mask = 0x80000000;
            break;
        case 2:
            Mask = 0xc0000000;
            break;
        case 4:
            Mask = 0xf0000000;
            break;
        case 8:
            Mask = 0xff000000;
            break;
        default:
            ASSERT(0);
    }
    //
    // First rotate the extra columns on the right into the image top.
    // Note that we do not have tie for multi-bit per pixel images.
    // In that case the entire rotation is done in this section.
    //
    if (ExtraCol)
    {
        SINT32 ColCount, RowCount;
        UINT8 DestData;
        UINT8 CurrentBit;
        UINT32 LineLength;
        UINT32 Address;

        //
        //
        CurrentBit = 0;
        DestData = 0;
        LineLength = 0;
        OutputStripe = Destination;
        TempOutput = OutputStripe->Data;

        //
        // Do a 90 degree rotation.
        // What this does is take the first column, 32 bits at a time and
        // rotate it into the first row in the image.  We then go to the
        // next set of colums and rotate them into the next set of rows
        // in the output.  This continues till we are complete.
        //
        if (Direction == e_Rotate90)
        {
            UINT32 Output[ROTSIZE/BPP];
            UINT32 *MyInput, MyData, *MyOutput;
            UINT32 n, OutputRowCount=0;
            UINT32 InputRowCount, SaveBits, SaveLineLength;



            //
            // Figure the output line length.  We work in groups of 4 bytes so
            // this must end up a multiple of 4.  The stuff that slops over on
            // the end we take care of by setting current bit.  That will then
            // take the partial number of bits and put them into the output
            // buffer.
            //
            LineLength = (Rows * BPP)/8;    // This must be in bytes
            CurrentBit = ((Rows * BPP) % 8);// how much is left over.  Partial bytes
            CurrentBit += (LineLength % sizeof(UINT32)) * 8; // Add in the
                                        // extra that comes because of rounding
                                        // to a multiple of 4.
            //
            // Adjust the line length to a multiple of 4.
            if((LineLength % sizeof(UINT32)) != 0)
                LineLength &= ~(sizeof(UINT32) -1);
            // Figure where to start in the first word.
            CurrentBit = ROTSIZE - CurrentBit;
            // Save the values for use later.
            SaveBits = CurrentBit;
            SaveLineLength = LineLength;
            // Initialize the output pointer.
            MyOutput = (UINT32 *)TempOutput;
            //
            // Do this for each column.  We work with 32 bits of data
            // at a time so we increment by ROTSIZE/BPP.  Since that
            // gives the number of colums we are rotating at a time.
            //
            for (ColCount = 0; ColCount < ExtraCol ; ColCount+=ROTSIZE/BPP)
            {
                UINT32 ColShift;

                //
                // ColShift is used to make sure we access only the data needed
                // when the width of the rotated image is not exactly divisable
                // by 32.  First we figure out how many bits to ignore.
                ColShift = 0;
                if(ColCount == ExtraCol)
                    ColShift = ROTSIZE/BPP - (ExtraCol % (ROTSIZE/BPP));  // number to ignore
                CurrentStripe = Source;
                MyOutput = OutputStripe->Data;

                MyInput = (UINT32 *)CurrentStripe->Data;
                InputRowCount = 0;
                //
                // We rotate into Output.  When Output is full we copy it
                // into the destination buffer.  Clear the output here.
                //
                
                // ASSERTION ensures we initialize the Output buffer
                // so that ROTSIZE / BPP >= 1
                ASSERT( BPP <= ROTSIZE );

                for(RowCount = 0; RowCount < ROTSIZE/BPP; RowCount++)
                    Output[RowCount] = 0;
                //
                // This works by scanning down the column in the source by rows
                // rotating the data into the destination.  When Output buffer
                // fills we copy that data into the rows in the output.  Again
                // we are building ROTSIZE/BPP colums a time.
                //
                for(RowCount = 0; RowCount < Rows; RowCount++)
                {
                    ASSERT(Destination->NextLink->NextLink->NextLink != 0);
                    //
                    // If our rowcount fources into a new stripe, get that
                    // stripe here.
                    //
                    if(InputRowCount >= CurrentStripe->Param)
                    {
                        CurrentStripe = CurrentStripe->NextLink;
                        InputRowCount = 0;
                        // Make sure the new strip is valid.
                        ASSERT(CurrentStripe != NULL);
                        // get the data pointer.
                        MyInput = (UINT32 *) CurrentStripe->Data;

                    }
                    //
                    // Now figure the source address for the data.  Start at the stripe boundry,
                    // add in the offset to the data. Then space down by the column offset.
                    //
                    Address = (((UINT32)MyInput + (InputRowCount) * ByteWidth + ((ColCount * BPP))/8) );
                    //
                    // Everything should come in aligned to 4 bytes.  However
                    // if not get single bytes and merge them into a work.
                    // else just load the word.
                    //
                    if (Address & 0x3)
                    {
                        MyData = (*(UINT8 *)Address)<<24;
                        MyData |= (*(((UINT8 *)Address)+1))<<16;
                        MyData |= (*(((UINT8 *)Address)+2))<<8;
                        MyData |= (*(((UINT8 *)Address)+3));
                    }
                    else
                        MyData = *((UINT32 *)Address);
                    //
                    // This for loop does the actual rotate.
                    //
                    for(n = ColShift; n < ROTSIZE/BPP; n++)
                    {
                        // n will step across the 32 bits from the Highest to lowest
                        // we then mask off only the data we are interested in and
                        // then we shift it into the correct position for storage.
                        // then we or that data into the output buffer.
                        //
                        Output[n - ColShift] |= ((MyData << ((n)* BPP)) & Mask)>> // Get the data
                            (ROTSIZE - BPP - CurrentBit); // put it in the correct position.
                    }
                    //
                    // The rotation of that source word is complete.  The next word will
                    // go in the next position.  Increment the position to put it
                    // there.
                    CurrentBit += BPP;
                    //
                    // See if it is time to write the data out.  This is done
                    // when we have filled the outbuffer which occurs when CurrentBit
                    // is equal to ROTSIZE
                    //
                    if(CurrentBit == ROTSIZE) // see if we need to store the stuff.
                    {
                        //
                        // Write the data to the output buffer.
                        //
                        for(n = 0; n < ROTSIZE/BPP; n++)
                        {
                            //
                            // Check to see if this write will force us to the next
                            // strip.  If so get the next strip.
                            //

                            if((n + OutputRowCount) >= OutputStripe->Param)
                                MyOutput = OutputStripe->NextLink->Data;
                            Address = (((UINT32) MyOutput +
                                     ((n+OutputRowCount)% OutputStripe->Param) *
                                     OutPutWidthBytes + LineLength));

                            //
                            // See if we have done all the rows.
                            //
                            if((n + OutputRowCount) >= HorzPixels)  // horzpixels now is the height
                                break;
                            // Write the output data
                            *(UINT32 *)Address = Output[n];
                            // CLear the buffer for the next pass.
                            Output[n] = 0;
                        }
                        // Restart at the beginning of the next word.
                        CurrentBit = 0;

                        // decrement the line length for the next pass.
                        LineLength -= (ROTSIZE)/8; // 8 is bits per byte
                        // reset the output to the beginning for the next pass.
                        MyOutput = OutputStripe->Data;

                    }
                    InputRowCount++;        // go to the next input row.
                }
                //
                //  Put the leftovers into the buffer.
                //  The output buffer should be 4 byte aligned so
                //  we can write the word out and not destroy anything.
                //
                if(CurrentBit != 0) // see if we have anything left over to store
                {
                    for(n = 0; n < ROTSIZE/BPP; n++)
                    {
                        //
                        // Check to see if this write will force us to the next
                        // strip.  If so get the next strip.
                        //

                        if((n + OutputRowCount) >= OutputStripe->Param)
                            MyOutput = OutputStripe->NextLink->Data;
                        Address = (((UINT32) MyOutput + ((n+OutputRowCount)% OutputStripe->Param) * OutPutWidthBytes +
                                  LineLength));

                        //
                        // See if we have done all the rows.
                        //
                        if((n + OutputRowCount) >= HorzPixels)  // horzpixels now is the height
                            break;
                        // Write the output data
                        *(UINT32 *)Address = Output[n];
                        // CLear the buffer for the next pass.
                        Output[n] = 0;
                    }
                }

                OutputRowCount+= ROTSIZE/BPP - ColShift;
                LineLength = SaveLineLength;
                CurrentBit = SaveBits;  // if we are here then we are not on a short xfer and can set this to 0.
                if(OutputRowCount > OutputStripe->Param)
                {
                    UINT32 TempCnt;
                    LINK_MEMBER *TTempLink;

                    TTempLink = OutputStripe;
                    TempCnt = OutputStripe->Param;
                    OutputRowCount-= OutputStripe->Param;
                    OutputStripe = OutputStripe->NextLink;
                    ASSERT(OutputStripe != NULL);
                }

            }
        }
        //
        // The only difference between this rotation and the one above is
        // that we start at the highest column and top row pulling bits.
        // Then we dec the column and continue till the end.  Detailed
        // comments see the above section.
        //
        LineLength = 0;
        if (Direction == e_Rotate270)
        {
            UINT32 Output[ROTSIZE/BPP];
            UINT32 *MyInput, MyData, *MyOutput;
            UINT32 n, OutputRowCount=0;
            UINT32 InputRowCount;

            MyOutput = (UINT32 *)TempOutput;
            //
            //
            // Do this for each column.  Start at the last colum and
            // rotate into the top column of the destination.

            for (ColCount = ExtraCol; ColCount > 0 ; ColCount-=ROTSIZE/BPP)
            {
                UINT32 ColShift;

                //
                // ColShift is used to make sure we access only the data needed
                // when the width of the rotated image is not exactly divisable
                // by 32.  First we figure out how many bits to ignore.
                ColShift = 0;
                if(ColCount == ExtraCol)
                    ColShift = ROTSIZE/BPP - (ExtraCol % (ROTSIZE/BPP));  // number to ignore
                CurrentStripe = Source;
                MyOutput = OutputStripe->Data;
                // get the data pointer to the input strip.
                MyInput = (UINT32 *)CurrentStripe->Data;
                InputRowCount = 0;

                //
                // Clear the output buffer.
                //
                for(RowCount = 0; RowCount < ROTSIZE/BPP; RowCount++)
                    Output[RowCount] = 0;
                // We have selected a column to scan.  Go down
                // the source column by rows till we reach the last column.
                //
                for(RowCount = 0; RowCount < Rows; RowCount++)
                {
                    UINT32 Address;
                    //
                    // See if we need to go to the next input strip.
                    //
                    if(InputRowCount >= CurrentStripe->Param)
                    {
                        CurrentStripe = CurrentStripe->NextLink;
                        InputRowCount = 0;
                        ASSERT(CurrentStripe != NULL);
                        MyInput = (UINT32 *) CurrentStripe->Data;

                    }
                    //
                    // Figure the address, we will access in 32 bit chuncks.
                    // so we need to make sure the address always increments
                    // by 4 due tot he colcount.  If the address due to bytewidth
                    // does not increment by 4, get the data in pieces.
                    //
                    Address = (UINT32)MyInput +
                                (InputRowCount) * ByteWidth +
                                (((ColCount * BPP)/8)/4)*4;
                    //
                    // Everything should come in aligned to 4 bytes.  However
                    // if not get single bytes and merge them into a work.
                    // else just load the word.
                    //
                    if (Address & 0x3)
                    {
                        MyData = (*(UINT8 *)Address)<<24;
                        MyData |= (*(((UINT8 *)Address)+1))<<16;
                        MyData |= (*(((UINT8 *)Address)+2))<<8;
                        MyData |= (*(((UINT8 *)Address)+3));
                    }
                    else
                        MyData = *((UINT32 *)Address);
                    //
                    // Now do the rotation.
                    // Here is where colshift is used to skip the bits that are
                    // not part of the image.
                    for(n = ColShift; n < ROTSIZE/BPP; n++)
                    {
                        Output[n - ColShift] |= ((MyData << ((ROTSIZE/BPP-n-1)* BPP)) &
                                                 Mask)>> CurrentBit;
                    }
                    // This row in the current column is complete.  Change to
                    // the next column
                    CurrentBit += BPP;
                    // Write the data out if the output buffer is full.
                    // When the CurrentBit reaches a word size the buffer is full
                    // and must be written out.
                    //
                    if(CurrentBit == ROTSIZE) // see if we need to store the stuff.
                    {
                        for(n = 0; n < ROTSIZE/BPP; n++)
                        {
                            // if we are larger than a stripe, get the next one.
                            if((n + OutputRowCount) >= OutputStripe->Param)
                                MyOutput = OutputStripe->NextLink->Data;
                            //
                            // Figure the address of the destination of the data.
                            // Start at the beginning of the buffer, index in
                            // by the number of lines times the line length.
                            // last add in the current offset into the line.
                            //
                            Address = (((UINT32) MyOutput + ((n+OutputRowCount) %
                                      OutputStripe->Param) * OutPutWidthBytes +
                                      LineLength));

                            //
                            // See if we have done all the rows, if we are done
                            // get out of here, the store is complete.
                            //
                            if((n + OutputRowCount) > HorzPixels)  // horzpixels now is the height
                                break;
                            *(UINT32 *)Address = Output[n]; // store the output data.
                            Output[n] = 0;      // clear the temp buffer for the next pass.
                        }

                        CurrentBit = 0;     // everything is stored. reset output pointer.
                        LineLength+= (ROTSIZE)/8; // 8 is bits per byte move to the next
                                                  // address in the line.
                        MyOutput = OutputStripe->Data;  // reset the output to the starting stripe.

                    }
                    InputRowCount++;
                }
                //
                // We have finished this set of columns.  If there is other stuff
                // left to store do it here.
                //  Put the leftovers into the buffer.
                //  The output buffer should be 4 byte aligned so
                //  we can write the word out and not destroy anything.
                //
                if(CurrentBit != 0) // see if we have anything left over to store
                {
                    for(n = 0; n < ROTSIZE/BPP; n++)
                    {
                        //
                        // Check to see if this write will force us to the next
                        // strip.  If so get the next strip.
                        //

                        if((n + OutputRowCount) >= OutputStripe->Param)
                            MyOutput = OutputStripe->NextLink->Data;
                        Address = (((UINT32) MyOutput + ((n+OutputRowCount)% OutputStripe->Param) * OutPutWidthBytes +
                                  LineLength));

                        //
                        // See if we have done all the rows.
                        //
                        if((n + OutputRowCount) > HorzPixels)  // horzpixels now is the height
                            break;
                        // Write the output data
                        *(UINT32 *)Address = Output[n];
                        // CLear the buffer for the next pass.
                        Output[n] = 0;
                    }
                }
                //
                // Setup for the next sets of columns.
                // Update the row count of the output.  This is usually 32/bpp
                // unless we have extra bits.
                //
                OutputRowCount+= ROTSIZE/BPP - ColShift;
                LineLength = 0;     // Start at the beginning of the output line.
                CurrentBit = 0;     // Amount of data in the output buffer.
                if(OutputRowCount > OutputStripe->Param)
                {
                    UINT32 TempCnt;
                    LINK_MEMBER *TTempLink;

                    TTempLink = OutputStripe;
                    TempCnt = OutputStripe->Param;
                    OutputRowCount-= OutputStripe->Param;
                    OutputStripe = OutputStripe->NextLink;
                    ASSERT(OutputStripe != NULL);
                }

            }
        }
    }
}

//#define ROTATE_TEST
#pragma pack(4)
#ifdef ROTATE_TEST
    unsigned char Input[]={0x11,0xf0,
                           0x11,0xf0,
                           0x11,0xf0,
                           0x11,0xf0
    };
    unsigned char CheckInput[]={0xf0,0xf0,0xf0,0xf0,
                                0xf0,0x00,0x00,0x00,
                                0xf0,0x00,0x00,0x00
    };
    unsigned char Input1[]={0xa5,0xa5,0xa0,
                            0xa5,0xa5,0xa0,
                            0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
        0xa5,0xa5,0xa0,
    };
    unsigned char CheckInput1[]={0x00, 0x00, 0xff, 0xc0,
                                 0x00, 0x00, 0xff, 0xc0,
                                 0xff, 0xc0, 0x00, 0x00,
                                 0xff, 0xc0, 0x00, 0x00,
                                 0x00, 0x00, 0xff, 0xc0,
                                 0x00, 0x00, 0xff, 0xc0,
        0xff, 0xc0, 0x00, 0x00,
        0xff, 0xc0, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xc0,
        0x00, 0x00, 0xff, 0xc0,

    };
    unsigned char Input2[]={0xa5,
                            0x5a,
                            0xa5,
                            0x5a,
        0xa5,
        0x5a,
        0xa5,
        0x5a,
    };
    unsigned char CheckInput2[]={0xaa,
                                 0x55,
                                 0xaa,
                                 0x55,
                                 0x55,
                                 0xaa,
                                 0x55,
                                 0xaa
    };
    //
    // This one is a 2x2 square matrix.
    unsigned char Input3[]={0x01,0x02,
                            0x10,0x20,
                            0x30,0x40,
        0x50,0x60,
        0x70,0x80,
        0x90,0xa0,
        0xb0,0xc0,
        0xd0,0xe0,
        0x03,0x04,
        0x01,0x02,
        0x03,0x04,
        0x05,0x06,
        0x07,0x08,
        0x09,0x0a,
        0x0b,0x0c,
        0x0d,0x0e,
    };
    unsigned char CheckInput3[]={0x00,0x00,
                                 0x80,0x55,
                                 0x00,0xb3,
                                 0x00,0x0f,
                                 0x00,0x00,
                                 0x55,0x00,
                                 0x33,0x00,
                                 0x0f,0x00,
                                 0x80,0xff,
                                 0x00,0xaa,
                                 0x00,0x19, // 3rd bit
                                 0x00,0x07, // 4th bit
                                 0x7f,0x00, // 1st bit
                                 0x2a,0x00, // 2nd bit
                                 0x19,0x00,  // 3rd bit
                                 0x07,0x00,
    };
unsigned char Input4[]={0x01,0x02,0x03,0x04,0xc0,
    0x05,0x06,0x07,0x08,0xc0,
    0x09,0x0a,0x0b,0x0c,0xc0,
    0x0d,0x0e,0x0f,0x10,0xc0,
    0x11,0x12,0x13,0x14,0xc0,
    0x15,0x16,0x17,0x18,0xc0,
    0x19,0x20,0x21,0x22,0xc0,
    0x23,0x24,0x25,0x26,0xc0,
    0x27,0x28,0x29,0x30,0xc0
};
unsigned char CheckInput4[]={0x00,0x00,
                            0xff,0x80,
                            0xff,0x80,
                            0x00,0x00,
                            0x03,0x00,
                            0xa9,0x00,
                            0x64,0x00,
                            0x1c,0x80,
                            0x03,0x80,// bit 2
                            0x00,0x00,//bit 3
                            0x00,0x00, //bit 4
                            0xff,0x80, //bit 1
                            0xfc,0x00, // bit 2
                            0x55,0x00, // bit 3
};
unsigned char Input5[]={0xa0,0xa1,0xa2,
    0xa3,0xa4,0xa5,
    0xa6,0xa7,0xa8,
    0xa9,0xaa,0xab,
    0xac,0xad,0xae,
};
unsigned char CheckInput5[]={0x93,0x80,
    0x1a,0xc0,
    0xaa,0x80,
    0xaa,0x80,
    0x4e,0x40,
    0x16,0xc0,
    0xaa,0x80,
    0xaa,0x80,
    0x39,0x00,
    0x06,0xc0,
    0xaa,0x80,
    0xaa,0x80,
};
unsigned char Input6[]={0x01,0x12,0x23,0x34,
    0x56,0x78,0x9a,0xab,
    0xbc,0xcd,0xde,0xef,
    0x10,0x21,0x32,0x43,
    0x65,0x78,0xa9,0xba,
    0xcb,0xdc,0xed,0xef,
    0x11,0x22,0x33,0x44,
    0x55,0x66,0x77,0x88,
    0x99,0xaa,0xbb,0xcc
};
unsigned char CheckInput6[]={0x4b,0xf3,0xaf,0x48,0xc0,
    0x3a,0xe4,0xbe,0x48,0xc0,
    0x3a,0xe2,0x9d,0x37,0xb0,
    0x29,0xd3,0xae,0x37,0xb0,
    0x28,0xd1,0x8c,0x26,0xa0,
    0x17,0xc2,0x7d,0x26,0xa0,
    0x16,0xc0,0x5b,0x15,0x90,
    0x05,0xb1,0x6c,0x15,0x90,
};
unsigned char Input7[]={0x01,0x02,0x03,0x04,0xf0,
    0x05,0x06,0x07,0x08,0xf0,
    0x15,0x06,0x07,0x08,0xf0,
    0x25,0x06,0x07,0x08,0xf0,
    0x35,0x06,0x07,0x08,0xf0,
    0x45,0x06,0x07,0x08,0xf0,
    0x55,0x06,0x07,0x08,0xf0,
    0x65,0x06,0x07,0x08,0xf0,
    0x75,0x06,0x07,0x08,0xf0,
    0x85,0x06,0x07,0x08,0xf0,
    0x95,0x06,0x07,0x08,0xf0,
    0xa5,0x06,0x07,0x08,0xf0,
    0xb5,0x06,0x07,0x08,0xf0,
    0xc5,0x06,0x07,0x08,0xf0,
    0xd5,0x06,0x07,0x08,0xf0,
    0xe5,0x06,0x07,0x08,0xf0,
    0xf5,0x06,0x07,0x08,0xf0,
    0x25,0x06,0x07,0x08,0xf0,
};
unsigned char CheckInput7[]={0xff,0xff,0xc0,
    0xff,0xff,0xc0,
    0xff,0xff,0xc0,
    0x00,0x00,0x00, // bit 0
    0x00,0x00,0x00, // bit 1
    0x80,0x00,0x00, // bit 2
    0x7f,0xff,0xc0, // bit 3
    0x00,0x00,0x00, // bit 4
    0x00,0x00,0x00, // bit 5
    0x00,0x00,0x00, // bit 6
    0x00,0x00,0x00, // bit 7
    0xff,0xff,0xc0, // bit 0
    0xff,0xff,0xc0, // bit 1
    0x7f,0xff,0xc0, // bit 2
    0x00,0x00,0x00, // bit 3
    0x00,0x00,0x00, // bit 4
    0x00,0x00,0x00, // bit 5
    0x00,0x00,0x00, // bit 6
    0x00,0x00,0x00, // bit 7
    0x00,0x00,0x00, // bit 0
    0xff,0xff,0xc0, // bit 1
    0x7f,0xff,0xc0, // bit 2
    0x00,0x00,0x00, // bit 3
    0x00,0x00,0x00, // bit 4
    0x00,0x00,0x00, // bit 5
    0x00,0x00,0x00, // bit 6
    0x00,0x00,0x00, // bit 7
    0xff,0xff,0xc0, // bit 0
    0x00,0x00,0x00, // bit 1
    0x7f,0xff,0xc0, // bit 2
    0x00,0x00,0x00, // bit 3
    0x2a,0xaa,0x80, // bit 4
    0x19,0x99,0xc0, // bit 5
    0x07,0x87,0x80, // bit 6
    0x00,0x7f,0x80, // bit 7



};
void RotateTest()
{
    unsigned char Output[200];
    LINK_MEMBER LinkIn, LinkOut;
    UINT32 i;

    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;
    rotate90(12,4,2,&LinkIn, &LinkOut,1,200);
    for(i = 0; i < sizeof(CheckInput); i++)
        if(Output[i] != CheckInput[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input check i %x Expected %x Got %x\n",i,CheckInput[i], Output[i]));

    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input1;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;
    rotate90(20,10,3,&LinkIn, &LinkOut,1,300);
    for(i = 0; i < sizeof(CheckInput1); i++)
        if(Output[i] != CheckInput1[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input1 check i %x Expected %x Got %x\n",i,CheckInput1[i], Output[i]));
    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input2;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;

    rotate90(8,8,1,&LinkIn,&LinkOut,1,300);
    for(i = 0; i < sizeof(CheckInput2); i++)
        if(Output[i] != CheckInput2[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input2 check i %x Expected %x Got %x\n",i,CheckInput2[i], Output[i]));
    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }

    LinkIn.NextLink = NULL;
    LinkIn.Data = Input3;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;
    rotate90(16,16,2,&LinkIn,&LinkOut,1,300);
    for(i = 0; i < sizeof(CheckInput3); i++)
        if(Output[i] != CheckInput3[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input3 check i %x Expected %x Got %x\n",i,CheckInput3[i], Output[i]));

    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input4;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;

    rotate90(35,9,5,&LinkIn,&LinkOut,1,300);
    for(i = 0; i < sizeof(CheckInput4); i++)
        if(Output[i] != CheckInput4[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input4 check i %x Expected %x Got %x\n",i,CheckInput4[i], Output[i]));
    //
    // Check for stripe operation
    {
        LINK_MEMBER LinkIn2,LinkIn3,LinkOut2,LinkOut3, LinkOut4, LinkOut5;;
        for (i = 0; i < sizeof(Output); i++)
        {
            Output[i] = 0;
        }
        LinkIn.NextLink = &LinkIn2;
        LinkIn.Data = Input7;
        LinkIn2.NextLink = &LinkIn3;
        LinkIn2.Data = Input7+5*8;
        LinkIn3.NextLink = NULL;
        LinkIn3.Data = LinkIn2.Data + 5*8;


        LinkOut.NextLink = &LinkOut2;
        LinkOut.Data = Output;
        LinkOut2.NextLink = &LinkOut3;
        LinkOut2.Data = Output + 3*8;
        LinkOut3.NextLink = &LinkOut4;
        LinkOut3.Data = LinkOut2.Data + 3*8;
        LinkOut4.NextLink = &LinkOut5;
        LinkOut4.Data = LinkOut3.Data + 3*8;
        LinkOut5.NextLink = NULL;
        LinkOut5.Data = LinkOut4.Data + 3*8;

        rotate90(35,18,5,&LinkIn,&LinkOut,1,8);
        for(i = 0; i < sizeof(CheckInput7); i++)
            if(Output[i] != CheckInput7[i])
                DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input7 check i %x Expected %x Got %x\n",i,CheckInput7[i], Output[i]));
    }
    //
    // 2 bit rotate test
    //
    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input5;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;

    rotate90(12,5,3,&LinkIn,&LinkOut,2,300);
    for(i = 0; i < sizeof(CheckInput5); i++)
        if(Output[i] != CheckInput5[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input5 check i %x Expected %x Got %x\n",i,CheckInput5[i], Output[i]));
    //
    // 4 bit rotate test
    //
    for (i = 0; i < sizeof(Output); i++)
    {
        Output[i] = 0;
    }
    LinkIn.NextLink = NULL;
    LinkIn.Data = Input6;
    LinkOut.NextLink = NULL;
    LinkOut.Data = Output;

    rotate90(8,9,4,&LinkIn,&LinkOut,4,300);
    for(i = 0; i < sizeof(CheckInput6); i++)
        if(Output[i] != CheckInput6[i])
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error Failed Input6 check i %x Expected %x Got %x\n",i,CheckInput6[i], Output[i]));

}
#endif
