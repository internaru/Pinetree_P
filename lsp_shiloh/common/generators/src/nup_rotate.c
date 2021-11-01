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

#include <stddef.h>
#include <stdarg.h>
#include "memAPI.h"
#include "logger.h"
#include "lassert.h"
#include "list.h"
#include "generator_nup.h"
#include "ioutils.h"
#include "dma_buffer.h"

#define ROTSIZE 32      ///< Size of the rotation primitive.
#define BYTESIZE 8

// swap pixels end to end within a 32bit word, most significant
// pixel to least significant and vice versa
void nup_pixel_swap(uint32_t *p, uint32_t bpp, uint32_t mask)
{
    uint32_t v=0;
    uint32_t ii;
    for(ii=0; ii<32; ii+=bpp)
    {
        v |= ((*p << ii) & mask) >> (32 - bpp -ii);
    }
        
    *p = v;
}

#if 0
void validate_strip_list(ATLISTENTRY  *source)
{
    ATLISTENTRY* node;
    nup_data_strip_t* strip;
    uint32_t strip_num = 0;

//-jrs    dbg_printf("==> %s\n", __func__);

    node = ATListHead(source);
    while (node != NULL)
    {
        strip = CONTAINING_RECORD(node, nup_data_strip_t, node);
        if (strip->databuf == NULL)
        {
            dbg_printf("%s: strip %d @ %#x NULL DATABUF\n", __func__, strip_num, strip);
            ASSERT(strip->databuf != NULL);
        }
        if (strip->databuf->freeFunc == NULL || 
            (strip->databuf->data != NULL && strip->databuf->dma_alloc != NULL) ||
            (strip->databuf->data == NULL && strip->databuf->dma_alloc == NULL) ||
            strip->databuf->datalen > 15000000)
        {
            dbg_printf("%s: strip %d @ %#x CORRUPT\n", __func__, strip_num, strip);
            ASSERT(0);
        }

        node = ATListNext(source, node);
        strip_num++;
    }

//-jrs    dbg_printf("<== %s %d strips validated\n", __func__, strip_num);
}
#endif

/**
 * Function name: rotate
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
 * \param ByteWidth_unpadded The number of unpadded bytes in an 
 *        image row
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
void nup_rotate(uint32_t HorzPixels, uint32_t Rows, uint32_t ByteWidth_unpadded,
            ATLISTENTRY  *Source, ATLISTENTRY *Destination, 
            uint8_t BPP, nup_ROTATE_DIR Direction)
{
    uint32_t OutPutWidthBytes;
    uint32_t OutputRowCnt;
    uint32_t ExtraCol;        ///< Number of extra columns at the right
    uint32_t ExtraRows;       ///< number of extra rows at the bottom.
    uint32_t Mask=0;          ///< Bit mask for pulling and placing pixels.
    ATLISTENTRY *CurrentNode, *NextNode, *OutputNode;
    nup_data_strip_t *CurrentStripe, *NextStripe, *OutputStripe;
    // calculate byte width from given parameters
    UINT ByteWidth = (HorzPixels * BPP)/BYTESIZE;
    uint32_t num_rows = Rows;

    OutputRowCnt = 0;       // start at the beginning.
    ExtraRows = 0;
    ExtraCol = HorzPixels;
    NextStripe = NULL;

    dbg_printf("==> %s source = %#x, dest = %#x\n", __func__, Source, Destination);

#if 0
    if(Source != NULL)
    {
        validate_strip_list(Source);
    }
        
    if(Destination != NULL)
    {
        validate_strip_list(Destination);
    }
#endif
            
    OutPutWidthBytes = (Rows * BPP) / BYTESIZE;
    if(((Rows * BPP) % BYTESIZE) != 0) OutPutWidthBytes++; // make room for extra.

    if(OutPutWidthBytes % sizeof(uint32_t) != 0)
    {
        OutPutWidthBytes += sizeof(uint32_t) - (OutPutWidthBytes % sizeof(uint32_t));
        // reduce num_rows so we don't overrun the destination buffer
//        num_rows = (OutPutWidthBytes - BYTESIZE/BPP) * BYTESIZE/BPP;
        num_rows = (OutPutWidthBytes - sizeof(uint32_t)) * BYTESIZE/BPP;
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
        uint8_t DestData;
        uint8_t CurrentBit;
        int32_t LineLength;
        uint32_t Address;

        //
        //
        CurrentBit = 0;
        DestData = 0;
        LineLength = 0;
        OutputNode = ATListHead(Destination);
        ASSERT(OutputNode != NULL);
        OutputStripe = CONTAINING_RECORD(OutputNode, nup_data_strip_t, node);

        //
        // Do a 90 degree rotation.
        // What this does is take the first column, 32 bits at a time and
        // rotate it into the first row in the image.  We then go to the
        // next set of colums and rotate them into the next set of rows
        // in the output.  This continues till we are complete.
        //
        if (Direction == nup_Rotate90)
        {
            //dbg_printf("Rotate 90\n");
            uint32_t Output[ROTSIZE/BPP];
            uint32_t *MyInput, MyData, *MyOutput;
            uint32_t n, OutputRowCount=0;
            uint32_t InputRowCount, SaveBits, SaveLineLength;

            //
            // Figure the output line length.  We work in groups of 4 bytes so
            // this must end up a multiple of 4.  The stuff that slops over on
            // the end we take care of by setting current bit.  That will then
            // take the partial number of bits and put them into the output
            // buffer.
            //
            LineLength = (num_rows * BPP)/BYTESIZE;    // This must be in bytes
            CurrentBit = ((num_rows * BPP) % BYTESIZE);// how much is left over.  Partial bytes
            CurrentBit += (LineLength % sizeof(uint32_t)) * BYTESIZE; // Add in the
                                        // extra that comes because of rounding
                                        // to a multiple of 4.
            //
            // Adjust the line length to a multiple of 4.
            if((LineLength % sizeof(uint32_t)) != 0)
            {
                LineLength -= (LineLength % sizeof(uint32_t));
            }

            // first byte to grab is a word dist from end of line
            LineLength -= sizeof(uint32_t);

            // Figure where to start in the first word.
            if(CurrentBit != 0) CurrentBit = ROTSIZE - CurrentBit; 
            // Save the values for use later.
            SaveBits = CurrentBit;
            SaveLineLength = LineLength;

            //
            // Do this for each column.  We work with 32 bits of data
            // at a time so we increment by ROTSIZE/BPP.  Since that
            // gives the number of colums we are rotating at a time.
            //
            for (ColCount = 0; ColCount < ExtraCol ; ColCount+=ROTSIZE/BPP)
            {
                uint32_t ColShift;
#if 0
                validate_strip_list(Source);
                validate_strip_list(Destination);
#endif
                //
                // ColShift is used to make sure we access only the data needed
                // when the width of the rotated image is not exactly divisable
                // by 32.  First we figure out how many bits to ignore.
                ColShift = 0;
                if(ColCount == ExtraCol)
                {
                    ColShift = ROTSIZE/BPP - (ExtraCol % (ROTSIZE/BPP));  // number to ignore
                }
                MyOutput = (uint32_t *)dma_buffer_mmap_forcpu(OutputStripe->databuf);

                CurrentNode = ATListHead(Source);
                CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
                MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);
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
                for(RowCount = 0; RowCount < num_rows; RowCount++)
                {
                    //
                    // If our rowcount fources into a new stripe, get that
                    // stripe here.
                    //
                    if(InputRowCount >= CurrentStripe->num_lines)
                    {
                        dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
                        CurrentNode = ATListNext(Source, CurrentNode);
                        ASSERT(CurrentNode != NULL);
                        CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
                        InputRowCount = 0;
                        // Make sure the new strip is valid.
                        ASSERT(CurrentStripe != NULL);
                        // get the data pointer.
                        MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);
                    }
                    //
                    // Now figure the source address for the data.  Start at the stripe boundry,
                    // add in the offset to the data. Then space down by the column offset.
                    //
                    Address = (((uint32_t)MyInput + 
                                (InputRowCount) * ByteWidth + 
                                ((ColCount * BPP))/BYTESIZE) );

                    //
                    // Everything should come in aligned to 4 bytes.  However
                    // if not get single bytes and merge them into a word.
                    // else just load the word.
                    //
                    if (Address & 0x3)
                    {
                        #ifdef CPU_LITTLE_ENDIAN
                        MyData = (*(uint8_t *)Address);
                        MyData |= (*(((uint8_t *)Address)+1))<<8;
                        MyData |= (*(((uint8_t *)Address)+2))<<16;
                        MyData |= (*(((uint8_t *)Address)+3))<<24;
                        #else
                        MyData = (*(uint8_t *)Address)<<24;
                        MyData |= (*(((uint8_t *)Address)+1))<<16;
                        MyData |= (*(((uint8_t *)Address)+2))<<8;
                        MyData |= (*(((uint8_t *)Address)+3));
                        #endif
                    }
                    else
                    {
                        MyData = be32_to_cpu(*((uint32_t *)Address));
                    }

                    //
                    // This for loop does the actual rotate.
                    //
                    for(n = ColShift; n < ROTSIZE/BPP; n++)
                    {
                        // n will step across the 32 bits from the Highest to lowest
                        // we then mask off only the data we are interested in and
                        // then we shift it into the correct position for storage.
                        // then we OR that data into the output buffer.
                        //
                        Output[n - ColShift] |= 
                             ((MyData << ((n)* BPP)) & Mask)
                                >> (ROTSIZE - BPP - CurrentBit); 
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
                        NextNode = NULL;
                        NextStripe = NULL;
                        uint32_t* old_output = NULL;
                        //
                        // Write the data to the output buffer.
                        //
                        for(n = 0; n < ROTSIZE/BPP; n++)
                        {
                            //
                            // Check to see if this write will force us to the next
                            // strip.  If so get the next strip.
                            //

                            if((n + OutputRowCount) >= OutputStripe->num_lines && NextStripe == NULL)
                            {
                                NextNode = ATListNext(Destination, OutputNode);
                                ASSERT(NextNode != NULL);
                                NextStripe = CONTAINING_RECORD(NextNode, nup_data_strip_t, node);
                                old_output = MyOutput;
                                MyOutput = dma_buffer_mmap_forcpu(NextStripe->databuf);
                            }
                            Address = (((uint32_t) MyOutput +
                              ((n+OutputRowCount)% OutputStripe->num_lines) *
                               OutPutWidthBytes + LineLength));

                            // See if we have done all the rows.
                            // horzpixels now is the height
                            if((n + OutputRowCount) >= HorzPixels)  
                            {
                                break;
                            }
                            // Write the output data
                            *(uint32_t *)Address = cpu_to_be32(Output[n]);
                            // CLear the buffer for the next pass.
                            Output[n] = 0;
                        }
                        // Restart at the beginning of the next word.
                        CurrentBit = 0;

                        // decrement the line length for the next pass.
                        LineLength -= (ROTSIZE)/BYTESIZE; // 8 is bits per byte
                        if(LineLength < 0)
                        {
                            break;
                        }

                        // reset the output to the beginning for the next pass.
                        if (NextStripe != NULL)
                        {
                            dma_buffer_unmmap_forcpu(NextStripe->databuf);
                            NextStripe = NULL;
                            MyOutput = old_output;
                        }
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
                    NextNode = NULL;
                    NextStripe = NULL;
                    uint32_t* old_output = NULL;

                    for(n = 0; n < ROTSIZE/BPP; n++)
                    {
                        //
                        // Check to see if this write will force us to the next
                        // strip.  If so get the next strip.
                        //
                        if((n + OutputRowCount) >= OutputStripe->num_lines && NextStripe == NULL)
                        {
                            NextNode = ATListNext(Destination, OutputNode);
                            ASSERT(NextNode != NULL);
                            NextStripe = CONTAINING_RECORD(NextNode, nup_data_strip_t, node);
                            old_output = MyOutput;
                            MyOutput = dma_buffer_mmap_forcpu(NextStripe->databuf);
                        }
                        Address = (((uint32_t) MyOutput + 
                         ((n+OutputRowCount)% OutputStripe->num_lines) *
                          OutPutWidthBytes + LineLength));

                        //
                        // See if we have done all the rows.
                        //
                        if((n + OutputRowCount) >= HorzPixels)  // horzpixels now is the height
                            break;
                        // Write the output data
                        *(uint32_t *)Address = cpu_to_be32(Output[n]);
                        // CLear the buffer for the next pass.
                        Output[n] = 0;
                    }

                    if (NextStripe != NULL)
                    {
                        dma_buffer_unmmap_forcpu(NextStripe->databuf);
                        MyOutput = old_output;
                    }
                }

                OutputRowCount+= ROTSIZE/BPP - ColShift;
                LineLength = SaveLineLength;
                CurrentBit = SaveBits;  // if we are here then we are not on a short xfer and can set this to 0.

                if (NextStripe != NULL)
                {
                    dma_buffer_unmmap_forcpu(NextStripe->databuf);
                }
                dma_buffer_unmmap_forcpu(OutputStripe->databuf);
                dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
                if(OutputRowCount > OutputStripe->num_lines)
                {
                    OutputRowCount-= OutputStripe->num_lines;
                    OutputNode = ATListNext(Destination, OutputNode);
                    ASSERT(OutputNode != NULL);
                    OutputStripe = CONTAINING_RECORD(OutputNode, nup_data_strip_t, node);
                    ASSERT(OutputStripe != NULL);
                }
#if 0
                validate_strip_list(Source);
                validate_strip_list(Destination);
#endif
            }
        }

        //
        // The only difference between this rotation and the one above is
        // that we start at the highest column and top row pulling bits.
        // Then we dec the column and continue till the end.  Detailed
        // comments see the above section.
        //
        else if (Direction == nup_Rotate270)
        {
            //dbg_printf("Rotate 270\n");
            uint32_t Output[ROTSIZE/BPP];
            uint32_t *MyInput, MyData, *MyOutput;
            uint32_t n, OutputRowCount=0;
            uint32_t InputRowCount;

            //compensate for the padding on the right side of image
            ExtraCol -= ByteWidth - ByteWidth_unpadded;
            
            // Do this for each column.  Start at the last colum and
            // rotate into the top column of the destination.
            for (ColCount = ExtraCol; ColCount > 0 ; ColCount-=ROTSIZE/BPP)
            {
                uint32_t ColShift;

                //
                // ColShift is used to make sure we access only the data needed
                // when the width of the rotated image is not exactly divisable
                // by 32.  First we figure out how many bits to ignore.
                ColShift = 0;
                if(ColCount == ExtraCol)
                    ColShift = ROTSIZE/BPP - (ExtraCol % (ROTSIZE/BPP));  // number to ignore
                CurrentNode = ATListHead(Source);
                CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
                MyOutput = (uint32_t *)dma_buffer_mmap_forcpu(OutputStripe->databuf);

                // get the data pointer to the input strip.
                MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);
                InputRowCount = 0;

                //
                // Clear the output buffer.
                //
                for(RowCount = 0; RowCount < ROTSIZE/BPP; RowCount++)
                    Output[RowCount] = 0;
                // We have selected a column to scan.  Go down
                // the source column by rows till we reach the last column.
                //
                for(RowCount = 0; RowCount < num_rows; RowCount++)
                {
                    uint32_t Address;
                    //
                    // See if we need to go to the next input strip.
                    //
                    if(InputRowCount >= CurrentStripe->num_lines)
                    {
                        dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
                        CurrentNode = ATListNext(Source, CurrentNode);
                        ASSERT(CurrentNode != NULL);
                        CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
                        InputRowCount = 0;
                        // Make sure the new strip is valid.
                        ASSERT(CurrentStripe != NULL);
                        // get the data pointer.
                        MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);
                    }
                    //
                    // Figure the address, we will access in 32 bit chuncks.
                    // so we need to make sure the address always increments
                    // by 4 due tot he colcount.  If the address due to bytewidth
                    // does not increment by 4, get the data in pieces.
                    //
                    Address = (uint32_t)MyInput +
                                (InputRowCount) * ByteWidth +
                                (((ColCount * BPP)/BYTESIZE)/4)*4;

                    //
                    // Everything should come in aligned to 4 bytes.  However
                    // if not get single bytes and merge them into a word.
                    // else just load the word.
                    //
                    if (Address & 0x3)
                    {
                        #ifdef CPU_LITTLE_ENDIAN
                        MyData = (*(uint8_t *)Address);
                        MyData |= (*(((uint8_t *)Address)+1))<<8;
                        MyData |= (*(((uint8_t *)Address)+2))<<16;
                        MyData |= (*(((uint8_t *)Address)+3))<<24;
                        #else
                        MyData = (*(uint8_t *)Address)<<24;
                        MyData |= (*(((uint8_t *)Address)+1))<<16;
                        MyData |= (*(((uint8_t *)Address)+2))<<8;
                        MyData |= (*(((uint8_t *)Address)+3));
                        #endif
                    }
                    else
                    {
                        MyData = be32_to_cpu(*((uint32_t *)Address));
                    }

                    //
                    // This for loop does the actual rotate.
                    //
                    for(n = ColShift; n < ROTSIZE/BPP; n++)
                    {
                        // n will step across the 32 bits from the Highest to lowest
                        // we then mask off only the data we are interested in and
                        // then we shift it into the correct position for storage.
                        // then we OR that data into the output buffer.
                        //
                        Output[n - ColShift] |= 
                          ((MyData << ((ROTSIZE/BPP-n-1)* BPP)) & Mask)
                           >> CurrentBit;
                    }
                    // This row in the current column is complete.  
                    // Change to the next column
                    CurrentBit += BPP;
                    // Write the data out if the output buffer is full.
                    // When the CurrentBit reaches a word size the buffer is full
                    // and must be written out.
                    //
                    if(CurrentBit == ROTSIZE) // see if we need to store the stuff.
                    {
                        NextNode = NULL;
                        NextStripe = NULL;
                        uint32_t* old_output = NULL;
                        for(n = 0; n < ROTSIZE/BPP; n++)
                        {
                            // if we are larger than a stripe, get the next one.
                            if((n + OutputRowCount) >= OutputStripe->num_lines && NextStripe == NULL)
                            {
                                NextNode = ATListNext(Destination, OutputNode);
                                ASSERT(NextNode != NULL);
                                NextStripe = CONTAINING_RECORD(NextNode, nup_data_strip_t, node);
                                old_output = MyOutput;
                                MyOutput = dma_buffer_mmap_forcpu(NextStripe->databuf);
                            }
                            //
                            // Figure the address of the destination of the data.
                            // Start at the beginning of the buffer, index in
                            // by the number of lines times the line length.
                            // last add in the current offset into the line.
                            //
                            Address = (((uint32_t) MyOutput + 
                              ((n+OutputRowCount) % OutputStripe->num_lines) * 
                               OutPutWidthBytes + LineLength));

                            //
                            // See if we have done all the rows, if we are done
                            // get out of here, the store is complete.
                            //
                            if((n + OutputRowCount) > HorzPixels)  // horzpixels now is the height
                                break;

                            //*(uint32_t *)Address = Output[n]; // store the output data.
                            *(uint32_t *)Address = cpu_to_be32(Output[n]);
                            Output[n] = 0;      // clear the temp buffer for the next pass.
                        }

                        CurrentBit = 0;     // everything is stored. reset output pointer.
                        LineLength+= (ROTSIZE)/BYTESIZE; // 8 is bits per byte move to the next
                                                  // address in the line.

                        // reset the output to the beginning for the next pass.
                        if (NextStripe != NULL)
                        {
                            dma_buffer_unmmap_forcpu(NextStripe->databuf);
                            MyOutput = old_output;
                        }

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
                    NextNode = NULL;
                    NextStripe = NULL;
                    uint32_t* old_output = NULL;
                    for(n = 0; n < ROTSIZE/BPP; n++)
                    {
                        //
                        // Check to see if this write will force us to the next
                        // strip.  If so get the next strip.
                        //
                        if((n + OutputRowCount) >= OutputStripe->num_lines && NextStripe == NULL)
                        {
                            NextNode = ATListNext(Destination, OutputNode);
                            ASSERT(NextNode != NULL);
                            NextStripe = CONTAINING_RECORD(NextNode, nup_data_strip_t, node);
                            old_output = MyOutput;
                            MyOutput = dma_buffer_mmap_forcpu(NextStripe->databuf);
                        }
                        Address = (((uint32_t) MyOutput + ((n+OutputRowCount)% OutputStripe->num_lines) * OutPutWidthBytes +
                                  LineLength));

                        //
                        // See if we have done all the rows.
                        //
                        if((n + OutputRowCount) > HorzPixels)  // horzpixels now is the height
                            break;
                        // Write the output data
                        *(uint32_t *)Address = cpu_to_be32(Output[n]);
                        // CLear the buffer for the next pass.
                        Output[n] = 0;
                    }
                    if (NextStripe != NULL)
                    {
                        dma_buffer_unmmap_forcpu(NextStripe->databuf);
                        MyOutput = old_output;
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
                dma_buffer_unmmap_forcpu(OutputStripe->databuf);
                dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
                if(OutputRowCount > OutputStripe->num_lines)
                {
                    OutputRowCount-= OutputStripe->num_lines;
                    OutputNode = ATListNext(Destination, OutputNode);
                    ASSERT(OutputNode != NULL);
                    OutputStripe = CONTAINING_RECORD(OutputNode, nup_data_strip_t, node);
                    ASSERT(OutputStripe != NULL);
                }
            }
        }

        // 180 rotation swaps rows vertically and pixels horizontally
        // all within the source data list.   the destination data
        // list is not used
        else if (Direction == nup_Rotate180)
        {
            //dbg_printf("Rotate 180\n");
            uint32_t ridx=0;
            ATLISTENTRY *top_node=0, *bottom_node=0;
            nup_data_strip_t *top_strip=0, *bottom_strip=0, *last_top_strip=0;
            uint32_t *top_data; uint32_t *bottom_data;
            uint32_t word_width=ByteWidth/sizeof(uint32_t);
            uint32_t word_width_unpadded=
                       ByteWidth_unpadded/sizeof(uint32_t);
            uint32_t *top_data_ceiling, *bottom_data_floor, *top_data_start;
            uint32_t cidx=0;

            if(ByteWidth_unpadded % sizeof(uint32_t) != 0)
            {
                word_width_unpadded++;
            }

            // initialize top data pointer with first row
            top_node = ATListHead(Source);
            top_strip = CONTAINING_RECORD(top_node, nup_data_strip_t, node);
            top_data = (uint32_t*)dma_buffer_mmap_forcpu(top_strip->databuf);
            top_data_start = top_data;
            top_data_ceiling = top_data + (word_width * top_strip->num_lines);
                                
            // initialize bottom data pointer with last row
            bottom_node = ATListTail(Source);
            bottom_strip = CONTAINING_RECORD(bottom_node, nup_data_strip_t, node);            
            bottom_data_floor=(uint32_t*)dma_buffer_mmap_forcpu(bottom_strip->databuf);
            bottom_data = bottom_data_floor + 
                     (word_width * bottom_strip->num_lines) -1;

            for(ridx=0; ridx<=Rows; ridx++)
            {
                if(top_data >= top_data_ceiling)
                {
                    // unmap if both are not pointing to the same strip
                    if (top_strip != bottom_strip)
                    {
                        dma_buffer_unmmap_forcpu(top_strip->databuf);
                    }

                    // increment to next data chunk
                    last_top_strip = top_strip;
                    top_node = ATListNext(Source, top_node);
                    top_strip = CONTAINING_RECORD(top_node, nup_data_strip_t, node);
                    if (top_strip != bottom_strip)
                    {
                        top_data = (uint32_t*)dma_buffer_mmap_forcpu(top_strip->databuf);
                    }
                    else
                    {
                        // buffer already mapped in as bottom so just point to it
                        top_data = bottom_data_floor;
                    }
                    top_data_ceiling = top_data + (word_width * top_strip->num_lines);
                    top_data_start = top_data;
                }

                //if(bottom_row < bottom_rows)
                if(bottom_data < bottom_data_floor)
                {
                    // unmap if both are not pointing to the same strip
                    if (top_strip != bottom_strip)
                    {
                        dma_buffer_unmmap_forcpu(bottom_strip->databuf);
                    }
                    // decrement to next data chunk
                    bottom_node = ATListPrev(Source, bottom_node);
                    bottom_strip = CONTAINING_RECORD(bottom_node, nup_data_strip_t, node);
                    if (top_strip != bottom_strip)
                    {                                
                        bottom_data_floor=(uint32_t*)dma_buffer_mmap_forcpu(bottom_strip->databuf);
                    }
                    else
                    {
                        // buffer already mapped in as top so just point to it
                        bottom_data_floor = top_data_start;
                    }
                    bottom_data = bottom_data_floor + (word_width * bottom_strip->num_lines) -1;
                }

                // white out padded area and adjust data pointers
                for(cidx=0; cidx<word_width-word_width_unpadded; cidx++)
                {
                    *bottom_data = 0;  // white out padded area
                    bottom_data--;
                }

                // when the increasing top data pointer crosses the
                // decreasing bottom data pointer we are done
                if((top_strip == bottom_strip &&
                            top_data > bottom_data) ||
                            bottom_strip == last_top_strip)
                {
                    //dbg_printf(
                        //"Data pointers crossed. We are done. ridx=%d\n", 
                        //ridx);
                    break;
                }

                // swap pixels horizontally from src row into dest row
                for(cidx=0; cidx<word_width_unpadded; cidx++)
                                   //cidx<word_width; cidx++)
                {
                    // boundary check to preclude memory corruption
                    if(top_data>=top_data_ceiling)
                    {
                        dbg_printf(
                            "!! top_dat hit ceiling, ridx:%d cidx:%d\n",
                            ridx, cidx);
                        break;
                    }
                    if(bottom_data<bottom_data_floor) 
                    {
                        dbg_printf(
                            "!!bottom_data hit floor, ridx:%d cidx:%d\n",
                            ridx, cidx);
                        break;
                    }

                    uint32_t tmp1, tmp2;
                    tmp1 = be32_to_cpu(*top_data);
                    nup_pixel_swap(&tmp1, BPP, Mask);
                    tmp2 = be32_to_cpu(*bottom_data);
                    nup_pixel_swap(&tmp2, BPP, Mask);
                    *top_data = cpu_to_be32(tmp2);
                    *bottom_data = cpu_to_be32(tmp1);
                    top_data ++;
                    bottom_data--;
                }

                // white out padded area and adjust data pointers
                for(; cidx<word_width; cidx++)
                {
                    *top_data = 0;  // white out padded area
                    top_data++;
                }

                // move pointer past the end-of-line padding
                //top_data += (word_width - word_width_unpadded);
            }
            // always unmap top
            dma_buffer_unmmap_forcpu(top_strip->databuf);
            // unmap bottom if top and bottom not equal
            if (bottom_strip != top_strip)
            {
                dma_buffer_unmmap_forcpu(bottom_strip->databuf);
            }
        }
        else
        {
            //dbg_printf("Rotation direction %d is not supported\n", 
                        //Direction);
            ASSERT(0);
        }
    }
#if 0
    if(Source != NULL)
    {
        validate_strip_list(Source);
    }

    if(Destination != NULL)
    {
        validate_strip_list(Destination);
    }
#endif
    dbg_printf("<== %s source = %#x, dest = %#x\n", __func__, Source, Destination);
}

