#ifndef _SCANAPP_DATAPROCESS_H
#define _SCANAPP_DATAPROCESS_H

//#define SCAN_THRESHOLD_VALUE	127
#define SCAN_THRESHOLD_VALUE	170	// mod.lsh.for PC Scan tuning

void _dither(short* p,			/* Prev error */
                    uint8_t* c,	/* Current data */
                    short* n,		/* Next error */
                    int w, int dir, int type);
int _do_pixel(uint8_t* c, short* e, int x);
void _pack(uint8_t* out, uint8_t* in, int w);
void _pack_fast_photo(uint8_t* out, uint8_t* in, int w);

void scanData_gray2bw_strip_pre(int width, int minimumVal);
void scanData_gray2bw_strip_post(void);
int scanData_gray2bw(uint8_t* data, int width, int height, int num_row, int dither_type, int minimumVal);
int scanData_gray2bw_fast(uint8_t* data, int width, int height, int num_row, int dither_type, int minimumVal);
int scanData_gray2bw_strip(uint8_t* data, int width, int height, int dither_type);
int scanData_gray2bw_convert(void* hdl, uint8_t* data, int numrows, int dither_type);
int scanData_gray2bw_convert_fast(void* hdl, uint8_t* data, int numrows, int dither_type);
void* scanData_gray2bw_create(int width, uint32_t* mem, int mem_bytes);

void scanData_gray2bw_strip_back_pre(int width, int minimumVal);
void scanData_gray2bw_strip_back_post(void);
int scanData_gray2bw_strip_back(uint8_t* data, int width, int height, int dither_type);

typedef enum {
	SCAN_DITHERING_TEXT	= 0,
	SCAN_DITHERING_PHOTH	= 1,
	SCAN_DITHERING_LAST,
} SCAN_DITHERING_MODE;

typedef enum {
	SCAN_DATA_MINIMUM_VAL_IS_BLACK = 0,
	SCAN_DATA_MINIMUM_VAL_IS_WHITE = 1,	
} SCAN_DITHERRING_MIN_VAL;
#endif
