/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipeTable.c
 *
 *
 *****************************************************************************/

#include <stdint.h>
#include "ATypes.h"
#include "agJobUtils.h"
#include "agResourceMgr.h"
#include "ColorPipeTables.h"
#if 0
#include "ColorPipeTablePriv.h"
#endif
#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include <stdlib.h>
#include "memAPI.h"

#include "acl_api.h"

#include "string.h"
#if 0
static csc_table_t * download_override_csc_table = NULL;
static cal_halftone_set_t * download_override_HT_table = NULL;
static mlimiter_t * color_table_mlimiter = NULL;
#endif
static struct 
{
    input_color_space_t input_space;
    uint32_t output_bits_per_pixel;
    halftone_type_t halftone_type;
    mediatype_t page_type;
    mediasize_t page_size;
} color_table_selection;

void color_table_set_defaults( void )
{
#ifdef COLOR_ENG
    color_table_selection.input_space = e_sRGB;
#else
    color_table_selection.input_space = e_additive_gray;
#endif    
    color_table_selection.output_bits_per_pixel = 2;
    color_table_selection.halftone_type = e_ht_default;
    color_table_selection.page_type = MEDIATYPE_PLAIN_PAPER;
    color_table_selection.page_size = MEDIASIZE_LETTER;
}

void color_table_set_input_space(input_color_space_t input_space)
{
    color_table_selection.input_space = input_space;
}

void color_table_set_output_bits_per_pixel( uint32_t output_bits_per_pixel )
{
    color_table_selection.output_bits_per_pixel = output_bits_per_pixel;
}

void color_table_set_halftone_type( halftone_type_t halftone_type )
{
    color_table_selection.halftone_type = halftone_type;
}

void color_table_set_page_type( mediatype_t page_type )
{
    color_table_selection.page_type = page_type;
}

void color_table_set_page_size( mediasize_t page_size )
{
    color_table_selection.page_size = page_size;
}
#if 0
static csc_table_t * colortable_get_override_dl_table(void)
{
    return download_override_csc_table;
}
#endif
bool  CSC_table_add_override_dl(uint8_t location,
                                uint8_t in_dim,
                                uint8_t out_dim,
                                uint8_t table_dim,
                                uint8_t * data_buffer) //false if ok
{
#if 0
    int i;
    bool new_table = (download_override_csc_table == NULL);

    ASSERT(data_buffer);

    //alloc override table, if it doesn't exist
    if (new_table) 
    {
        download_override_csc_table = (csc_table_t *) MEM_MALLOC_LIMITED(color_table_mlimiter, sizeof(csc_table_t)* NUM_COLORGROUP_ENTRIES );
        for (i = MONO_LUT_INDEX; i < NUM_COLORGROUP_ENTRIES; i++) 
        {
            download_override_csc_table[i].table_data = NULL; 
        }
    }

    //check if overwriting a table
    //because we don't save the size of the saved csc_table, it is better to dealloc and alloc new table
    if (download_override_csc_table[location].table_data != NULL) 
    {
        //keeps the compiler from complaining
        uint8_t * tmpptr;
        tmpptr = (uint8_t *) download_override_csc_table[location].table_data;
        memFree(tmpptr);
        download_override_csc_table[location].table_data = NULL;
    }

    
    download_override_csc_table[location].in_dim = in_dim;
    download_override_csc_table[location].out_dim = out_dim;
    download_override_csc_table[location].table_dim = table_dim;
    download_override_csc_table[location].table_data = data_buffer;
#endif   
    return false;
}

void CSC_table_delete_dl(void) //This assumes that add/delete_dl are done sync, in the data stream, using PJL.
                               //If you delete async, while a page is being ripped, you will probably get a garbage page out.

{
#if 0
    int i;
    if (download_override_csc_table) 
    {
        for (i = MONO_LUT_INDEX; i < NUM_COLORGROUP_ENTRIES; i++) 
        {
            if (download_override_csc_table[i].table_data != NULL) 
            {
                //keeps the compiler from complaining
                uint8_t * tmpptr;
                tmpptr = (uint8_t *) download_override_csc_table[i].table_data;
                memFree(tmpptr);
                download_override_csc_table[i].table_data = NULL;
            }
        }
        MEM_FREE_AND_NULL(download_override_csc_table);
    }
#endif
}
#if 0
csc_table_t * CSC_table_get( bool * inv_grey )
{

    csc_table_t * color_table;
    csc_table_t * dl_color_table;
    int i;
    static csc_table_t override_colortable[NUM_COLORGROUP_ENTRIES];

    if (color_table_selection.input_space == e_subtractive_grey) 
    {
        *inv_grey = true;
    }
    else
    {
        *inv_grey = false;
    }
    color_table = get_colortable(color_table_selection.input_space, color_table_selection.page_type, color_table_selection.page_size);
    dl_color_table = colortable_get_override_dl_table();
    
    if (dl_color_table) //user has downloaded at least part of the colortable
    {
        for (i = MONO_LUT_INDEX; i < NUM_COLORGROUP_ENTRIES; i++) 
        {
            if (dl_color_table[i].table_data == NULL) //fill in the empty slots
            {
                override_colortable[i].table_data = color_table[i].table_data;
                override_colortable[i].in_dim = color_table[i].in_dim;
                override_colortable[i].out_dim = color_table[i].out_dim;
                override_colortable[i].table_dim = color_table[i].table_dim;
            }
            else
            {
                override_colortable[i].table_data = dl_color_table[i].table_data;
                override_colortable[i].in_dim = dl_color_table[i].in_dim;
                override_colortable[i].out_dim = dl_color_table[i].out_dim;
                override_colortable[i].table_dim = dl_color_table[i].table_dim;
            }
        }
        return(  override_colortable );
    }
    return(  color_table );

}
#endif
#if 0
static ht_table_t * HT_get_override_dl_table(uint32_t bits_per_pixel)
{

    if (!download_override_HT_table) 
    {
        return NULL;
    }
    if (bits_per_pixel == 1) 
    {
        return &download_override_HT_table[0];
    }
    else
    {
        return &download_override_HT_table[1];
    }
}
#endif
bool HT_table_add_override_dl(uint8_t bits_per_pixel,
                              uint8_t color_plane,
                              uint16_t x_dim,
                              uint16_t y_dim,
                              uint8_t num_screens,
                              uint8_t * data_buffer)
{
#if 0
    int i,j;
    halftone_screen_t * screen_data;

    ASSERT(data_buffer);
    

    //alloc override table, if it doesn't exist
    if (download_override_HT_table == NULL) 
    {
        download_override_HT_table = (cal_halftone_set_t *) MEM_MALLOC_LIMITED(color_table_mlimiter, sizeof(cal_halftone_set_t)* 2/*support 1 & 2 bpp*/ );
        for (i = 0; i < 2; i++) 
        {
            for (j = 0; j < 3; j++) 
            {
                download_override_HT_table[i].htScreenC.pScreenDataCal[j] = NULL;
                download_override_HT_table[i].htScreenM.pScreenDataCal[j] = NULL;
                download_override_HT_table[i].htScreenY.pScreenDataCal[j] = NULL;
                download_override_HT_table[i].htScreenK.pScreenDataCal[j] = NULL;
            }
        }
    }

    //check if overwriting a table
    //because we don't save the size of the saved ht_table, it is better to dealloc and alloc new table
    screen_data = (color_plane == 0)? &download_override_HT_table[bits_per_pixel - 1].htScreenC:
                  ((color_plane == 1)? &download_override_HT_table[bits_per_pixel - 1].htScreenM:
                  ((color_plane == 2)? &download_override_HT_table[bits_per_pixel - 1].htScreenY:
                                      &download_override_HT_table[bits_per_pixel - 1].htScreenY));
    if (screen_data->pScreenDataCal[0] != NULL) 
    {
        //keeps the compiler from complaining
        uint8_t * tmpptr;
        tmpptr = (uint8_t *) screen_data->pScreenDataCal[0];
        memFree(tmpptr);
        screen_data->pScreenDataCal[0] = NULL;
    }

    
    screen_data->xDim = x_dim;
    screen_data->yDim = y_dim;
    screen_data->bitDepth = bits_per_pixel;
    screen_data->numScreens = num_screens;
    screen_data->pScreenDataCal[0] = data_buffer;
    if (num_screens == 3) 
    {
        screen_data->pScreenDataCal[1] = &data_buffer[x_dim * y_dim];
        screen_data->pScreenDataCal[2] = &data_buffer[x_dim * 2 * y_dim];
    }
#endif
    return false;
}

void HT_table_delete_dl(void) //This assumes that add/delete_dl are done sync, in the data stream, using PJL.
                               //If you delete async, while a page is being ripped, you will probably get a garbage page out.

{
#if 0
    int i;
    if (download_override_HT_table) 
    {
        for (i = 0; i < 2; i++) 
        {
            if (download_override_HT_table[i].htScreenC.pScreenDataCal[0] != NULL)
            {
                //keeps the compiler from complaining
                uint8_t * tmpptr;
                tmpptr = (uint8_t *) download_override_HT_table[i].htScreenC.pScreenDataCal[0];
                memFree(tmpptr);
                download_override_HT_table[i].htScreenC.pScreenDataCal[0] = NULL;
            }
            if (download_override_HT_table[i].htScreenM.pScreenDataCal[0] != NULL)
            {
                //keeps the compiler from complaining
                uint8_t * tmpptr;
                tmpptr = (uint8_t *) download_override_HT_table[i].htScreenM.pScreenDataCal[0];
                memFree(tmpptr);
                download_override_HT_table[i].htScreenM.pScreenDataCal[0] = NULL;
            }
            if (download_override_HT_table[i].htScreenY.pScreenDataCal[0] != NULL)
            {
                //keeps the compiler from complaining
                uint8_t * tmpptr;
                tmpptr = (uint8_t *) download_override_HT_table[i].htScreenY.pScreenDataCal[0];
                memFree(tmpptr);
                download_override_HT_table[i].htScreenY.pScreenDataCal[0] = NULL;
            }
            if (download_override_HT_table[i].htScreenK.pScreenDataCal[0] != NULL)
            {
                //keeps the compiler from complaining
                uint8_t * tmpptr;
                tmpptr = (uint8_t *) download_override_HT_table[i].htScreenK.pScreenDataCal[0];
                memFree(tmpptr);
                download_override_HT_table[i].htScreenK.pScreenDataCal[0] = NULL;
            }
        }
        MEM_FREE_AND_NULL(download_override_HT_table);
    }
#endif
}
#if 0
ht_table_t * HT_table_get( void )
{

    ht_table_t * ht_table;
    ht_table_t * dl_ht_table;
    static ht_table_t override_HT_table;
    ht_table = get_color_calibrated_halftones( color_table_selection.output_bits_per_pixel, 
                                             color_table_selection.halftone_type, color_table_selection.page_type, 
                                             color_table_selection.page_size );
    dl_ht_table = HT_get_override_dl_table(color_table_selection.output_bits_per_pixel);
    if (dl_ht_table) 
    {
        // structure copy, NOT pointer copy
        // if dl copy exists use it, else use existing copy.
        memcpy(&override_HT_table.htScreenC,
               (dl_ht_table->htScreenC.pScreenDataCal[0])? &dl_ht_table->htScreenC:&ht_table->htScreenC,
               sizeof(halftone_screen_t));
        memcpy(&override_HT_table.htScreenM,
               (dl_ht_table->htScreenM.pScreenDataCal[0])? &dl_ht_table->htScreenM:&ht_table->htScreenM,
               sizeof(halftone_screen_t));
        memcpy(&override_HT_table.htScreenY,
               (dl_ht_table->htScreenY.pScreenDataCal[0])? &dl_ht_table->htScreenY:&ht_table->htScreenY,
               sizeof(halftone_screen_t));
        memcpy(&override_HT_table.htScreenK,
               (dl_ht_table->htScreenY.pScreenDataCal[0])? &dl_ht_table->htScreenY:&ht_table->htScreenY,
               sizeof(halftone_screen_t)); 
        return &override_HT_table;
    }
    return ht_table;

}
#endif
SINT32 color_table_dl_CSC_table(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
#if 0
    uint8_t * data_buffer;
    SINT32 bytes_read;
    uint32_t size;

    if (!(size = AclCmd->data.downloadCSCTable.data_length)) 
    {
        return(1);
    }

    if((data_buffer = (uint8_t *) MEM_MALLOC_LIMITED(color_table_mlimiter, size )) == NULL)
    {
        /* do I need to try and dump data?*/
        return(1);
    }
    
    if ((bytes_read = Pipe->ReadConnect(Pipe, data_buffer, size, PARSEREAD_EXACT )) <= 0)
            bytes_read = 0;

    if (bytes_read < size) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (AclCmd->data.downloadCSCTable.location > KN_TABLE_INDEX) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (!((AclCmd->data.downloadCSCTable.in_dim == 1) || 
        (AclCmd->data.downloadCSCTable.in_dim == 3))) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (!((AclCmd->data.downloadCSCTable.out_dim == 1) || 
        (AclCmd->data.downloadCSCTable.out_dim == 4))) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (!((AclCmd->data.downloadCSCTable.table_dim == 17) || 
        (AclCmd->data.downloadCSCTable.table_dim == 256))) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (AclCmd->data.downloadCSCTable.in_dim == AclCmd->data.downloadCSCTable.out_dim) //luts
    { 
        if (AclCmd->data.downloadCSCTable.out_dim * AclCmd->data.downloadCSCTable.table_dim != size) 
        {
            memFree(data_buffer);
            return(1);
        }
    }
    else //CSC tables
    {
        uint32_t array_size = AclCmd->data.downloadCSCTable.out_dim;
        uint32_t count;
        for (count = 0; count < AclCmd->data.downloadCSCTable.in_dim; count++) 
        {
            array_size *= AclCmd->data.downloadCSCTable.table_dim;
        }
        //array_size == (table_dim ^ in_dim) * out_dim
        if (array_size != size) 
        {
            memFree(data_buffer);
            return(1);
        }
    }
    return CSC_table_add_override_dl(AclCmd->data.downloadCSCTable.location,
                                     AclCmd->data.downloadCSCTable.in_dim,
                                     AclCmd->data.downloadCSCTable.out_dim,
                                     AclCmd->data.downloadCSCTable.table_dim,
                                     data_buffer); //false if ok
#endif
    return 1;
}

SINT32 color_table_delete_CSC_tables(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
    CSC_table_delete_dl();
    return(0);
}

SINT32 color_table_dl_HT_table(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
#if 0
    uint8_t * data_buffer;
    SINT32 bytes_read;
    uint32_t size;

    if (!(size = AclCmd->data.downloadHTTable.data_length)) 
    {
        return(1);
    }
    
    if((data_buffer = (uint8_t *) MEM_MALLOC_LIMITED(color_table_mlimiter, size )) == NULL)
    {
        /* do I need to try and dump data?*/
        return(1);
    }
    
    if ((bytes_read = Pipe->ReadConnect(Pipe, data_buffer, size, PARSEREAD_EXACT )) <= 0)
            bytes_read = 0;

    if (bytes_read < size) 
    {
        memFree(data_buffer);
        return(1);
    }
    
    if (AclCmd->data.downloadHTTable.color_plane > 3) 
    {
        memFree(data_buffer);
        return(1);
    }
    if (AclCmd->data.downloadHTTable.bits_per_pixel == 1) 
    {
        if (AclCmd->data.downloadHTTable.num_screens != 1) 
        {
            memFree(data_buffer);
            return(1);
        }
    }
    else if (AclCmd->data.downloadHTTable.bits_per_pixel == 2) 
    {
        if (AclCmd->data.downloadHTTable.num_screens != 3) 
        {
            memFree(data_buffer);
            return(1);
        }
    }
    else // ((AclCmd->data.downloadHTTable.bits_per_pixel > 2) ||
         //  (AclCmd->data.downloadHTTable.bits_per_pixel == 0 ))
    {
        memFree(data_buffer);
        return(1);
    }
    if (AclCmd->data.downloadHTTable.x_dim * 
        AclCmd->data.downloadHTTable.y_dim * 
        AclCmd->data.downloadHTTable.num_screens != size) 
    {
        memFree(data_buffer);
        return(1);
    }
    return HT_table_add_override_dl(AclCmd->data.downloadHTTable.bits_per_pixel,
                                     AclCmd->data.downloadHTTable.color_plane,
                                     AclCmd->data.downloadHTTable.x_dim,
                                     AclCmd->data.downloadHTTable.y_dim,
                                     AclCmd->data.downloadHTTable.num_screens,
                                     data_buffer); //false if ok
#endif
    return 1;
}
SINT32 color_table_delete_HT_tables(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
    HT_table_delete_dl();
    return(0);
}
void color_table_init_ACL(void) 
{
#if 0
    if (color_table_mlimiter == NULL) //first time only
    {
        color_table_mlimiter = get_memory_limiter_for_parser( e_Printer );
    }
    RegisterACLCommand(ACLCMD_DL_CSC_TABLE , color_table_dl_CSC_table , NULL);
    RegisterACLCommand(ACLCMD_DELETE_CSC_TABLES , color_table_delete_CSC_tables , NULL);
    RegisterACLCommand(ACLCMD_DL_HT_TABLE , color_table_dl_HT_table , NULL);
    RegisterACLCommand(ACLCMD_DELETE_HT_TABLES , color_table_delete_HT_tables , NULL);
#endif
}
