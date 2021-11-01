/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include "pip_page_adaptor.h"
#include "pip_strip_adaptor.h"
#include "dma_buffer.h"

struct pip_file_page_adaptor 
{
    pip_page_adaptor_BaseClassMembers;  // put this macro in your subclass 
    
    uint32_t pip_mode;  // instead of computing this from the rest of the system's output intent set here.
};

static void pip_file_attach_page( struct pip_page_adaptor *page, void *file_blob )
{
    struct pip_file_page_adaptor *ppa = (struct pip_file_page_adaptor * )page;

    //ppa->page = (PAGE_DATA*)from_page_blob; // ignore input page intent 

    // configure the input / output 
    ppa->scale_x = 1;
    ppa->scale_y = 1;
    ppa->pix_width = 5104;  // hack should pull this from input file.
    ppa->page_height = 600 * 11;
    ppa->pip_mode = 111;   // majic number will lookup and configure the pip pipe.
}
static void pip_file_send_page( struct pip_page_adaptor *page )
{
    
}
static void pip_file_close_page( struct pip_page_adaptor *page )
{
    
}
static uint32_t pip_file_pip_mode( struct pip_page_adaptor *page )
{
    struct pip_file_page_adaptor *ppa = (struct pip_file_page_adaptor *)page; 
    return ppa->pip_mode;
}

struct pip_page_adaptor *construct_pip_file_page_adaptor( struct pip_strip_adaptor *reuse_if_not_null )
{
    struct pip_file_page_adaptor *ppa = 0;

    ASSERT(!reuse_if_not_null);
  
    ppa = (struct pip_file_page_adaptor *) MEM_MALLOC( sizeof( struct pip_file_page_adaptor ) );
    if (!ppa) 
	return 0;
    ppa = (struct pip_file_page_adaptor*) construct_pip_page_adaptor( (struct pip_page_adaptor*)ppa );

    ppa->attach_from_page = pip_file_attach_page;
    ppa->pip_send_page = pip_file_send_page;
    ppa->page_close = pip_file_close_page;
    ppa->from_page_pip_mode = pip_file_pip_mode;

    return (struct pip_page_adaptor *) ppa;
}

struct pip_file_strip_adaptor
{
    pip_strip_adaptor_BaseClassMembers;  // put this macro in your subclass 

    char *filename;
    FILE *fp;
    
    BigBuffer_t *big_buffer;
};




static BigBuffer_t *pip_file_get_bb_in( struct pip_strip_adaptor *strip )
{
    if ( strip )
	return ((struct pip_file_strip_adaptor *)strip)->big_buffer;
    return 0;
}
static void pip_file_attach_from_strip( struct pip_strip_adaptor *base, void *from_blob )
{
    struct pip_file_strip_adaptor *psa = (struct pip_file_strip_adaptor *) base;
    
    char *file_name = (char*) from_blob;
    char str[256];
    const int str_max = 256;
    int width = 5104;  
    int height = 64;
    int size;
    int result;


    FILE *fp = fopen(file_name, "r");
    
    fgets(str, str_max, fp);

    puts(str); // dbg.
    // token scanf string to validate file content and to fetch width and height
    // expect P5 width height 255
    
    psa->width_pix_in = width;
    psa->lines_in = height;
    psa->scale_x = 1;
    psa->scale_y = 1;
    psa->output_bpp = 1;
    psa->out_strip_height = 100;
    psa->is_end_of_page = 0; 

    psa->width_pix_out = ((psa->width_pix_in * psa->scale_x) + 127) & ~127; 
    size = width * height;

    char *raw = MEM_MALLOC( size );

    result = fread(raw, 1, size, fp);
    if( result != size )
    {
	psa->lines_in = result/width;  // last strip is often short.
	psa->is_end_of_page = true;
    }

    psa->big_buffer = dma_buffer_adopt( raw, result );

    fclose(fp);
}
static void pip_file_free_in_strip( struct pip_strip_adaptor *base )
{
}

static char *dump_path = "/data/dump/"; 

void dbg_dump_BigBuffer_to_pgm_file( int pagecnt, 
				     BigBuffer_t *output_buffer, 
				     int planecount, 
				     int pixel_width,
				     int lines )
{
    char path[200];
    FILE *fp;
    
    if ( dump_path ) {

	sprintf(path, "%s_p_%02d_s_%03d.ppm", 
		dump_path, pagecnt, planecount);
		
	if ( !(fp = fopen(path, "w")) ) {
	    return;
	}

	dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
	dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
	void *data = dma_buffer_mmap_forcpu(output_buffer);
 	
	sprintf(path, "P5 %d %d 255 \n",  
		pixel_width,
		lines );
	fwrite(path, 1, strlen(path)+1, fp); 
	fwrite(data, 1, output_buffer->datalen, fp);
	fclose(fp);		
	dma_buffer_unmmap_forcpu( output_buffer );
    }
}

void dbg_dump_BigBuffer_to_pbm_file( int pagecnt, int color, int planecount, 
				     BigBuffer_t *output_buffer, 
				     int bpp,
				     int pixel_width,
				     int lines )
{
    char path[200];
    FILE *fp;

    if ( dump_path ) {
	sprintf(path, "%spage_p_%02d_c_%1d_s_%03d.pbm", dump_path, pagecnt, color, planecount);

	if ( !(fp = fopen(path, "w")) ) {
	    return;
	}
	dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
	dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
	void *data = dma_buffer_mmap_forcpu(output_buffer);
 	
	sprintf(path, "P4 %d %d \n",  
		pixel_width * bpp, /// 1bpp is waht pbm file always expects.
		lines );
	fwrite(path, strlen(path)+1, 1, fp); 

	fwrite(data, output_buffer->datalen, 1, fp);
	fclose(fp);
	dma_buffer_unmmap_forcpu( output_buffer );
    }
}

static void pip_file_mono_send_strip( struct pip_strip_adaptor *to_strip, BigBuffer_t *bb[6] )
{
    struct pip_file_strip_adaptor *psa = (struct pip_file_strip_adaptor *) to_strip;
    if (bb[4])
    {
	static int planenumber = 0; // plane numbers must increment within a page
	++planenumber;
	dbg_dump_BigBuffer_to_pbm_file(1, 0, planenumber, bb[4], psa->output_bpp, psa->width_pix_out, psa->out_strip_height );
	BigBuffer_Free(bb[4]);
    }
}
  


struct pip_strip_adaptor *construct_pip_file_strip_adaptor( struct pip_strip_adaptor *reuse_if_not_null )
{
    struct pip_file_strip_adaptor *psa = 0;

    psa = (struct pip_file_strip_adaptor *) MEM_MALLOC( sizeof( struct pip_file_strip_adaptor ) );
    if (!psa) 
	return 0;
    psa = (struct pip_file_strip_adaptor*) construct_pip_strip_adaptor( (struct pip_strip_adaptor*)psa );

    psa->get_bb_in = pip_file_get_bb_in;
    psa->attach_from_strip = pip_file_attach_from_strip;
    psa->free_in_strip = pip_file_free_in_strip;

    // use base class definition : psa->recv_strip  
    psa->pip_send_strip = pip_file_mono_send_strip;
    


    return (struct pip_strip_adaptor *) psa;
}  





static int is_valid_pgm_file(const struct dirent *dir)
{
    if ( strncmp( "page_", dir->d_name, 5 ) != 0 )
	return 0;
    // open file test for pgm todo: improve pretty weak test 
    // close file 
    return 1 ; 
}

int main_pip_grayfile_adaptor( int argc, char *argv[] )
{
    struct dirent **eps;
    int n;
    int cnt;
    struct pip_page_adaptor * page_adaptor = construct_pip_file_page_adaptor( 0 );
    struct pip_strip_adaptor *strip_adaptor = construct_pip_file_strip_adaptor( 0 );
    char filename[256];

    // one directory per page with "page_..." sorted strips of pgm files
     
    ASSERT(argc > 1);
    n = scandir(argv[1], &eps, is_valid_pgm_file, alphasort);
    if ( n < 0 ) {
	perror ("Couldn't open the directory");    
	return 0;
    }

    pip_recv_page( page_adaptor, "foo" );
    pip_new_page( strip_adaptor );

    for (cnt = 0; cnt < n; ++cnt) 
    {
	char *strip_file_name = eps[cnt]->d_name;
	strncpy(filename, argv[1], 256);
	strncat(filename, strip_file_name, 256);
	puts( strip_file_name );
	pip_recv_strip( page_adaptor, strip_adaptor, filename );	    
    }

    from_page_close( page_adaptor );    
    memFree( page_adaptor );
    memFree( strip_adaptor );

    return 0;
}
