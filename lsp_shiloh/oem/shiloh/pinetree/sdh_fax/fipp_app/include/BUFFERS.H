/*
 *  $RCSfile: BUFFERS.H,v $
 * $Revision: 1.3 $
 *   $Author: miurat $
 *     $Date: 2009/09/25 18:45:38 $
 */
/************************************************************************
 *
 *      BUFFERS.H
 *
 *  Definitions for bitmaps and coded data buffers (CDBs) as
 *  used in the general system.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef BUFFERS_H
#define BUFFERS_H

#define BITMAP_MAX_PLANES 6
#define BITMAP_PLANE_K  0
#define BITMAP_PLANE_C  1
#define BITMAP_PLANE_M  2
#define BITMAP_PLANE_Y  3
#define BITMAP_PLANE_LC 4
#define BITMAP_PLANE_LM 5

#define BITMAP_PLANE_R  0
#define BITMAP_PLANE_G  1
#define BITMAP_PLANE_B  2


typedef struct scanline_marks
{
   UINT16   first;   /* first non-zero PIXEL in the scanline */
   UINT16   last;    /* last non-zero PIXEL in the scanline */
} MARKS;


typedef struct bitmapStruct
{

       /* This is a place to add any special parameters.
          This should be used by anyone other than the
          original creator of the bitmaps as a stack.
          Therefore the previous value should be saved
          as part of any structure pointed to by this
          and restored before the bitmap is returned. */
   void * additional_params;

   struct bitmapStruct *next;

       /*
         Parameters are the pointer to this bitmap, a flag
         indicating if this is being returned due to a
         returnBitmaps command, and any associated jobIDs
         of the called module.
         */
   void (*notify) (struct bitmapStruct *this_bitmap,
                   BOOL by_request, UINT8 jobID);

       /* Data area for monochrome data.  With color data this
          can be set to point to any plane for handling by any
          monochrome-only modules */
   UINT32 *data;

   BOOL end_of_page;

       /* No of filled lines in bitmap */
   UINT16 img_lines;

       /* No of total (actual) lines allocated in bitmap. */
   UINT16 max_lines;

       /* Width of a line in pixels */
   UINT16 width;

       /* Flags */
   BOOL all_white_data;

       /* Warp factor to be used in 32 bits.  Note that this is
          used to assure that each line starts at a 32-bit boundary. */
   UINT32 warp;

       /* Array of MARKS structures telling first and last marks for
          each line of data.  Array should be allocated when the bitmap is.
          Access is probably best as bitmap->mark[line].first,etc */
   MARKS  *mark;

       /* Array of data areas for different color planes */
   UINT32 *color_data[BITMAP_MAX_PLANES];

       /* Array of MARKS structure arrays for different color planes */
   MARKS  *color_mark[BITMAP_MAX_PLANES];


} BITMAP;


typedef struct CDBStruct
{

       /* This is a place to add any special parameters.
          This should be used by anyone other than the
          original creator of the CDBs as a stack.
          Therefore the previous value should be saved
          as part of any structure pointed to by this
          and restored before the CDB is returned. */
   void * additional_params;

   struct CDBStruct *next;

       /*
         Parameters are the pointer to this CDB, a flag
         indicating if this is being returned due to a
         returnCDBs command, and any associated jobIDs
         of the calling module (eg. T4Cx).
         */
   void (*notify) (struct CDBStruct *this_cdb,
                   BOOL by_request, UINT8 jobID);

      /* pointer to base of data buffer */
   UINT8 *data;

   BOOL end_of_page;

       /* Number of filled bytes in the CDB */
   UINT16 size;

       /* Amount of available bytes allocated in the CDB */
   UINT16 max_size;

      /* byte offset to beginning of unconsumed data */
   UINT16 consumed;

} CDB;



/* Something to make the function protypes cleaner */
#define CDB_NOTIFY_FN void (*notify) (CDB *cdb, BOOL by_request, UINT8 jobID)
#define BITMAP_NOTIFY_FN void (*notify) (BITMAP *bitmap, BOOL by_request, UINT8 jobID)


#endif /* BUFFERS_H */
