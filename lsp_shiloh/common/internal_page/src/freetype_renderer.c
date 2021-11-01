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
#include <dprintf.h>

#include "memAPI.h"
#include "lassert.h"
#include "renderer.h"
#include "freetype_api.h"
#include "debug.h"


#define CHARSIZE 8	        ///< Number of bits per byte	  

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

void FreetypeTextRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            UINT32 Color, 
                            UINT32 Rotate,
                            bool *strip_dirty,
                            uint32_t color_index)
{
  TEXT *text_obj;
  char *string; 
  freetype_glyph_data_t glyph;
  void* font;
  int y_max = 0;
  int y_min = 40000;
  int x_max = 0;
  char *fontfile = NULL;//"/tmp/test.ttf";
  int mono = 0; 
  uint32_t pixel_offset = get_pixel_offset(RendInput->packing);
  // 
  // if we are packed we have to add in the color for correct packing.
  if(RendInput->packing != IPAGE_PLANAR_INTERLEAVED &&
     RendInput->packing != IPAGE_PLANAR_ORDERED)
  {
      pixel_offset += color_index;
  }

  /* sanity check */
  if(!TempPage) return;
  /* if nothing on this plane since the test is mono, return */ 
  if((((TEXT *)TempPage->Object)->Color & Color) == 0) return ;   

  text_obj = (TEXT *)TempPage->Object;
  string   = text_obj->Data;  
  mono     = (RendInput->BPP==1);

  /* open the font.  NOTE: the FONT variable within TEXT is actualy the point 
     size */
  font = freetype_open_font(fontfile, text_obj->FONT, RendInput->X_Resolution, RendInput->Y_Resolution);

  /* Calculate the box that this text "object" will fit into.  Only do 
     this once since it will be stored for later calls */
  if (TempPage->OBJ_X_LEN==0xFFFFFFFF)  
  {
    int i;

    for(i=0; i<strlen(string);i++)
    {
      int s;
      /* load glyph to get the size */
      if (freetype_render_glyph(font, string[i], mono, &glyph)) continue; 
      if ((s = (TempPage->OBJ_Y - glyph.top)) < y_min) 
      {
        y_min = s;
        if(y_min < 0)
        { 
          y_min = 0;
        }
      }
      if ((s = (TempPage->OBJ_Y + (glyph.rows-glyph.top))) > y_max)
      {
        y_max = s;
      }
      x_max += glyph.advance_x;
    }
    /* just use these variables for storage, even though we are not using 
       then for their intended purpose..it works and is being done other places */
    TempPage->OBJ_X_LEN = y_min;
    TempPage->OBJ_Y_LEN = y_max; 

    if(text_obj->justify == IPAGE_JUSTIFY_RIGHT)
    {
      if(x_max < TempPage->OBJ_X)
      {
        TempPage->OBJ_X -= x_max; 
      }
    }
    else if(text_obj->justify == IPAGE_JUSTIFY_CENTER)
    {
      if(TempPage->OBJ_X > 0  && TempPage->OBJ_X > x_max/2)
      {
        TempPage->OBJ_X -= x_max/2; 
      }
    }
    else 
    {
      text_obj->justify = IPAGE_JUSTIFY_LEFT;
    }
  }
  else
  {
    /* get the y min and max variables from storage...this is not a typo...the 
       variables are not named correctly */
    y_min = TempPage->OBJ_X_LEN;
    y_max = TempPage->OBJ_Y_LEN;  
  }

  /* Check if any of the text fits within this strip */
  if ((pRSession->CurY <= y_max) && 
      ((pRSession->CurY + RendInput->OutBufLPS) >=  y_min))
  {
    int i;
    DISP_LIST *TempDispList;
    GRAPHIC *TempGraphic;
    int x_loc = 0;
    int start_y;
    int offset;
    int len;

    for(i=0;i<strlen(string);i++)
    {
      if (freetype_render_glyph(font, string[i], mono, &glyph)) continue;

      TempGraphic = (GRAPHIC *) MEM_CALLOC(1,sizeof(GRAPHIC)+ sizeof(DISP_LIST)); 
      ASSERT(TempGraphic != NULL);    // error check.
     
      /** fill out the graphic object */
      TempGraphic->X_DIMENSION = glyph.width;
      TempGraphic->Y_DIMENSION = glyph.rows;

      TempGraphic->LENGTH_X = glyph.pitch; 
      *(((UINT32 *) &(TempGraphic->K_OR_R_PLANE))+Color) = (uint32_t)glyph.data; 

      /* the display list is as was passed to us. */
      TempDispList = (DISP_LIST *)((UINT32)TempGraphic + sizeof(GRAPHIC));
      memcpy(TempDispList, TempPage, sizeof(DISP_LIST));

      /** fill out disp list with offsets */
      TempDispList->OBJ_X += x_loc + glyph.left;
      start_y = TempPage->OBJ_Y - glyph.top; 

      offset = 0;
      /* if start of character is before this stripe */
      if(start_y < pRSession->CurY)
      {
        offset = pRSession->CurY - start_y;    // how far to space into the character.
        start_y = pRSession->CurY;
      }
      /* figure length based on offset calculated above */
      len = glyph.rows - offset;
      /* limit output to this stripe only */
      if (((start_y + len)> (pRSession->CurY + RendInput->OutBufLPS)))
      {
        len = pRSession->CurY + RendInput->OutBufLPS - start_y;
      }
      /* make start_y relative to this stripe */
      start_y -= pRSession->CurY;
      /* clear so the chars don't get stretched */
      TempDispList->OBJ_X_LEN = 0; 
      TempDispList->OBJ_Y_LEN = 0;

      CopyObj(Strip, TempGraphic, Color, TempDispList, start_y, len, offset, 
              pRSession->PIXELLINE, 0, RendInput->BPP,
              get_pixel_stride(RendInput->packing), pixel_offset,
              RendInput->data_type);

      /* mark dirty */
      pRSession->Dirty = 1;
      *strip_dirty = 1; 

      x_loc += glyph.advance_x;
      MEM_FREE_AND_NULL(TempGraphic);
    }
  } 
  freetype_close_font(font);
}
