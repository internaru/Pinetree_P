#ifndef __TIFF_MAIN__
#define __TIFF_MAIN__

#define uint32_t unsigned long
#define uint8_t unsigned char


#define AVERAGE(a, b)     (uint8_t)( ((a) + (b)) >> 1 )

#define PATH_SEPARATOR     '/'

#define WIDTH_A4     0
#define WIDTH_B4     1
#define WIDTH_A3     2


typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BOOLEAN;


# ifndef FALSE
    #define FALSE 0
# endif // # ifndef FALSE


# ifndef TRUE
    #define TRUE 1
# endif // # ifndef TRUE

/* */
#define A4_HEIGHT 3432
#define A4_PIXEL_PER_MM_300DPI (3432/297)

typedef enum {
    FAX_VR_STANDARD,
    FAX_VR_FINE,
    FAX_VR_SUPERFINE,
    FAX_VR_300,
    FAX_VR_ULTRAFINE,

    FAX_VR_LAST
} FAX_VR_t;


typedef struct {
    int width;
    short* rows[2];
    int which;
} _cntxt_t;


int MakePBMToFaxImage (int PageIndex);
void load_tiff(char* input_filename);
void ScaleLineAvg(uint8_t *Target, uint8_t *Source, int SrcWidth, int TgtWidth);
void ScaleRect(uint8_t *Target, uint8_t *Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);
int raw_to_fax_image(char* output_filename, UINT16 jnum);


extern void fsu_update_image_width_pixel (UINT16 jnum, UINT32 width);
extern void fsu_update_image_height_pixel (UINT16 jnum, UINT32 height);
extern void *checkedmalloc(size_t n);
extern int sw_jbig_encode_tx (int Page, char *FileName, UINT16 jnum, UINT8 Direction);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 faxapp_get_no_of_recipient(void);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern UINT32 fsu_get_image_width_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);

#endif // #ifndef __TIFF_MAIN__
