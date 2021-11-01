/**
 *
 * ============================================================================
 * Copyright (c) 2011-2013 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/
 
 /**
 * \file generator_nup.c 
 * \brief N-up page generator
 */

/** include files **/
#include <stdint.h>
#include <stdbool.h>
#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "SJMgr.h"
#include "print_job_types.h"
#include "generator_inits.h"
#include "generator_nup.h"
#include "print_mode_api.h"
#include "print_job_api.h"
#include "print_job_api_restricted.h"
#include "dma_buffer.h"

#define NUP_STRIP_HEIGHT 64
#define CHARSIZE 8 

/** local definitions **/
#define GEN_NUP_STACK_SIZE POSIX_MIN_STACK_SIZE
#define GEN_NUP_QUEUE_SIZE 10

#define _DBG_PRINTF  DBG_PRINTF_DEBUG 
//#define _DBG_PRINTF  DBG_PRINTF_CRIT 
#define DBG_PRFX "GENUP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(5)

#define K_IDX COLOR_BLACK
#define C_IDX COLOR_CYAN
#define M_IDX COLOR_MAGENTA
#define Y_IDX COLOR_YELLOW


void* gen_nup_task( void* );

static pthread_t gen_nup_thread;
static mqd_t gen_nup_queue;
static ALIGN8 uint8_t gen_nup_stack[GEN_NUP_STACK_SIZE];

void renderer_limited(nup_RENDER_INPUT *RendInput, mlimiter_t *limiter);

void nup_rotate(uint32_t, uint32_t, uint32_t,
    ATLISTENTRY*, ATLISTENTRY*, uint8_t, nup_ROTATE_DIR);

                              
typedef struct {
    uint32_t pos_x;
    uint32_t pos_y;
} IMAGEPOS;

typedef struct {
    uint32_t width;
    uint32_t height;
} ENGMEDIASIZES;

void print_page_data(PAGE_DATA *page)
{
    dbg_printf("--%s--\n", __FUNCTION__);
//-jrs    dbg_printf(" vidx:%d Vidy:%d Rasx:%d Rasy:%d\n",page->VIDEO_X, page->VIDEO_Y, page->RASTER_X, page->RASTER_Y);
//-jrs    dbg_printf(" resx:%d resy:%d\n",page->RESOLUTION_X, page->RESOLUTION_Y);
//-jrs    dbg_printf("inputinfo:\n");
//-jrs    dbg_printf(" bpp:%d WinB:%d HinL:%d WinP:%d res:%d,%d\n",page->input_info.image_bpp, page->input_info.image_data_width_in_bytes, page->input_info.image_height_in_lines, page->input_info.image_width_in_pixels, page->input_info.resolution.x, page->input_info.resolution.y);
    dbg_printf("image info:\n");
    dbg_printf(" bpp:%d WinB:%d HinL:%d WinP:%d res:%d,%d\n",page->image_output_info.image_bpp, page->image_output_info.image_data_width_in_bytes, page->image_output_info.image_height_in_lines, page->image_output_info.image_width_in_pixels, page->image_output_info.image_dpi.x, page->image_output_info.image_dpi.y);
}

#define WHITE_VALUE 0x00

void allocate_nup_memory(mlimiter_t* limiter, ATLISTENTRY *nup_memory, ATLISTENTRY *image_memory, 
         uint32_t height, uint32_t bytes_per_row, page_color_mode_t color_mode)
{
    uint32_t i, c;
    uint32_t strip_cnt=0;

    nup_data_strip_t *strip;
    int datalen;

    _DBG_PRINTF(  "%s bpr=%ld height=%ld\n", __FUNCTION__, bytes_per_row, height );

    datalen = NUP_STRIP_HEIGHT * bytes_per_row;

    //
    // Now allocate all the strip buffers we need.
    //
    for(c=0; c<NUM_MARKING_COLOR_CHANNELS; c++)
    {
        if(color_mode == e_Mono && c != K_IDX)
        {
            continue;
        }

        ATInitList(&image_memory[c]);
        ATInitList(&nup_memory[c]);

        //dbg_printf( "---Allocate: data for color %d\n", c);
        strip_cnt = 0;

        for( i=0 ; i<height ; i+=NUP_STRIP_HEIGHT ) 
        {
            strip = (nup_data_strip_t*) MEM_MALLOC_LIMITED(limiter, sizeof(nup_data_strip_t));
            ASSERT( strip );

/*
            if(i < height)
            {
                strip->num_lines = NUP_STRIP_HEIGHT;
            }
            else
            {
                strip->num_lines = i - height;
            }
*/
            if( (i+NUP_STRIP_HEIGHT) <= height)
            {
                strip->num_lines = NUP_STRIP_HEIGHT;
            }
            else
            {
                strip->num_lines = height - i;
                datalen = strip->num_lines * bytes_per_row;
            }
            strip->data_len = datalen;

            strip->databuf = BigBuffer_Malloc(limiter, datalen);
            ASSERT(strip->databuf);

            memset(strip->databuf->data, WHITE_VALUE, datalen); 
            //dbg_printf( "Allocate: data for color %d %p len:%d\n", c, dnode->Data, datalen);
            ATInsertTailList(&nup_memory[c], &strip->node);
            strip_cnt++;
        }

        _DBG_PRINTF("Allocated %d out-strips for color %d datalen:%d\n", 
                    strip_cnt, c, datalen);

    }
}

void free_linked_memory(ATLISTENTRY *nup_strip_lists, 
                          page_color_mode_t color_mode)
{
    ATLISTENTRY* node;
    nup_data_strip_t* strip;

    uint32_t c;

    _DBG_PRINTF("%s\n", __FUNCTION__);

    if(nup_strip_lists == NULL)
    {
        return;
    }

    for(c=0; c<NUM_MARKING_COLOR_CHANNELS; c++)
    {
        if(color_mode == e_Mono && c != K_IDX)
        {
            continue;
        }

        node = ATRemoveHeadList(&nup_strip_lists[c]);
        while(node != NULL)
        {
            //dbg_printf("%d. FREE color node %p %p\n", c, LinkMem, LinkMem->Data);
            strip = CONTAINING_RECORD(node, nup_data_strip_t, node);
            ASSERT(strip != NULL);
            if (strip->databuf)
            {
                BigBuffer_Free(strip->databuf);
                strip->databuf = NULL;
            }
            MEM_FREE_AND_NULL(strip);

            node = ATRemoveHeadList(&nup_strip_lists[c]); 
        }
    }
}

void calc_nup_image_position(uint32_t page_width, uint32_t page_height, uint32_t image_width, uint32_t image_height, int32_t image_engshift,
                     int image_scale_right_padding, int image_top_padding, uint32_t nup_page_number, nup_image_params_t *nup_image_params,
                     uint32_t *obj_x, uint32_t *obj_y, nup_ROTATE_DIR *rotate, uint32_t rendered_count, DUPLEX_MODE duplex_job)
{
    uint32_t nup_pages_wanted = nup_image_params->num_images;
    uint32_t ID_nup = nup_image_params->ID_nup;
    bool adf_scan = nup_image_params->adf_scan;
    bool fb_scan = !(nup_image_params->adf_scan);
    bool portrait = nup_image_params->page_orientation == PAGE_ORIENT_PORTRAIT;
    bool landscape = nup_image_params->page_orientation == PAGE_ORIENT_LANDSCAPE;
    int nobj_x, nobj_y;

    //dbg_printf( "%s nup_page_number=%d nup_pages_wanted=%d pagw:%d pagh:%d\n", __FUNCTION__, nup_page_number, nup_pages_wanted, page_width, page_height);

    if((ID_nup) && (ID_nup != IDNUP_SDH_ACTIVE) && (ID_nup != IDNUP_A3MERGE_ROTATE))// &&(ID_nup == IDNUP_A3MERGE_NOROTATE))
    {
        // all id nup images cover entire page and are OR'ed with each other
        nobj_x = 0;
        nobj_y = 0;
        *rotate = nup_NoRotate;
    }
    else
    {
        switch(nup_pages_wanted) 
        {
            case 1:
				if (ID_nup == IDNUP_A3MERGE_NOROTATE)
				{
					nobj_x= 24;//(page_width - (int)image_width)/2;
					nobj_y = ((int)page_height - (int)image_height)/2;
			        *rotate = nup_NoRotate;
				}
				else
				{
					nobj_x = (((int)page_width-(int)image_height) + image_top_padding) / 2;
					nobj_y = (((int)page_height - (int)image_width ) + image_scale_right_padding) / 2;
					nobj_x += (image_height%32)/2;
			        *rotate = nup_Rotate90;
				}
                break;
            case 2:
               *rotate = nup_Rotate90;
                DBG_PRINTF_ERR("adf_scan:%d, portrait:%d, fb_scan:%d, landscape:%d\n", adf_scan, portrait, fb_scan, landscape);
				if(ID_nup == IDNUP_SDH_ACTIVE)
                {
					*rotate = nup_NoRotate; //DAWTEST
				}
                else if((adf_scan && portrait) || (fb_scan && landscape))		// Conditions are all false.
                {
                    *rotate = nup_Rotate270;
                    image_scale_right_padding *= -1;
                    image_top_padding *= -1;
                }

				if(ID_nup == IDNUP_SDH_ACTIVE)
                {
//	                nobj_x = 0;
//	                nobj_y = 100;
					nobj_x = ((int)page_width - (int)image_width + image_top_padding) / 2;
					nobj_y = ((int)(page_height/2)-(int)image_height + image_scale_right_padding)/2;
				}
				else		// only nup_Rotate90
				{
	                // Put the image kind of in the middle.
	                nobj_x = ((int)page_width - (int)image_height + image_top_padding) / 2;
	                nobj_y = ((int)(page_height/2)-(int)image_width + image_scale_right_padding)/2;
	                nobj_x += (image_height%32)/2;
	            }
	            
                if(nup_page_number == 2)
                {
                    nobj_y += ((int)page_height/2);
                }
                break;

            case 4:
                // For 4 up we never need to rotate.
                *rotate = nup_NoRotate;
                if(adf_scan)
                {
                    *rotate = nup_Rotate180;
                    image_scale_right_padding *= -1;
//                    image_top_padding *= -1;
                }
                //*rotate = nup_Rotate180;  //DAWTEST
                //*rotate = nup_NoRotate;  //DAWTEST
 
                // For 4 up the location of the pages is different
                // depending on if we are printing landscape or
                // portrait.
                if(portrait)
                {
                    switch(nup_page_number) 
                    {
                        case 1:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2;
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2;
                            break;
                        case 2:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2 + ((int)page_width/2);
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2;
                            break;
                        case 3:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2;
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2 +((int)page_height/2);
                            break;
                        case 4:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2 + ((int)page_width/2);
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2 +((int)page_height/2);
                            break;
                        default:
                            XASSERT(0,nup_page_number);
                    }
                } 
                else 
                {
                    // assumption: image origin is top right corner
                    switch(nup_page_number) 
                    {
                        case 1:
                            nobj_x =(((int)page_width/2) - (int)image_width + image_scale_right_padding)/2 + ((int)page_width/2);
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2;
                            break;
                        case 2:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2 + ((int)page_width/2);
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2 +((int)page_height/2);
                            break;
                        case 3:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2;
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2;
                            break;
                        case 4:
                            nobj_x = (((int)page_width/2) - (int)image_width + image_scale_right_padding)/2;
                            nobj_y = (((int)page_height/2)-(int)image_height - image_top_padding)/2 +((int)page_height/2);
                            break;
                        default:
                            XASSERT(0,nup_page_number);
                    }
                }
                break;

            default:
            //dbg_printf("case ??\n");
                nobj_x = 0;
                nobj_y = 0;
                break;
        }
    }
	if (nobj_x > 0)
	    *obj_x = (uint32_t)nobj_x;
	else
		*obj_x = 0;
		
	if (nobj_y > 0)
	    *obj_y = (uint32_t)nobj_y;
	else 
		*obj_y = 0;
    DBG_PRINTF_ERR("%s x=%d(%d) y=%d(%d), pw=%d ph=%d, iw=%d ih=%d, srp=%d, tp=%d\n", __FUNCTION__, *obj_x, nobj_x, *obj_y, nobj_y, page_width, page_height,
                  image_width, image_height, image_scale_right_padding, image_top_padding);
    //dbg_printf( "%s x=%d y=%d rot:%d\n", __FUNCTION__, *obj_x, *obj_y, *rotate );
}



void render_nup_page(ATLISTENTRY *nup_memory, ATLISTENTRY *image_memory,
      IMAGEPOS *image_pos, uint32_t nup_page_number, nup_ROTATE_DIR image_rotation, 
      uint32_t image_width, uint32_t image_width_unpadded, uint32_t image_height,
      uint32_t page_width, uint32_t page_height, uint32_t bpp, 
      page_color_mode_t color_mode, uint32_t resolution_x, uint32_t resolution_y)
{
    nup_LINKGRAPHIC image;
    nup_DISP_LIST display;
    nup_RENDER_INPUT render;
    uint32_t c;
    nup_data_strip_t* strip;


    uint32_t colors;
    colors = 1;
    if(color_mode == e_Color) colors = NUM_MARKING_COLOR_CHANNELS; 

    _DBG_PRINTF( "%s pg(%dx%d) img(%dx%d) pagenum=%d,"
        "posx:%d posy:%d rot:%d medw:%d medh:%d\n", __FUNCTION__, 
        page_width, page_height, image_width, image_height,
        nup_page_number, image_pos->pos_x, image_pos->pos_y, 
        image_rotation, page_width, page_height );

    memset( &render, 0, sizeof(nup_RENDER_INPUT) );
    memset( &image, 0, sizeof(nup_LINKGRAPHIC) );
    memset( &display, 0, sizeof(nup_DISP_LIST) );

    // copy the list heads; now multiple heads point to same lists
    for(c=0; c<colors; c++)
    {
        image.InPlane[c] = &image_memory[c];
        render.Output[c] = &nup_memory[c];
        image.PlaneRotated[c] = false;
        // get the strip at the head of the list
        ASSERT(ATListHead(&image_memory[c]) != NULL);
        strip = CONTAINING_RECORD(ATListHead(&image_memory[c]), nup_data_strip_t, node);
        ASSERT(strip != NULL);
        image.InputLPS = strip->num_lines;
    }

    image.X_DIMENSION = image_width;
    image.X_DIMENSION_UNPADDED = image_width_unpadded;
    image.Y_DIMENSION = image_height;
    image.LENGTH_X = (image_width * bpp) / 8;
    
    display.OBJ_X_LEN = 0;
    display.OBJ_Y_LEN = 0;
    display.OBJ_X = image_pos->pos_x;
    display.OBJ_Y = image_pos->pos_y;
    display.NextObj = NULL;
    display.Object = &image;
    display.Operation = nup_or;
    //display.Type = e_LinkGraphics;

    render.BPP = bpp;
    //render.GenerateStripe = false;
    render.OutBufLPS = NUP_STRIP_HEIGHT;
    render.Page = &display;
    render.X_Resolution = resolution_x;
    render.Y_Resolution = resolution_y;
    render.Rotation = nup_Portrait;
    render.ImagesRotation = image_rotation;

    render.XSize = page_width * bpp;
    render.YSize = page_height;

    if(color_mode == e_Mono)
    {
        // mono
        render.data_type = NUP_REND_DATA_MONO;
        render.packing = NUP_PAGE_PLANAR_INTERLEAVED;
        render.color_order[K_IDX] = NUP_PAGE_BLACK;
    }
    else
    {
        // color
        render.data_type = NUP_REND_DATA_CMYK_4;
        render.packing = NUP_PAGE_PLANAR_INTERLEAVED;
        render.color_order[K_IDX] = NUP_PAGE_BLACK;
#if NUM_MARKING_COLOR_CHANNELS == 4
        render.color_order[C_IDX] = NUP_PAGE_CYAN;
        render.color_order[M_IDX] = NUP_PAGE_MAGENTA;
        render.color_order[Y_IDX] = NUP_PAGE_YELLOW;
#endif
    }

    renderer_limited(&render, 0);
    //Renderer_limiter(&render, mlimiter_by_name("print"));
    // the Renderer can change the data pointers so we need to 
    // update image_memory with the new pointers for subsequent
    // deallocation
    for(c=0; c<NUM_MARKING_COLOR_CHANNELS; c++)
    {
//-jrs        image_memory[c] = image.InPlane[c];
/*
        image_memory[c].LinkHead = image.InPlane[c].LinkHead;
        image_memory[c].LinkTail = image.InPlane[c].LinkTail;
*/ 
    }

    //dbg_printf("back from renderer\n");
}

static inline PLANE_TYPE map_color_to_plane_type(COLOR color)
{
    static PLANE_TYPE plane_type_map[] = {PLANAR_K, PLANAR_C, PLANAR_Y, PLANAR_M};
    // map declaration above relies on the order enforced by this assert
    ASSERT(COLOR_BLACK == 0 && COLOR_CYAN == 1 && COLOR_YELLOW == 2 && COLOR_MAGENTA == 3);

    return plane_type_map[color];
}

void send_nup_data_to_printer(CURRENT_RESOURCE *our_resource, 
                    ATLISTENTRY *nup_memory, PLANED *planed, 
                    uint32_t page_width, uint32_t bpp, 
                    page_color_mode_t color_mode)
{
    ATLISTENTRY* current_node;
    nup_data_strip_t* current_strip;
    bool last_strip;
    uint32_t buf_size=0;
    uint32_t num_rows;
    MESSAGE new_msg;
    PLANED *new_planed;
    uint32_t c;
    error_type_t ret=0;
    mlimiter_t* limiter = mlimiter_by_name("print");

    _DBG_PRINTF(  "%s\n", __FUNCTION__ );

    new_msg.msgType = MSG_JOBPLANESTART;
    new_msg.param1 = COLOR_BLACK;
    new_msg.param2 = 0;
    new_msg.param3 = 0;
    SYMsgSend(our_resource->Destination, &new_msg);
    if (color_mode == e_Color)
    {
        new_msg.param1 = COLOR_CYAN;
        SYMsgSend(our_resource->Destination, &new_msg);
        new_msg.param1 = COLOR_MAGENTA;
        SYMsgSend(our_resource->Destination, &new_msg);
        new_msg.param1 = COLOR_YELLOW;
        SYMsgSend(our_resource->Destination, &new_msg);
    }

    c = K_IDX;
    if(color_mode == e_Color)
    {
        c = C_IDX;
    }

    memset(&new_msg, 0, sizeof(MESSAGE));
    new_msg.msgType = MSG_JOBPLANEDATA;

    current_node = ATRemoveHeadList(&nup_memory[c]);

    do
    {
        ASSERT(current_node != NULL);
        current_strip = CONTAINING_RECORD(current_node, nup_data_strip_t, node);

        last_strip = false;
        if( ATIsListEmpty(&nup_memory[c]) ) 
        {
            last_strip = true;
        }

        num_rows = current_strip->num_lines;
        buf_size = num_rows * (page_width * bpp / 8);

        new_planed = print_construct_plane( limiter,
                                   c,
                                   current_strip->databuf,
                                   buf_size,
                                   map_color_to_plane_type(c),
                                   last_strip, // last_strip, is this only on one plane or on all?
                                   planed->image_info.image_dpi.x,
                                   planed->image_info.image_dpi.y,
                                   page_width,
                                   num_rows,
                                   bpp,
                                   planed->image_info.left_to_right,
                                   &planed->compression_info  
                                 ); 
        if((last_strip) && (new_planed))
        {
            _DBG_PRINTF("sending last strip plane %d, Rx:%d Ry:%d Rxb:%d\n", c,
                new_planed->image_info.image_width_in_pixels, new_planed->image_info.image_height_in_lines,
                new_planed->image_info.image_data_width_in_bytes);
        }

        new_msg.param1 = c;
        new_msg.param3 = new_planed;
        ret = SYMsgSend(our_resource->Destination, &new_msg);
        ASSERT( OK==ret );  

        if(color_mode == e_Color)
        {
            // sequence through colors
            if     (c == C_IDX) c = M_IDX;
            else if(c == M_IDX) c = Y_IDX;
            else if(c == Y_IDX) c = K_IDX;
            else                c = C_IDX;
        }

        MEM_FREE_AND_NULL(current_strip);

        current_node = ATRemoveHeadList(&nup_memory[c]);

        // we are sending the strips in CMYK order so when the K strips
        // are exhausted we are at the last strip
    } while((c != K_IDX) || !last_strip);

    new_msg.msgType = MSG_JOBPLANEEND;
    new_msg.param1 = COLOR_BLACK;
    new_msg.param2 = 0;
    new_msg.param3 = 0;
    SYMsgSend(our_resource->Destination, &new_msg);
    if (color_mode == e_Color)
    {
        new_msg.param1 = COLOR_CYAN;
        SYMsgSend(our_resource->Destination, &new_msg);
        new_msg.param1 = COLOR_MAGENTA;
        SYMsgSend(our_resource->Destination, &new_msg);
        new_msg.param1 = COLOR_YELLOW;
        SYMsgSend(our_resource->Destination, &new_msg);
    }

    MEM_FREE_AND_NULL(planed);
    planed = NULL;
    //dbg_printf("finished sending data to printer\n");
}

void propagate_msg(CURRENT_RESOURCE *our_resource, MESSAGE *msg, bool propogate)
{
    error_type_t ret;

    if(propogate)
    {
       // if(msg->msgType != MSG_JOBPLANEDATA) dbg_printf("=-=-=-send it on...msgid:%d\n", msg->msgType);
        ASSERT( NULL != our_resource );
        ret = SYMsgSend( our_resource->Destination, msg );
        ASSERT( OK==ret );  
    }
    else
    {
       // dbg_printf("block it...\n");
    }
}

/** 
 * \brief Release all resources associated with a page.
 * 
 * \param page - page to free
 *
 * \retval void
 * 
 **/
void gen_nup_free_page(PAGE_DATA* page)
{
	int err;
    extern sem_t *page_token_semaphore;
    err = sem_post(page_token_semaphore);
    ASSERT(err == 0);
    
    _DBG_PRINTF("%s: free page %#x\n", __func__, page);

    page->page_data_cookie = 0;
    MEM_FREE_AND_NULL(page->print_mode);
    MEM_FREE_AND_NULL(page);
}

/** 
 * \brief Release all resources associated with a sheet.
 * 
 * \param page - sheet to free
 *
 * \retval void
 * 
 **/
void gen_nup_free_sheet(SHEET_DATA* sheet)
{
    _DBG_PRINTF("%s: free sheet %#x\n", __func__, sheet);

    // current generator does not expect pages to be attached to the 
    // sheet at this point.  If we start attaching pages earlier assert
    // here to let us know we need to udpate the generator
    ASSERT(sheet->FrontPage == NULL);
    ASSERT(sheet->BackPage == NULL);

    MEM_FREE_AND_NULL(sheet);
}

/** 
 * \brief Send a rendered n-up page along to the job manager or other downstream 
 * parsers.  Handles details of sending auto-duplex settings, sheet creation, and 
 * messaging protocol. Passed in planed is freed by one of the sub-rouintes. 
 * Passed in page is no longer owned on return.
 * 
 * \retval void
 * 
 **/
void nup_send_rendered_page(mlimiter_t* limiter, PAGE_DATA* page, PLANED* planed, uint32_t page_width, uint32_t page_height, 
                            uint32_t image_height, CURRENT_RESOURCE* our_resource, ATLISTENTRY* nup_memory, 
                            nup_ROTATE_DIR rotation, uint32_t bpp, page_color_mode_t color_mode, uint32_t* current_page_number,
                            bool duplex_rotate_backside)
{
    uint32_t rotated_columns;
    MESSAGE msg;
    static SHEET_DATA *sheet = 0; // remember sheet from front to back.
    bool is_first_page_on_sheet;

    _DBG_PRINTF("%s: page = %#x\n", __func__, page);

    page->image_output_info.image_width_in_pixels = page_width;
    page->image_output_info.image_height_in_lines = page_height;
    page->image_output_info.image_data_width_in_bytes = page_width * bpp / 8;

    rotated_columns = image_height;

    if ( (rotation == nup_Rotate90 || rotation == nup_Rotate270) && (page->nup_image_params.num_images >= 2) )
    {
        // pi/2 3pi/2 rotations the image width becomes
        // the image height before rotation.  no need to
        // render the entire media page width
        // the length of the page is the same so don't
        // modify image height.
        page->image_output_info.image_width_in_pixels = rotated_columns;
        //-jrs                        page->image_output_info.image_data_width_in_bytes = rotated_columns * bpp / 8;
    }

    //print_page_data(page);

    // clear the existing print mode and then recalculate it
    memset(page->print_mode, 0, print_mode_get_sizeof());
    print_mode_get(page);

    // handle the autoduplex settings; this determines whether or not we need
    // to allocate a sheet
    *current_page_number = *current_page_number+1;
    is_first_page_on_sheet = print_page_handle_autoduplex_restricted(page, *current_page_number, duplex_rotate_backside);

    if(is_first_page_on_sheet)
    {
        sheet = (SHEET_DATA *) MEM_MALLOC_LIMITED(limiter, sizeof(SHEET_DATA));
        ASSERT(sheet);
        memset(sheet, 0, sizeof(SHEET_DATA));
        msg.msgType = MSG_JOBSHEETSTART;
        msg.param1 = 0;
        msg.param2 = 0;
        msg.param3 = sheet;
        SYMsgSend(our_resource->Destination, &msg);         // get a sheet started in the Job manager.
    }
    ASSERT(sheet);
    page->sheet = sheet;

    _DBG_PRINTF("%s: propogate page %#x\n", page);
    // Send the page start to the job manager
    msg.msgType = MSG_JOBPAGESTART;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = page;
    SYMsgSend(our_resource->Destination, &msg);
    page = NULL;  // page has been passed along so release reference


    send_nup_data_to_printer(our_resource, nup_memory,
                             planed, page_width, bpp, color_mode);
    // the memory referenced by the following pointer
    // is freed in the above function so detach pointers
    planed = NULL;

    // Send the page end to the job manager
    msg.msgType = MSG_JOBPAGEEND;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = NULL;
    SYMsgSend(our_resource->Destination, &msg);
}


/** public functions **/
 
/** 
 * \brief Initialize the nup generator
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t gen_nup_init()
{
   // Create a mail box and register it.
   posix_create_message_queue(&gen_nup_queue, "/gen_nup_queue", GEN_NUP_QUEUE_SIZE, sizeof(MESSAGE));

   // Register the queue with the router.
   router_register_queue(GEN_NUP_ID, gen_nup_queue);

   // Register the resource
   rm_register(e_Gen_NUp,  GEN_NUP_ID);

   // thread priority is LOW so scan has priority during Nup rendering
   posix_create_thread(&gen_nup_thread, gen_nup_task, 0, "gen_nup", gen_nup_stack, GEN_NUP_STACK_SIZE, POSIX_THR_PRI_LOW);

   return 0;
}  


/**
 *  \brief Main nup generator task.
 *  The task sits between the print_job_api interface and the print job manager. 
 *  It processes the print job manager messages and data and then passes the
 *  processed messages and data on to the print job manager.
 * 
 * \param temp Dummy.
 * 
 * \retval None
 *
 * 
 **/
void* gen_nup_task(void* temp)
{
    MESSAGE msg, new_msg, sheet_msg, page_msg;
    CURRENT_RESOURCE  *our_resource = NULL;
    error_type_t ret;
    int mq_ret;
    PAGE_DATA *page=NULL;
    PLANED *planed=NULL;
    uint32_t nup_page_count=0;
    uint32_t rendered_count=0;
    uint32_t nup_pages_wanted=0;
    uint32_t image_engshift=0;
    uint32_t image_scale_right_padding=0;
    uint32_t image_top_padding=0;
    uint32_t current_output_page_number=0;
    uint32_t bpp=2;
    uint32_t image_width=0, image_width_unpadded=0, image_height=0;
    uint32_t page_width=0, page_height=0;
    uint32_t resolution_x=0, resolution_y=0;
    bool nup_job=false;
    bool ID_nup_job=false;
    nup_ROTATE_DIR nup_image_rotate;
    bool sheet_start_pending=false;
    IMAGEPOS image_pos;
    page_color_mode_t color_mode=0;
    uint32_t color_idx=0;
    uint32_t row_count[NUM_MARKING_COLOR_CHANNELS] = {0};
    uint32_t last_strip[NUM_MARKING_COLOR_CHANNELS];
    ATLISTENTRY nup_memory[NUM_MARKING_COLOR_CHANNELS];
    ATLISTENTRY image_memory[NUM_MARKING_COLOR_CHANNELS];
    mlimiter_t* limiter = NULL;
    bool duplex_rotate_backside = false;
	DUPLEX_MODE	duplex_job = DMDUP_SIMPLEX;
//	static int dump_no=0;
	
    memset(last_strip, 0, sizeof(uint32_t)*NUM_MARKING_COLOR_CHANNELS);

    SysWaitForInit();

    DBG_PRINTF_NOTICE("%s\n", __func__);

    // main loop
    while (1)
    {
        // get next message
        mq_ret = posix_wait_for_sys_message(gen_nup_queue, &msg, POSIX_WAIT_FOREVER);
        XASSERT(mq_ret == 0, mq_ret);

        switch (msg.msgType)
        {
            case MSG_USER_INPUT:
            {
                _DBG_PRINTF("MSG_USER_INPUT\n");
                if(msg.param1 == IDNUP_FINISHED)
                {
                    nup_page_count = nup_pages_wanted;
                }
                break;
            }
            case MSG_RESOURCES:
            {
                _DBG_PRINTF("MSG_RESOURCES resource = %#x\n", msg.param3);
                our_resource = (CURRENT_RESOURCE *) msg.param3;
                _DBG_PRINTF("genNup received resources src=%d dst=%d\n",
                            our_resource->Source, our_resource->Destination);
                break;
            }
            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                _DBG_PRINTF("MSG_ACKRECIPE\n");
                break;
            }
            case MSG_JOBSTART:
            {
                _DBG_PRINTF("MSG_JOBSTART\n");
                ASSERT( NULL != our_resource );
                _DBG_PRINTF("gennup sending jobstart to dst=%d\n", 
                                      our_resource->Destination );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                sheet_start_pending=false;
                nup_page_count=0;
                rendered_count=0;
                current_output_page_number = 0;
                // remember important job settings
                ASSERT(IS_JOB_DATA((JOB_DATA*)msg.param3));
                duplex_rotate_backside = ((JOB_DATA*)msg.param3)->duplex_rotate_backside;

                // limiter needs to start/stop on job boundaries because
                // we may need to allocate limiter memory in job end when
                // sending the final page
                limiter = mlimiter_by_name("print");
                mlimiter_start(limiter);

                break;
            }
            case MSG_JOBSHEETSTART:
            {
                _DBG_PRINTF("MSG_JOBSHEETSTART\n");
                sheet_start_pending=true;
                sheet_msg = msg;
                break;
            }
            case MSG_JOBPAGESTART:
            {
                _DBG_PRINTF("MSG_JOBPAGESTART\n");
                if(page != NULL)
                {
                    gen_nup_free_page(page);
                    page = NULL;
                }
                page = (PAGE_DATA *)msg.param3;
                ASSERT(page != NULL);
                ASSERT(IS_PAGE_DATA(page));
                rendered_count++;
                _DBG_PRINTF("    new page = %#x\n", (uint32_t)page);

                DBG_PRINTF_ERR(" num_images %d, ID_nup %d, ro %d\n", 
                                 page->nup_image_params.num_images, 
                                 page->nup_image_params.ID_nup, page->fRotate_oneup_image); 
                // if num_images == 1 this is not an nup job
                // if ID_nup == INACTIVE this is not an ID_nup job
				duplex_job = page->DUPLEX;
                image_engshift = page->image_output_info.image_engshift;
                image_scale_right_padding = page->image_output_info.image_scale_right_padding;
                image_top_padding = page->image_output_info.image_top_padding;
// One up Rotation case
                if(page->nup_image_params.num_images == 1 && page->fRotate_oneup_image)
                {
                    nup_job = true;
                    nup_page_count++;
                    ID_nup_job = false;
                }
// A3 merge case
                else if (page->nup_image_params.ID_nup == IDNUP_A3MERGE_ROTATE) // || (page->nup_image_params.ID_nup == IDNUP_A3MERGE_NOROTATE))
                {
                    nup_job = true;
                    nup_page_count++;
                    ID_nup_job = false;
                }
// One up no rotation
                else if((page->nup_image_params.num_images < 2 && page->nup_image_params.ID_nup == IDNUP_INACTIVE) || (page->nup_image_params.ID_nup == IDNUP_A3MERGE_NOROTATE) )
                {
                    nup_job = false;
                    nup_page_count=0;
                    ID_nup_job = false;
                }
// 2/4 UP case
                else
                {
                    nup_job = true;
                    ID_nup_job = true;
                    nup_page_count++;
                    if(page->nup_image_params.ID_nup == IDNUP_INACTIVE)
                    {
                        ID_nup_job = false;
                    }
                }

                if(sheet_start_pending)
                {
                    sheet_start_pending = false;
                    if (nup_job)
                    {
                        // free the sheet.  we will recreate later as needed depending
                        // on duplex settings.
                        gen_nup_free_sheet((SHEET_DATA*)sheet_msg.param3);
                        sheet_msg.param3 = NULL;
                    }
                    else
                    {
                        // we now know this is not an nup job, so just forward the sheet.
                        propagate_msg(our_resource, &sheet_msg, !nup_job);
                    }
                }

                propagate_msg(our_resource, &msg, !nup_job);

                if(nup_job)
                {
                    uint32_t c;
                    for(c=0; c<NUM_MARKING_COLOR_CHANNELS; c++)
                    {
                        row_count[c] = 0;
                    }
                    _DBG_PRINTF("This is a nup job\n"); 
                    color_mode = page->page_output_color_mode;
                    nup_pages_wanted = page->nup_image_params.num_images;

                    if(page->nup_image_params.ID_nup == IDNUP_ACTIVE)
                    {
                        // set so high it wont realistically be reached
                        // we will reset when job has completed
                        nup_pages_wanted = 500;
                    }
                    else if(page->nup_image_params.ID_nup == IDNUP_FINISHED)
                    {
                        // set to current page count. forces data to
                        // be sent to printer
                        nup_pages_wanted = nup_page_count;
                    }

                    memset(last_strip, 0, sizeof(uint32_t)*NUM_MARKING_COLOR_CHANNELS);
                    if(color_mode == e_Mono)
                    {
                        memset(last_strip, 1, sizeof(uint32_t)*NUM_MARKING_COLOR_CHANNELS);
                        last_strip[K_IDX] = 0;
                    }

                    page_width  = page->nup_image_params.image_width;
                    page_height = page->nup_image_params.image_height;
                    bpp = page->image_output_info.image_bpp;
                    image_width = page->image_output_info.image_data_width_in_bytes * 8 / bpp;
                    image_width_unpadded = 
                           page->image_output_info.image_width_in_pixels;
                    image_height = page->image_output_info.image_height_in_lines;

                    resolution_x = page->image_output_info.image_dpi.x;
                    resolution_y = page->image_output_info.image_dpi.x;

                    // reinitialize page mode
                    page_msg = msg;
                    page_msg.param3  = page;

                    _DBG_PRINTF("nup num_images:%d\n", nup_pages_wanted);
                    _DBG_PRINTF("page -width:%d height:%d bpp:%d resx:%d\n",
                                 page_width, page_height, bpp, resolution_x);
                    _DBG_PRINTF("image -width:%d imgwidthNopad:%d imgheight:%d\n",
                        image_width, image_width_unpadded, image_height);
					
                    if(nup_page_count == 1)// && nup_pages_wanted > 1)
                    {
                        allocate_nup_memory(limiter, nup_memory, image_memory, page_height,
                                            (page_width * bpp)/8,
                                            page->page_output_color_mode);
                    }
#if 0
                    calc_nup_image_position( page_width, page_height, 
    	                      nup_page_count, &(page->nup_image_params),
        	                 &image_pos.pos_x, &image_pos.pos_y, 
            	             &nup_image_rotate);
#endif
                    if(nup_pages_wanted == 1)
                    	nup_image_rotate = page->fRotate_oneup_image;
                }
                else
                {
                    // if not doing n-up we are done with the page, since we passed it down the chain
                    page = NULL;
                }

                break;
            }
            case MSG_JOBPLANESTART:
            {
                _DBG_PRINTF("MSG_JOBPLANESTART\n");

                propagate_msg(our_resource, &msg, !nup_job);
                break;
            }
            case MSG_JOBPLANEDATA:
            {
                ASSERT(NULL != our_resource);

                // dont free big_buffer. freed by renderer
                if (planed != NULL)
                {
                    MEM_FREE_AND_NULL(planed);
                }

                planed = (PLANED *)msg.param3;
                ASSERT(IS_PLANED(planed));

                DBG_PRINTF_DEBUG("MSG_JOBPLANEDATA planed = %#x\n", 
                                               (uint32_t)planed);
               
                bool testing=false; // DAWTEST
                if(testing)
                {
                    // place some visible tics to mark page and strip boundaries
                    char *d = (char *)(planed->big_buffer->data);
                    memset(d, 0xff, 10);
                    memset(d + (planed->image_info.image_data_width_in_bytes -10), 0xff, 10);
                }

                propagate_msg(our_resource, &msg, !nup_job);

                if(nup_job)
                {
                    switch(planed->BlockDataType)
                    {
                        case PLANAR_K: color_idx=K_IDX; break;
#if NUM_MARKING_COLOR_CHANNELS > 1
                        case PLANAR_C: color_idx=C_IDX; break;
                        case PLANAR_M: color_idx=M_IDX; break;
                        case PLANAR_Y: color_idx=Y_IDX; break;
#endif
                        default:       color_idx=K_IDX; break;
                    }

                    planed->image_info.image_dpi.x = resolution_x;

                    nup_data_strip_t* strip = MEM_MALLOC_LIMITED(limiter, sizeof(nup_data_strip_t));
                    ASSERT(strip != NULL);
                    memset(strip, 0, sizeof(nup_data_strip_t));

                    strip->num_lines = planed->image_info.image_height_in_lines;
                    strip->databuf = planed->big_buffer;
                    planed->big_buffer = NULL;

                    row_count[color_idx] += strip->num_lines;

                    ATInsertTailList(&image_memory[color_idx], &strip->node);
                }
                else
                {
                    planed = NULL;
                }
                break;
            }
            case MSG_JOBPLANEEND:
            {
                _DBG_PRINTF("MSG_JOBPLANEEND\n");
                ASSERT( NULL != our_resource );

                propagate_msg(our_resource, &msg, !nup_job);
                break;
            }

            case MSG_JOBPAGEEND:
            {
                DBG_PRINTF_ERR("MSG_JOBPAGEEND\n");
                ASSERT( NULL != our_resource );
                ASSERT( OK==ret );  
                //dbg_printf("=-=-= %d %d %d\n", nup_job,nup_page_count,nup_pages_wanted);

                propagate_msg(our_resource, &msg, !nup_job);// || (nup_page_count == nup_pages_wanted));

                if (nup_job)
                {
                    // to keep from overflowing render buffers
                    if (row_count[0] < image_height)
                    {
                        _DBG_PRINTF("row_count[0](%d)<image_height(%d)\n",
                                    row_count[0], image_height);
                        image_height = row_count[0];
                    }

					{
						ENGMEDIASIZES paper_size;
						paper_get_media_size(page->PAPER, &paper_size.width, &paper_size.height);
						calc_nup_image_position( page_width, page_height, image_width, image_height,
							  image_engshift, (int)image_scale_right_padding, (int)image_top_padding,
    	                      nup_page_count, &(page->nup_image_params),
        	                 &image_pos.pos_x, &image_pos.pos_y, 
            	             &nup_image_rotate, rendered_count, duplex_job);
            	    }

#if 0		// source image dump
{
	uint32_t *MyInput;
	static FILE *image_ryu=NULL;
	ATLISTENTRY *CurrentNode;
	nup_data_strip_t *CurrentStripe;
	char path[200];
	
	sprintf(path, "/mnt/sda1/nup_strip_%d_%d_%d.raw",  ++dump_no, image_width, image_height);
	image_ryu = fopen(path,"wb");
	
	CurrentNode = ATListHead(&image_memory[K_IDX]);

	while(CurrentNode)
	{
		CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
		MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);	// data_len, num_lines
//		dbg_printf("[RYU] save %d, %d, %d\n", CurrentStripe->data_len, CurrentStripe->num_lines, planed->image_info.image_data_width_in_bytes*CurrentStripe->num_lines);
		fwrite(  MyInput , 1,  CurrentStripe->num_lines * image_width , image_ryu );
		dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
		CurrentNode = ATListNext(&image_memory[K_IDX], CurrentNode);
	}
	fflush(image_ryu);
	fclose(image_ryu);
}
#endif
                    // render the newly received page image into the existing n-up page
                    render_nup_page(nup_memory, image_memory,
                                    &image_pos, nup_page_count,
                                    nup_image_rotate, image_width,
                                    image_width_unpadded, image_height,
                                    page_width, page_height, bpp, color_mode,
                                    resolution_x, resolution_y);

                    //nup_page_count = nup_pages_wanted;//DAWTEST
                    free_linked_memory(image_memory, color_mode);
                    // dbg_printf("===DONE render nup page\n");

                    if (nup_page_count == nup_pages_wanted)
                    {
#if 0		// rendered image dump
{
	uint32_t *MyInput;
	static FILE *image_ryu=NULL;
	ATLISTENTRY *CurrentNode;
	nup_data_strip_t *CurrentStripe;
	uint32_t OutPutWidthBytes;
	char path[200];
	
	sprintf(path, "/mnt/sda1/nup_strip_%d_%d_%d.raw",  ++dump_no, page_width, page_height);
	
	OutPutWidthBytes =  (page_width * bpp) / 8;
	image_ryu = fopen(path,"wb");
	
	CurrentNode = ATListHead(&image_memory[K_IDX]);

	while(CurrentNode)
	{
		CurrentStripe = CONTAINING_RECORD(CurrentNode, nup_data_strip_t, node);
		MyInput = (uint32_t *)dma_buffer_mmap_forcpu(CurrentStripe->databuf);	// data_len, num_lines
//		dbg_printf("[RYU] save %d, %d, %d\n", CurrentStripe->data_len, CurrentStripe->num_lines, OutPutWidthBytes*CurrentStripe->num_lines);
		fwrite(  MyInput , 1,  CurrentStripe->num_lines * OutPutWidthBytes , image_ryu );
		dma_buffer_unmmap_forcpu(CurrentStripe->databuf);
		CurrentNode = ATListNext(&image_memory[K_IDX], CurrentNode);
	}
	fflush(image_ryu);
	fclose(image_ryu);
}
#endif
                        ASSERT(page != NULL && planed != NULL);
                        nup_send_rendered_page(limiter, page, planed, page_width, page_height, image_height, our_resource, nup_memory,
                                               nup_image_rotate, bpp, color_mode, &current_output_page_number, duplex_rotate_backside);
                        page = NULL;  // page has been passed along so release reference

                        // the memory referenced by the following pointer
                        // is freed in the above function so detach pointers
                        planed = NULL;

                        nup_page_count = 0;
                    }
                    // we do not NULL page in this case.  
                    // It will be free'd and NULL'd at the start of the next page
                }
                else
                {
                    // we should have released the page reference immediately after sending it along
                    ASSERT(page == NULL);
                }
                break;
            }
            case MSG_JOBEND:
            {
                _DBG_PRINTF("MSG_JOBEND\n");

                if (nup_job && nup_page_count != 0)
                {
                    ASSERT(page != NULL && planed != NULL);
                    nup_send_rendered_page(limiter, page, planed, page_width, page_height, image_height, our_resource, nup_memory,
                                           nup_image_rotate, bpp, color_mode, &current_output_page_number, duplex_rotate_backside);
                    page = NULL;  // page has been passed along so release reference

                    // the memory referenced by the following pointer
                    // is freed in the above function so detach pointers
                    planed = NULL;

                    nup_page_count = 0;
                }

                if (limiter != NULL)
                {
                    mlimiter_stop(limiter);
                    limiter = NULL;
                }
                                            
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );                         
                
                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;

                our_resource = NULL;
                page = NULL;
                planed = NULL;

                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }
            case MSG_CANCELJOB: 
            {
                _DBG_PRINTF("MSG_CANCELJOB\n");

                if(nup_job)
                {
                    free_linked_memory(image_memory, color_mode);
                    free_linked_memory(nup_memory, color_mode);
                }
                if (page != NULL)
                {
                    gen_nup_free_page(page);
                    page = NULL;
                }
                if (planed != NULL)
                {
                    MEM_FREE_AND_NULL(planed);
                    planed = NULL;
                }

                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GEN_NUP_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

                if (limiter)
                {
                    mlimiter_stop(limiter);
                    limiter = NULL;
                }
				else {
					DBG_PRINTF_ERR("limiter already NULL\n");	
				}
                
				if(our_resource) {
                    ASSERT( NULL != our_resource );
	                // free resources
	                new_msg.msgType = MSG_FREERECIPE;
	                new_msg.param1 = 0;
	                new_msg.param2 = 0;
	                new_msg.param3 = our_resource;

	                our_resource = NULL;

	                ret = SYMsgSend( SJMID, &new_msg );
	                ASSERT( OK==ret );
				}
				else {
					DBG_PRINTF_ERR("our_resource already NULL\n");	
				}
                
                break;
            }
            case MSG_JOBABORT:
            {
                DBG_PRINTF_ERR("Unexpected message JOB_ABORT\n");
                ASSERT(0); // we should never see this message!
                break;
            }
            default:
            {
                DBG_PRINTF_ERR("Unknown message %d\n", msg.msgType);
                ASSERT(0);  // we need to identify all of these messages
                break;
            }
        }
    }
    return 0;
}

//==========================================================================
//=============================== Renderer =================================
//==========================================================================
static mlimiter_t *g_mlimiter;


/**
 * \brief return the number of pixels between different pixels.  
 * This is used to get how far to space to get to the next location
 * for the very next pixel.  This allows packing of the data.
 * \param[in] pack Enum specifying the packing of the data.
 * \returns uint8_t
 * \retval 1, 3, or 4 depending on packing.
 * asserts on failure.
 */
uint8_t _get_pixel_stride(nup_page_output_data_packing_t pack)
{
    uint8_t value = 1;
    switch(pack)
    {
        case NUP_PAGE_PLANAR_INTERLEAVED:
        case NUP_PAGE_PLANAR_ORDERED:
            value = 1;
            break;
        case NUP_PAGE_RGB_PACK:
            value = 3;
            break;
        case NUP_PAGE_RGBX_PACK:
        case NUP_PAGE_XRGB_PACK:
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
uint8_t _get_pixel_offset(nup_page_output_data_packing_t pack)
{
    uint8_t value = 0;
    switch(pack)
    {
        case NUP_PAGE_PLANAR_ORDERED:
        case NUP_PAGE_PLANAR_INTERLEAVED:
        case NUP_PAGE_RGB_PACK:
        case NUP_PAGE_RGBX_PACK:
            value = 0;
            break;
        case NUP_PAGE_XRGB_PACK:
            value = 1;
            break;
        default:
            ASSERT(0);
    }
    return value;
}
uint8_t _get_pixel_byte_offset(nup_page_output_data_packing_t pack, uint32_t color)
{
    uint8_t value = 0;
    uint8_t pixel_offset = _get_pixel_offset(pack);
    switch(pack)
    {
        case NUP_PAGE_PLANAR_ORDERED:
        case NUP_PAGE_PLANAR_INTERLEAVED:
            value = pixel_offset + 0;
            break;
        case NUP_PAGE_RGB_PACK:
        case NUP_PAGE_RGBX_PACK:
        case NUP_PAGE_XRGB_PACK:
            value = pixel_offset + color;
            break;
        default:
            ASSERT(0);
    }
    return value;
}

/**
 * Function name: copy_link_obj
 *
 * \brief Copy a linked object into strip.
 * This takes a graphic object object and puts the stuff into strip.
 *
 * Description: 
 *
 * \param output_strip The destination strip for the data.
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
void copy_link_obj(BigBuffer_t *output_strip, nup_LINKGRAPHIC *Object, UINT32 Plane, 
        nup_DISP_LIST *DispObj, UINT32 Start, UINT32 Length, UINT32 ObjOffset, 
        UINT32 LineLength, UINT32 XOffset, UINT32 BPP, uint8_t pixel_stride, 
        uint8_t pixel_offset, nup_rend_byte_type_t data_type)
{
    UINT32 Line, XStartByte, Shift, ObjOff, XLen, CurrentXPtr, XObjLen;
    SINT32 XLength;
    UINT32 CurXByte;
    UINT32 CurY;
    unsigned char Residue, *ObjPtr, LastBitMask;
    nup_OBJ_OPERATION Operation;
    UINT32 ResidueBits;
    ATLISTENTRY *CurrentInNode;
    nup_data_strip_t* current_in_strip;
    UINT32 LineStrip, StripeCount;
    uint8_t* output_data = dma_buffer_mmap_forcpu(output_strip);

    //if(Plane == 0)
      //dbg_printf("copy_link_obj Start:%d, Length:%d, ObjOffset:%d, LineLength:%d, XOffset:%d, pixel_offset:%d\n", Start, Length, ObjOffset,LineLength, XOffset, pixel_offset);
      
    // Get the address of the plane to get the data from.
    CurrentInNode = ATListHead(Object->InPlane[Plane]);
    ASSERT(CurrentInNode);
    current_in_strip = CONTAINING_RECORD(CurrentInNode, nup_data_strip_t, node);
	
    // Space down to the correct strip
    LineStrip = current_in_strip->num_lines;
    while(LineStrip < ObjOffset)
    {
        CurrentInNode = ATListNext(Object->InPlane[Plane], CurrentInNode);
        ASSERT(CurrentInNode != NULL);
        current_in_strip = CONTAINING_RECORD(CurrentInNode, nup_data_strip_t, node);
        LineStrip += current_in_strip->num_lines;
    }

    // We are now pointing to the stripe we need to start placing in this strip
    // Make sure the offset is relative to the start of this input stripe.
    ObjOffset -= LineStrip - current_in_strip->num_lines;
    LineStrip = 0;                  // number of lines that are output
    StripeCount = ObjOffset;        // the count within the current stripe.
    ObjPtr = (unsigned char *) dma_buffer_mmap_forcpu(current_in_strip->databuf);

    // change the operation based on the data type.  
    // The objects coming are 0 for clear, 1 for full intensity.  
    // With rgb it's opposite so the operation must be changed to match.
    Operation = DispObj->Operation;

    for (Line = Start; Line < (Start + Length); Line++)
    {
        Residue = 0;      // the amount to carry into the next byte.
        CurrentXPtr = 0;
        if((StripeCount++) >= current_in_strip->num_lines)
        {
            // We have moved on to another stripe.  Update everything.
            LineStrip += current_in_strip->num_lines;
            CurrentInNode = ATListNext(Object->InPlane[Plane], CurrentInNode);
            if(CurrentInNode == NULL)
            {
                _DBG_PRINTF("Ran out of linked data line:%d start:%d len:%d\n",
                                          Line, Start, Length);
                break;
            }

            dma_buffer_unmmap_forcpu(current_in_strip->databuf);

            current_in_strip = CONTAINING_RECORD(CurrentInNode, nup_data_strip_t, node);
            ObjPtr = (unsigned char *) dma_buffer_mmap_forcpu(current_in_strip->databuf);
            StripeCount = 1;
        }

        CurY = Line - Start + ObjOffset-LineStrip;         // find the line that we are working on.
        // Find the length of the area to put in.
        XLength = DispObj->OBJ_X_LEN*BPP;
        if (XLength == 0) 
        {
            XLength = (SINT32) Object->X_DIMENSION * (SINT32) BPP;
        }
        if ((XLength+DispObj->OBJ_X*BPP) > (LineLength * BPP * CHARSIZE))
        {
            XLength = LineLength * CHARSIZE - DispObj->OBJ_X * BPP;
        }
        while (XLength > 0)
        {
            // Find out how much of the object to put down this time around.
            XObjLen = XLength;
            // Figure how much to shift the byte before doing the operation.
            Shift = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset) % CHARSIZE;         // number of extra bits.
            // Figure where we need to start in the strip.
            XStartByte = (DispObj->OBJ_X*BPP + CurrentXPtr + XOffset)*pixel_stride / CHARSIZE + Line * LineLength*pixel_stride;
            //
            XLen = (XObjLen + Shift) / CHARSIZE;
            LastBitMask = (XObjLen + Shift) % CHARSIZE;

            // Now put the bytes into the strip.
            ObjOff = CurY * Object->LENGTH_X;           // must init here in case XLen is 0.
            for (CurXByte = 0; CurXByte < XLen; CurXByte++)
            {
                // If I am in here I have at least 8 bits to put down.
                ObjOff = CurY * Object->LENGTH_X + CurXByte;       // get the address of our buffer location
                ResidueBits = 0;
                // Now put the bits into the strip.
                switch (Operation)
                {
                    case nup_or:
                        output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] |= Residue | (ObjPtr[ObjOff]>>Shift);
                        //DAWTEST
                        //if(ObjPtr[ObjOff] != 0)
                        //if(Strip[CurXByte*pixel_stride + XStartByte + pixel_offset]  != 0)
                        //{
                             //dbg_printf("-%d- CurXByt:%d l:%d ll:%d objoff:%d\n", 
                             //Plane,CurXByte, Line,LineLength, ObjOff);
                        //}
                        break;
                    case nup_and:
                        // If we do an and the not used byts must be 1 
                        // not zero.  Change them here.
                        if (CurXByte == 0) Residue = 0xff<<(CHARSIZE - Shift);
                        output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] &= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    case nup_xor:
                        output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] ^= Residue | (ObjPtr[ObjOff]>>Shift);
                        break;
                    default:
                        break;
                }
                // Get the stuff from the current byte that needs to be put 
                // in the next byte of the strip.
                Residue = ((ObjPtr[ObjOff] & (0xff >> ResidueBits)) & (0xff >> (CHARSIZE - Shift))) << (CHARSIZE - Shift);

            }

            // Write the last data into the buffer.
            //
            // if we go into the next word, get the bits.
            if (((int)LastBitMask - (int)Shift) > 0)
            {
                Residue |= ObjPtr[ObjOff+1]>>Shift;
            }

            if (LastBitMask)
            {
                switch (Operation)
                {
                    case nup_or:
             //           output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] |= (Residue ) & ~(0xff >> (LastBitMask));
                        break;
                    case nup_and:
                        output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] &= ((Residue ) & ~(0xff>> (LastBitMask))) | (0xff>>LastBitMask);
                        break;
                    case nup_xor:
                        output_data[CurXByte*pixel_stride + XStartByte + pixel_offset] ^= (Residue ) & ~(0xff>> (LastBitMask));
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
    dma_buffer_unmmap_forcpu(current_in_strip->databuf);
    dma_buffer_unmmap_forcpu(output_strip);
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
 * \returns void
 *
*/ 
void link_graphics_renderer(nup_RendererSession *pRSession, BigBuffer_t *strip, 
        uint32_t strip_len, nup_DISP_LIST *page_display_list,  nup_RENDER_INPUT *RendInput, 
        uint32_t Color,  uint32_t Rotate, uint32_t color_index)
{
    UINT32 XDim,YDim;
    nup_LINKGRAPHIC TempGraphic;
    int Len, StartY, Offset;
    uint32_t byte_width_unpadded = 0;
	
//    _DBG_PRINTF("%s...\n", __FUNCTION__);

    if((page_display_list == 0) ||
        (page_display_list->Object->LENGTH_X == 0))
    {
         return;
    }

    // Get the correct x and y dimensions.
    XDim = page_display_list->Object->X_DIMENSION;
    YDim = page_display_list->Object->Y_DIMENSION;

    // pi/2 3pi/2 rotations swap x & y coordinates
    if(RendInput->ImagesRotation == nup_Rotate90 ||
       RendInput->ImagesRotation == nup_Rotate270)
    {
        XDim = page_display_list->Object->Y_DIMENSION;
        YDim = page_display_list->Object->X_DIMENSION;
    }

    // First see if there is anything from this object on this color plane.
    if (ATIsListEmpty(page_display_list->Object->InPlane[Color]))
    {
        return;      // nothing on this plane, go on to the next object.
    }

    // See if this object is in this strip, check the beginning
    if (page_display_list->OBJ_Y >= (pRSession->CurY + strip_len))
    {
        //dbg_printf("----ObjY(%d) >= CurY(%d) + LPS(%d)\n",
               //page_display_list->OBJ_Y, pRSession->CurY, RendInput->OutBufLPS);
        return;
    }

    // See if the end of the object is before this strip
    if (page_display_list->OBJ_Y_LEN == 0)
    {
        if ((YDim + page_display_list->OBJ_Y) <= pRSession->CurY)
        {
            //dbg_printf("----YDim(%d) + ObjY(%d) <= CurY(%d)\n",
                       //YDim, page_display_list->OBJ_Y, pRSession->CurY);
            return;
        }
    }
    else
    {
        if ((((page_display_list->OBJ_Y_LEN + page_display_list->OBJ_Y) <= pRSession->CurY)))
        {
            return;
        }
    }

    // Some part of this object is in this strip put it there.
    pRSession->Dirty = 1;
    StartY = page_display_list->OBJ_Y;
    // Offset is how far into the graphic to index when starting the copy.
    Offset = 0;
    // Now see if the start is before the stripe, if so adjust.
    if(StartY < pRSession->CurY)
    {
        // start is before the stripe, adjust
        Offset = pRSession->CurY - StartY;
        StartY = pRSession->CurY;
    }
    // The length is determined by if we are stretching or 
    // just the object length
    if(page_display_list->OBJ_Y_LEN == 0)
    {
//        Len = page_display_list->Object->Y_DIMENSION - Offset;
        Len = YDim - Offset;
    } 
    else
    {
        Len = page_display_list->OBJ_Y_LEN - Offset;
    }
    // See if we overflowed the strip length, if so adjust
    if((StartY + Len) > (pRSession->CurY + strip_len))
    {
        Len = pRSession->CurY + strip_len - StartY;
    }

    // StartY is relative to the start of the page.  Want it relative to the
    // start of the strip.  Make that adjustment here.
    StartY -= pRSession->CurY;

    if (! (page_display_list->Object->PlaneRotated[Color]) && 
               RendInput->ImagesRotation == nup_Rotate180)
    {
        _DBG_PRINTF("180rotate color %d data:%p xdim:%d ydim:%d\n", Color,
             &page_display_list->Object->InPlane[Color],
             page_display_list->Object->X_DIMENSION,
             page_display_list->Object->Y_DIMENSION);

        byte_width_unpadded = 
            (page_display_list->Object->X_DIMENSION_UNPADDED * 
             RendInput->BPP) / 8;

        nup_rotate(page_display_list->Object->X_DIMENSION,
               page_display_list->Object->Y_DIMENSION, byte_width_unpadded,
               page_display_list->Object->InPlane[Color], 0,
               RendInput->BPP, RendInput->ImagesRotation);

        page_display_list->Object->PlaneRotated[Color] = true;
    } 
    // If the object needs to be rotated, do it here.
    // This will save the rotated image so that we only need to rotate it once.
    else if (! (page_display_list->Object->PlaneRotated[Color]) && 
                     ( RendInput->ImagesRotation == nup_Rotate90 ||
                       RendInput->ImagesRotation == nup_Rotate270 ))
    {
       //dbg_printf("=-=-=- Rotate image %d...\n", Color);

        ATLISTENTRY RotPlane;
        ATLISTENTRY* rot_node;
        nup_data_strip_t* rot_strip;

        UINT32 Rx,Ry;
        nup_LINKGRAPHIC *MyGraphic;

        MyGraphic = page_display_list->Object;
        // Rotate the image.
        MyGraphic->RotLPS = 100;      // do 100 lines per stripe.
        // Allocate room for the rotation.
        ATInitList(&RotPlane);

        Rx = (XDim * RendInput->BPP)/CHARSIZE;

        // If we slopped over, add one to size.
        if (((XDim * RendInput->BPP) % CHARSIZE) != 0) { Rx++; }
        if((Rx & 0x3) != 0) { Rx = (Rx & 0xfffffffc) + 0x4; }

        // Take 100 lines or less
        if(YDim < MyGraphic->RotLPS) { MyGraphic->RotLPS = YDim; }

        uint32_t rot_height;
        rot_height = (YDim/MyGraphic->RotLPS);
        if(YDim % MyGraphic->RotLPS != 0) rot_height++;
//        dbg_printf("Allocate: YDim:%d rot_height:%d RotLPS:%d\n", YDim,rot_height,MyGraphic->RotLPS);

        for (Ry = 0; Ry < rot_height; Ry++)
        {
            uint32_t datalen = Rx * MyGraphic->RotLPS;

            rot_strip = MEM_MALLOC_LIMITED(g_mlimiter, sizeof(nup_data_strip_t));
            ASSERT(rot_strip);
            rot_strip->num_lines = MyGraphic->RotLPS;
            rot_strip->data_len = datalen;
            rot_strip->databuf = BigBuffer_Malloc(g_mlimiter, datalen);
            ASSERT(rot_strip->databuf != NULL);
            memset(rot_strip->databuf->data, WHITE_VALUE, datalen);
            ATInsertTailList(&RotPlane, &rot_strip->node);
           //dbg_printf("Allocate: %d %p, Rx=%d LPS=%d color=%d\n", Ry,
                //TempLink, Rx, MyGraphic->RotLPS, Color);
        }
        MyGraphic->PlaneRotated[Color] = true;

        // We now have the destination location.
        // Rotate the thing.
        _DBG_PRINTF("rotate color %d data:%p xdim:%d ydim:%d\n", 
             Color, &RotPlane, MyGraphic->X_DIMENSION, MyGraphic->Y_DIMENSION);

        byte_width_unpadded = (MyGraphic->X_DIMENSION_UNPADDED *
                                RendInput->BPP) / 8;

        nup_rotate(MyGraphic->X_DIMENSION, MyGraphic->Y_DIMENSION, byte_width_unpadded,
                MyGraphic->InPlane[Color],
                &RotPlane, RendInput->BPP, 
                RendInput->ImagesRotation);

        // free the input plane, we will replace it with the rot plane
        ATLISTENTRY* node;
        while((node = ATRemoveHeadList(MyGraphic->InPlane[Color])) != NULL)
        {
            rot_strip = CONTAINING_RECORD(node, nup_data_strip_t, node);
            BigBuffer_Free(rot_strip->databuf);
            rot_strip->databuf = NULL;
            MEM_FREE_AND_NULL(rot_strip);
        }

        // put the rotated data back into the input plane
        while((rot_node = ATRemoveHeadList(&RotPlane)) != NULL)
        {        
            ATInsertTailList(MyGraphic->InPlane[Color], rot_node);
        }
        MyGraphic->LENGTH_X = Rx;
    }

    // Set up the graphic object with the info needed for the renderer
    TempGraphic = *((nup_LINKGRAPHIC *) page_display_list->Object);
    TempGraphic.X_DIMENSION = XDim;
    TempGraphic.Y_DIMENSION = YDim;

    // Set up the line length.
    if(!TempGraphic.PlaneRotated[Color])
    {
        TempGraphic.LENGTH_X = (XDim*RendInput->BPP) / 8;
        if(((XDim* RendInput->BPP) % 8) != 0)
             TempGraphic.LENGTH_X++;
    }

	if (Len<=0)
		return;

    // Everything is ready to go.  Put in the object.
    copy_link_obj(strip, &TempGraphic, Color, page_display_list, StartY, 
             Len, Offset, (pRSession->PIXELLINE*RendInput->BPP)/CHARSIZE,
             0, RendInput->BPP, _get_pixel_stride(RendInput->packing),
             _get_pixel_byte_offset(RendInput->packing, Color), 
             RendInput->data_type);

    if(0)//DAWTEST
    {
        int iii=0;
        for(iii=0; iii<(Len*(pRSession->PIXELLINE*RendInput->BPP))/CHARSIZE; iii++)
        {
//-jrs            if(Strip[iii] != 0)
//-jrs            {
                //dbg_printf("==%d== Strip:%p iii:%d\n", Color, Strip, iii);
//-jrs            }
        }
    }
    return;
}

/**
 * Function name: renderer_limited
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
void renderer_limited(nup_RENDER_INPUT *RendInput, mlimiter_t *limiter)
{
    nup_DISP_LIST   *MyList, *page_display_list;
    UINT32 iColor, Color;
    ATLISTENTRY* CurrentOutStrip[NUM_MARKING_COLOR_CHANNELS];
    UINT32 Rotate;
    uint32_t num_colors=0;
    nup_data_strip_t* strip = NULL;

    _DBG_PRINTF("%s\n", __FUNCTION__);

    g_mlimiter = limiter;

    nup_RendererSession RSession;
    RSession.PIXELLINE = RendInput->XSize/RendInput->BPP;
    Rotate = RendInput->Rotation;

    switch(RendInput->data_type)
    {
        case NUP_REND_DATA_MONO:
            num_colors = 1;
            break;
        case NUP_REND_DATA_RGB:
            num_colors = 3;
            break;
        case NUP_REND_DATA_CMYK_4:
            num_colors = 4;
            break;
        case NUP_REND_DATA_CMYK:
            /* We need 6 color planes for "CMYK" due to the planes in the engine */
            num_colors = 6;
            break;
        default:
            ASSERT(0);
            break;
    }

    //  Get the display list into MyList.  Sorted by y order might make things
    // faster.  Then after you are past the end of a item it can be removed.
    MyList= RendInput->Page;
    RSession.YLen = RendInput->YSize;    // get the size of the input.
    // Get the starting point of the output strips.
    for(Color = 0; Color < NUM_MARKING_COLOR_CHANNELS; Color++)
    {
        CurrentOutStrip[Color] = ATListHead(RendInput->Output[Color]);
    }

    RSession.Dirty = 1;          // tells me if we have something on this page.
    // Scan through the strips putting the objects into the strips.
    iColor = 0;
    do
    {
        RSession.CurY = 0;                   // the line we are starting on.
        while (RSession.CurY < RendInput->YSize) 
        {
            // For each color put in the stuff.
            do
            {
                switch(RendInput->color_order[iColor])
                {
#if NUM_MARKING_COLOR_CHANNELS > 1
                    case NUP_PAGE_CYAN:
                        Color = COLOR_CYAN;
                        break;
                    case NUP_PAGE_MAGENTA:
                        Color = COLOR_MAGENTA;
                        break;
                    case NUP_PAGE_YELLOW:
                        Color = COLOR_YELLOW;
                        break;
#endif
                    default:
                        ASSERT( NUP_PAGE_BLACK == RendInput->color_order[iColor] );
                        Color = COLOR_BLACK;
                        break;
                }

                // Get a pointer to the output buffer.
                if (! (RendInput->packing == NUP_PAGE_PLANAR_INTERLEAVED ||
                        RendInput->packing == NUP_PAGE_PLANAR_ORDERED))
                {
                    Color = 0;
                }

                if(ATListNext(RendInput->Output[Color], CurrentOutStrip[Color]) == NULL)
                {
                    continue; //DAWTEST
                }
                // if this is planar packing then we get current settings.
                XASSERT(CurrentOutStrip[Color] != NULL, Color);
                // get the pointer to the current strip.
                strip = CONTAINING_RECORD(CurrentOutStrip[Color], nup_data_strip_t, node);
                ASSERT(strip != NULL);

                // Initialize the pointers to the boundry of the strip.
                RSession.CurX = 0;

                page_display_list = MyList;  // initialize the display list pointer.
                // Now scan through each object seeing if any part of that 
                // object is on this strip.  If it is put that part in.
                while (page_display_list != NULL) // When to end this rendering
                {
                    link_graphics_renderer(&RSession, strip->databuf, strip->num_lines, page_display_list,
                           RendInput, Color, Rotate, iColor);
                    // Go to the next object in the display list.
                    page_display_list = page_display_list->NextObj;
                }
                // At this point everything that goes into this stripe 
                // is there. Decide what to do with the stripe.
                if (RendInput->packing == NUP_PAGE_PLANAR_INTERLEAVED ||
                        RendInput->packing == NUP_PAGE_PLANAR_ORDERED)
                {
                    // We are not sending the stripe to the engine so we 
                    // Go to the next stripe to fill.
                    CurrentOutStrip[Color] = ATListNext(RendInput->Output[Color], CurrentOutStrip[Color]);
                }
                if (((RendInput->packing != NUP_PAGE_PLANAR_INTERLEAVED) &&
                     (RendInput->packing != NUP_PAGE_PLANAR_ORDERED)) &&
                    (iColor == num_colors-1))
                {
                    // packed are always stored in 0
                    CurrentOutStrip[0] = ATListNext(RendInput->Output[0], CurrentOutStrip[0]);
                }

            } while (RendInput->packing != NUP_PAGE_PLANAR_ORDERED &&
                            ++iColor < num_colors);

            // Update the number of lines.
            RSession.CurY += RendInput->OutBufLPS;
            if(RendInput->packing != NUP_PAGE_PLANAR_ORDERED)
            {
                iColor = 0;
            }
        }
        // if we are not doing ordered, the colors start over here.
    } while (++iColor < num_colors && 
                   RendInput->packing == NUP_PAGE_PLANAR_ORDERED);

}
 

