#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "regAddrs.h"
#include "agJobUtils.h"
#include "ResourceMap.h"
#include "utils.h"
#include "agprint.h"
#include "hwconfig_api.h"
#include "debug.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "data_access_api.h"
#include "error_types.h"
#include "BigBuffer.h"
#include "event_observer.h"
#include "scanapp_observers_api.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scands.h"
#include "scanhwerr.h"
#include "scanman.h"
#include "scanplat.h"
#include "scancap.h"
#include "scanmsg.h"
#include "print_job_api.h"
#include "PrintJobInfo.h"
#include "printvars_api.h"
#include "data_access_api.h"
#include "dma_buffer.h"
#include "assert.h"
#include "scanapp.h"
#include "scanapp_uiscan.h"
#include "scanapp_dataprocess.h"
#include "platform.h"
#include "scancore.h"
#include "scos.h"
#include "scandbg.h"
#include "scantools.h"
#include "piehw.h"
#include "scanhwerr.h"
#include "scanmech.h"
#include "scanmech_app.h"
#include "asp.h"
#include "pic.h"
#include "fraction.h"
#include "adfsensor.h"
#include "outfile.h"
#include "logger.h"
#include "scanapp_dataprocess.h"

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN UI: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
///Using Method.
// DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
// DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

typedef struct {
    int width;
    short* rows[2];
    int which;
} _cntxt_t;

static int image_min_val_type = SCAN_DATA_MINIMUM_VAL_IS_BLACK;
static int image_white_val = 255;
static int image_black_val = 0;
static void *handle;
static void *handle_back;
static uint8_t *working_mem;
static uint8_t *working_mem_back;
	
int _do_pixel(uint8_t* c, short* e, int x)
{
    int v = 0;
    
	v= e[x] + c[x];

	if (v > SCAN_THRESHOLD_VALUE)
	{
		c[x] = image_white_val;
		
		if (image_min_val_type == SCAN_DATA_MINIMUM_VAL_IS_BLACK)
			return v - image_white_val;
		else
			return v - image_black_val;
	}
	else
	{
		c[x] = image_black_val;
		
		if (image_min_val_type == SCAN_DATA_MINIMUM_VAL_IS_BLACK)
			return v - image_black_val;
		else
			return v - image_white_val;
	}

//	return v - c[x];
}

void _dither(short* p,				/* Prev error */
                    uint8_t* c,		/* Current data */
                    short* n,			/* Next error */
                    int w, int dir,
                    int ditherType)
{
    /* This is thresholding which does not work well for images */
	if(ditherType == SCAN_DITHERING_TEXT)
	{
		//DBG_PRINTF_SDH_DEBUG("_dither : scan_mode == B/W Text\n");
		int i;
		for (i=0;i<w;i++) {
			if (c[i] > SCAN_THRESHOLD_VALUE) 
			{
				c[i] = image_white_val;	///White
			}
			else 
			{
				c[i] = image_black_val;	///Black
			}
		}
	}
	else if(ditherType == SCAN_DITHERING_PHOTH)
	{
		//DBG_PRINTF_SDH_DEBUG("_dither : scan_mode == B/W Photo\n");
		/* What follows is serpentine floyd stienberg */
		int i, e;
		memset(n, 0, w*2);
		if (dir)
		{
			e = _do_pixel(c, p, i=0);
			p[i+1] +=    e /2;
			n[i+1] += (1*e)/8;
			n[i]   += (3*e)/8;
			for (i=1;i<(w-1);i++)
			{
				e = _do_pixel(c, p, i);
				p[i+1] += (7*e)/16;
				n[i+1] += (1*e)/16;
				n[i]   += (5*e)/16;
				n[i-1] += (3*e)/16;
			}
			e = _do_pixel(c, p, i=w-1);
			n[i]   += (5*e)/8;
			n[i-1] += (3*e)/8;
		}
		else
		{
			e = _do_pixel(c, p, i=w-1);
			p[i-1] +=    e /2;
			n[i-1] += (1*e)/8;
			n[i]   += (3*e)/8;
			for (i=w-2;i>0;i--)
			{
				e = _do_pixel(c, p, i);
				p[i-1] += (7*e)/16;
				n[i-1] += (1*e)/16;
				n[i]   += (5*e)/16;
				n[i+1] += (3*e)/16;
			}
			e = _do_pixel(c, p, i=0);
			n[i]   += (5*e)/8;
			n[i+1] += (3*e)/8;
		}
	}
}

void _pack(uint8_t* out, uint8_t* in, int w)
{
    uint8_t byte = 0;

    while (w)
    {
        if (*in++) byte |= 0x80; if (!--w) break;
        if (*in++) byte |= 0x40; if (!--w) break;
        if (*in++) byte |= 0x20; if (!--w) break;
        if (*in++) byte |= 0x10; if (!--w) break;
        if (*in++) byte |= 0x08; if (!--w) break;
        if (*in++) byte |= 0x04; if (!--w) break;
        if (*in++) byte |= 0x02; if (!--w) break;
        if (*in++) byte |= 0x01; if (!--w) break;
        *out++ = byte;
        byte = 0;
    }

    *out++ = byte;
}

void _pack_fast_photo(uint8_t* out, uint8_t* in, int w)
{
    uint8_t register byte = 0;

    while (w)
    {
        if (*in++) byte |= 0x80;
        if (*in++) byte |= 0x40;
        if (*in++) byte |= 0x20;
        if (*in++) byte |= 0x10;
        if (*in++) byte |= 0x08;
        if (*in++) byte |= 0x04;
        if (*in++) byte |= 0x02;
        if (*in++) byte |= 0x01;
        w -= 8;
        *out++ = byte;
        byte = 0;
    }
}


int scanData_gray2bw_convert(void* hdl, uint8_t* data, int numrows, int dither_type)
{
    _cntxt_t* c = (_cntxt_t*)hdl;
    uint8_t* in = data;
    uint8_t* out = data;

    while (numrows--)
	{
		_dither(c->rows[c->which], in, c->rows[c->which^1], c->width, c->which, dither_type);
		_pack(out, in, c->width);
		in += c->width;
		out += (c->width+7)/8;
		c->which ^= 1;
    }

    return 0;
}

int scanData_gray2bw_convert_fast(void* hdl, uint8_t* data, int numrows, int dither_type)
{
    _cntxt_t* c = (_cntxt_t*)hdl;
    uint8_t* in = data;
//    uint8_t* out = data;

	if ( dither_type == SCAN_DITHERING_PHOTH )
	{
    	uint8_t* out = data;
    	
		while (numrows--)
		{
			_dither(c->rows[c->which], in, c->rows[c->which^1], c->width, c->which, SCAN_DITHERING_PHOTH);
			_pack_fast_photo(out, in, c->width);
			in += c->width;
			out += (c->width+7)/8;
			c->which ^= 1;
	    }
	}
	else
	{
		uint32_t register word = 0;
		uint32_t* out = (void*)data;

		int w;

		if(image_min_val_type == SCAN_DATA_MINIMUM_VAL_IS_BLACK)
		{
			while (numrows--)
			{
				w = c->width;
				while (w)
				{
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x80;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x40;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x20;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x10;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x08;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x04;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x02;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x01;
					                                  
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x8000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x4000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x2000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x1000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x0800;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x0400;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x0200;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x0100;
					                                  
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x800000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x400000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x200000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x100000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x080000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x040000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x020000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x010000;
					                                  
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x80000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x40000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x20000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x10000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x08000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x04000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x02000000;
					if (*in++ > SCAN_THRESHOLD_VALUE ) word |= 0x01000000;
					
					w -= 32;
					*out++ = word;
					word = 0;
				}
		    }
		}
		else
		{
			while (numrows--)
			{
				w = c->width;
				while (w)
				{
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x80;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x40;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x20;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x10;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x08;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x04;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x02;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x01;
					             
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x8000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x4000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x2000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x1000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x0800;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x0400;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x0200;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x0100;
					             
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x800000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x400000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x200000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x100000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x080000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x040000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x020000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x010000;
					             
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x80000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x40000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x20000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x10000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x08000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x04000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x02000000;
					if (*in++ <= SCAN_THRESHOLD_VALUE ) word |= 0x01000000;
					
					w -= 32;
					*out++ = word;
					word = 0;
				}
		    }
		}
	}

    return 0;
}

uint32_t scanData_gray2bw_memreq(int width)
{
    return sizeof(_cntxt_t) + 4*width;
}

void* scanData_gray2bw_create(int width, uint32_t* mem, int mem_bytes)
{
    _cntxt_t* hdl;

    if (mem_bytes < scanData_gray2bw_memreq(width))
        return NULL;

    memset(mem, 0, scanData_gray2bw_memreq(width));
    hdl = (_cntxt_t*)mem;
    mem += sizeof(_cntxt_t)/4;
    hdl->rows[0] = (short*)mem;
    hdl->rows[1] = hdl->rows[0] + width;
    hdl->width = width;
    return hdl;
}

int scanData_gray2bw(uint8_t* data, int width, int height, int num_row, int dither_type, int minimumVal)
{
	uint32_t mem_req;
	
	/* check the minimum value of image */
	if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_BLACK) {
		image_white_val = 255;
		image_black_val = 0;
	}
	else if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_WHITE) {		
		image_white_val = 0;
		image_black_val = 255;
	}
	else {
		image_white_val = 255;
		image_black_val = 0;		
	}
	image_min_val_type = minimumVal;	

	if (working_mem)
		MEM_FREE_AND_NULL(working_mem);
	
    /* 8bit -> 1bit */
	mem_req = scanData_gray2bw_memreq(width);
	working_mem = MEM_MALLOC(mem_req);
	handle = scanData_gray2bw_create(width, (uint32_t*)working_mem, mem_req);
	if(handle == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  gray2bw_create fail\n", __FUNCTION__);
		assert(0);
	}
	scanData_gray2bw_convert(handle, data, height, dither_type);

	MEM_FREE_AND_NULL(working_mem);
	
	return 0;
}

int scanData_gray2bw_fast(uint8_t* data, int width, int height, int num_row, int dither_type, int minimumVal)
{
	uint32_t mem_req;
	
	/* check the minimum value of image */
	if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_WHITE) {
		image_white_val = 0;
		image_black_val = 255;
	}
	else {
		image_white_val = 255;
		image_black_val = 0;		
	}

	image_min_val_type = minimumVal;

	if (working_mem)
		MEM_FREE_AND_NULL(working_mem);
	
    /* 8bit -> 1bit */
	mem_req = scanData_gray2bw_memreq(width);
	working_mem = MEM_MALLOC(mem_req);
	handle = scanData_gray2bw_create(width, (uint32_t*)working_mem, mem_req);
	if(handle == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  gray2bw_create fail\n", __FUNCTION__);
		assert(0);
	}
	scanData_gray2bw_convert_fast(handle, data, height, dither_type);

	MEM_FREE_AND_NULL(working_mem);
	
	return 0;
}

int scanData_gray2bw_strip(uint8_t* data, int width, int height, int dither_type)
{
    /* 8bit -> 1bit */

	if (working_mem == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  scanData_gray2bw_strip fail\n", __FUNCTION__);
		assert(0);
	}
	scanData_gray2bw_convert(handle, data, height, dither_type);

	return 0;
}

void scanData_gray2bw_strip_pre(int width, int minimumVal)
{
	uint32_t mem_req;

	/* check the minimum value of image */
	if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_BLACK) {
		image_white_val = 255;
		image_black_val = 0;
	}
	else if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_WHITE) {		
		image_white_val = 0;
		image_black_val = 255;
	}
	else {
		image_white_val = 255;
		image_black_val = 0;		
	}
	image_min_val_type = minimumVal;	

	if (working_mem)
		MEM_FREE_AND_NULL(working_mem);
			
	mem_req = scanData_gray2bw_memreq(width);
	working_mem = MEM_MALLOC(mem_req);
	handle = scanData_gray2bw_create(width, (uint32_t*)working_mem, mem_req);
	if(handle == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  gray2bw_create fail\n", __FUNCTION__);
		assert(0);
	}
}

void scanData_gray2bw_strip_post(void)
{
	if (working_mem)
		MEM_FREE_AND_NULL(working_mem);
}

int scanData_gray2bw_strip_back(uint8_t* data, int width, int height, int dither_type)
{
    /* 8bit -> 1bit */

	if (working_mem_back == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  scanData_gray2bw_strip fail\n", __FUNCTION__);
		assert(0);
	}
	scanData_gray2bw_convert(handle_back, data, height, dither_type);

	return 0;
}

void scanData_gray2bw_strip_back_pre(int width, int minimumVal)
{
	uint32_t mem_req;

	/* check the minimum value of image */
	if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_BLACK) {
		image_white_val = 255;
		image_black_val = 0;
	}
	else if(minimumVal == SCAN_DATA_MINIMUM_VAL_IS_WHITE) {		
		image_white_val = 0;
		image_black_val = 255;
	}
	else {
		image_white_val = 255;
		image_black_val = 0;		
	}
	image_min_val_type = minimumVal;	

	if (working_mem_back)
		MEM_FREE_AND_NULL(working_mem_back);
			
	mem_req = scanData_gray2bw_memreq(width);
	working_mem_back = MEM_MALLOC(mem_req);
	handle_back = scanData_gray2bw_create(width, (uint32_t*)working_mem_back, mem_req);
	if(handle_back == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s()  gray2bw_create fail\n", __FUNCTION__);
		assert(0);
	}
}

void scanData_gray2bw_strip_back_post(void)
{
	if (working_mem_back)
		MEM_FREE_AND_NULL(working_mem_back);
}