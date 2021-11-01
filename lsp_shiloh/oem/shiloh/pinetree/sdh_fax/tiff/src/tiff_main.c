#include "fax_memAPI.h"
#include "SDHFaxCommonHeader.h"
#include "tiff.h"
#include "tiffio.h"
#include "tiffiop.h"
#include "tif_config.h"
#include <errno.h>
#include <fcntl.h>
#include "tiff_main.h"
//#include "fxcb.h"
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "jbig85.h"
#include <ctype.h>
#include <time.h>
#include "paper_size_config.h"
#include "error_types.h"
#include "lassert.h"
#include "faxapp_notify.h"
#include "spec.h"
#include "dc_fwcxdxx_lnx_api.h"
//#include "agprint.h"

#define CODING_MH               0
#define CODING_MR               1
#define CODING_MMR              2
#define CODING_JPEG             3
#define CODING_JPEG_COLOR       3
#define CODING_JPEG_GRAYSCALE   4
#define CODING_JBIG             5

#define HIGH_ORDER	3
#define MIDDLE_ORDER	2
#define LOW_ORDER		1

#if __TX_REPORT_IMAGE_PRINT__
#define REPORT_IMAGE_MAX_LEN 1800

extern char *pTxResultRptImageName;
extern int getgFaxTxResultRptImage(void);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
#endif

extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern int DiscardLength;
extern UINT16 fsu_job_num_prn;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern int is1stPageDataTransfered;
extern uint8_t *faxapp_print_buffer;

extern UINT8 isSDCardEnable;
extern char *pRootFolderARIA;
extern UINT8 isDKSolution;

extern int getAutoReductionOn(void);
extern void fsu_gen_fax_header(char *PBMFileName, UINT8 faxRes, UINT8 fax_image_width_type);
extern BOOLEAN fsu_get_tti(void);
extern uint8 Get_TrayInfo(uint8 Tray_num);
extern error_type_t paper_get_media_size(mediasize_t mediaSize, uint32_t *width, uint32_t *height);
extern UINT8 PCFax_get_no_of_recipient(void);
extern UINT8 fsu_get_width_info(UINT8 res, UINT32 Width);
extern UINT8 fsu_get_print_reason (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern void fsu_update_width (UINT16 jnum, UINT8 width);
extern void fsu_delete_file_on_file_system (const char *Path);
extern int faxapp_get_print_media_size_tray_1 (void);
extern int faxapp_get_print_media_size_tray_2 (void);
extern int faxapp_get_print_media_size_tray_3 (void);
extern int faxapp_get_print_media_size_tray_0 (void);
extern int faxapp_get_print_default_src_tray (void);
extern UINT8 fsu_get_photo_mode (UINT16 jnum);
extern int getFaxInputTray(void);
extern void fsu_update_split_print (UINT16 jnum, UINT8 splitprint);
extern UINT8 fsu_get_isDuplex (UINT16 jnum);
extern int getDuplexPrintOn(void);
extern void fsu_update_isDuplex (UINT16 jnum, UINT8 Duplex);
#ifdef HAVE_IMAGELOG
extern int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
#endif
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT8 fsu_get_width (UINT16 jnum);
extern UINT8 fsu_requset_get_current_page_no_from_Job_Queue_File (UINT16 jnum);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);

int T4_encoding(int Page, char *FileName, int codingtype, int faxRes, int job_num_send, UINT8 Direction, int photo_image_resolution);
uint8_t* reduce_mipmap(uint8_t *_data, int newWidth, int newHeight, int _width, int _height);
void fax_rotate(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int margin);
void fax_rotate_for_print(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int strip_real_width, int margin);
int T4_encoded_image_validation_check(UINT16 docID, char *FileName, int image_width, int rx_resolution, int coding_type);
int copyDecodeFaxFileTest(TIFF* tifin, TIFF* tifout, uint32 x_size, char *refbuffer, char *rowbuffer, uint16 *badruncheck);
uint8_t * bilinear(uint8_t *img, int newWidth, int newHeight, int width, int height);

int change_width = 0;
int change_height = 0;
int need_image_rotation = 0;
uint32_t paper_width_pixel = 0;
uint32_t paper_height_pixel = 0;
int paper_pixel_per_mm = 0;
extern mediasize_t paper_enum;
int input_tray = 0;
int isEncodeComplete = 1;
int planetype = 0;
static size_t m_width = 0;
static size_t m_height = 0;
uint8_t* m_data = (uint8_t *)NULL;
uint8 minBytePerLine = 0;
int txImageThreshold = 127;

typedef struct{
	int enable;
	int size;
}t_Tray_info;

int image_dump_index = 0;
void fax_image_dump(int width, int height, uint8_t * buffer, int type)//0: 1bit 1:8bit
{
	char filename[MAX_LEN_FILE_PATH_NAME] = {0, };
	FILE *ff = NULL;
	printf("%s() image_dump_index:%d\n",__FUNCTION__, image_dump_index);
	
	if(type == 0) //1bit
	{
		snprintf(filename,sizeof(filename),"/dump_%d.pbm",image_dump_index);
		ff = fopen(filename,"wb");
		if(ff == NULL)
		{
			return;
		}

		fprintf(ff, "P4\n%d %d\n", width, height);
		
		if(fwrite(buffer, (width*height) >> 3, 1, ff) != (width*height) >> 3)
		{
			printf("%s() image_dump FAil: %d !!\n",__FUNCTION__, image_dump_index);
			image_dump_index++;
			fclose(ff);
			return;
		}
		
		fclose(ff);
	}
	else //8bit
	{
		snprintf(filename,sizeof(filename),"/dump_%d.pgm",image_dump_index);
		ff = fopen(filename,"wb");
		if(ff == NULL)
		{
			return;
		}

		fprintf(ff, "P5\n%d %d\n255\n", width, height);
		
		if(fwrite(buffer, (width*height), 1, ff) != (width*height))
		{
			printf("%s() image_dump FAIL:%d !!\n",__FUNCTION__,image_dump_index);
			image_dump_index++;
			fclose(ff);
			return;
		}
		
		fclose(ff);
	}
	
	image_dump_index++;
}


unsigned long getint(FILE *f)
{
  int c;
  unsigned long i = 0;

  while ((c = getc(f)) != EOF && !isdigit(c))
    if (c == '#')
      while ((c = getc(f)) != EOF && !(c == 13 || c == 10)) ;
  if (c != EOF) {
    ungetc(c, f);
    if(fscanf(f, "%lu", &i) == EOF)
    //if(fread(&i,sizeof(i),1,f) < 1)
    {
		XASSERT(0,0);
	}
  }

  return i;
}


void data_out(unsigned char *start, size_t len, void *file)
{
  fwrite(start, len, 1, (FILE *) file);
  return;
}


int MakePBMToFaxImage (int PageIndex)
{
    int size, c;
    char type;
    FILE *fhandle = NULL;
    char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char Raw8bppFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    unsigned char res = 0;
    UINT16 doc_id = 0;
    int i, j, tempchar;
    int wid = 0;
    int hig = 0;
    int ret_val = 0;
    uint8_t *data = (uint8_t *)NULL;
    UINT16 jnum = fsu_job_num_prn;

    res = fsu_get_resolution(jnum);

    wid = (fsu_get_image_width_pixel(jnum) / 8);
    hig = fsu_get_image_height_pixel(jnum);

    doc_id = fsu_get_doc_id(jnum);

    // Step 1. Open the PBM file
    snprintf(PBMFileName, sizeof(PBMFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, doc_id, PATH_SEPARATOR, PageIndex);
    printf("\n(YHCHO) %s() - PBMFileName: %s\n", __FUNCTION__, PBMFileName);
    fhandle = fopen(PBMFileName, "rb");
    if(!fhandle)
    {
        printf("\n(YHCHO) %s() - Check Point [1]\n", __FUNCTION__);

        XASSERT(0,0);
    }
    else
    {
        data = (uint8_t *)MEM_MALLOC(wid*hig);
        if (data == (uint8_t *)NULL)
        {
            //printf("\n(YHCHO) %s() - Check Point [2]\n", __FUNCTION__);
            XASSERT(0,0);
        }
        
        /* read PBM header */
        while ((c = getc(fhandle)) != EOF && (isspace(c) || c == '#'))
			if (c == '#')
			  while ((c = getc(fhandle)) != EOF && !(c == 13 || c == 10)) ;
        type = (char)getc(fhandle);
        if (c != 'P' || (type != '1' && type != '4')) {
			fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", PBMFileName);
			fclose(fhandle);

			return 1;
        }
        m_width = getint(fhandle);
        m_height = getint(fhandle);
        fgetc(fhandle);    /* skip line feed */
        if(c == EOF)
        {
            fprintf(stderr, "Input file '%s' is Empty or Read Fail (skip line feed detect fail)\n", PBMFileName);
            fclose(fhandle);

            return 1;
        }

        size = fread(data, wid*hig, 1, fhandle);
        if (size < 0)
        {
            //printf("\n(YHCHO) %s() - Check Point [3]\n", __FUNCTION__);
            XASSERT(0,size);
        }
        else
        {
            m_width = fsu_get_image_width_pixel(jnum);
            m_height = fsu_get_image_height_pixel(jnum);

            printf("\n(YHCHO) %s() - m_width = %d, m_height = %d\n", __FUNCTION__,m_width,m_height);

            m_data = (uint8_t *)MEM_MALLOC(m_width*m_height);
            if (m_data == (uint8_t *)NULL)
            {
                //printf("\n(YHCHO) %s() - Check Point [4]]\n", __FUNCTION__);
                XASSERT(0,0);
            }

            // Convert 1-bit to 8-bit
            j = 0;
            for(i=0; i<wid*hig;)
            {
                tempchar = data[i++];

                m_data[j++]=(tempchar&0x80)? 255 : 0;
                m_data[j++]=(tempchar&0x40)? 255 : 0;
                m_data[j++]=(tempchar&0x20)? 255 : 0;
                m_data[j++]=(tempchar&0x10)? 255 : 0;
                m_data[j++]=(tempchar&0x08)? 255 : 0;
                m_data[j++]=(tempchar&0x04)? 255 : 0;
                m_data[j++]=(tempchar&0x02)? 255 : 0;
                m_data[j++]=(tempchar&0x01)? 255 : 0;
            }

            fclose(fhandle);
            MEM_FREE_AND_NULL(data);

            snprintf(Raw8bppFileName, sizeof(Raw8bppFileName), "%s%cDoc%04X%cfax%02X.raw", pRootFolderNameRAM, PATH_SEPARATOR, doc_id, PATH_SEPARATOR, PageIndex);

            ret_val = raw_to_fax_image(Raw8bppFileName, jnum);
            if (ret_val == 0)
            {
                unlink(PBMFileName);
            }
        }
    }

    return ret_val;
}


void ScaleLineAvg(uint8_t *Target, uint8_t *Source, int SrcWidth, int TgtWidth)
{
    /* N.B. because of several simplifications of the algorithm,
    *      the zoom range is restricted between 0.5 and 2. That
    *      is: TgtWidth must be >= SrcWidth/2 and <= 2*SrcWidth.
    */
    int NumPixels = TgtWidth;
    int Mid = TgtWidth / 2;
    int E = 0;
    uint8_t p;

    if (TgtWidth > SrcWidth)
        NumPixels--;

    while (NumPixels-- > 0)
    {
        p = *Source;
        if (E >= Mid)
            p = AVERAGE(p, *(Source+1));

        *Target++ = p;
        E += SrcWidth;
        if (E >= TgtWidth)
        {
            E -= TgtWidth;
            Source++;
        } /* if */
    } /* while */

    if (TgtWidth > SrcWidth)
        *Target = *Source;
}


void ScaleRect(uint8_t *Target, uint8_t *Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight)
{
    int NumPixels = TgtHeight;
    int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
    int FractPart = SrcHeight % TgtHeight;
    int E = 0;
    uint8_t *PrevSource = NULL;

    while (NumPixels-- > 0)
    {
        if (Source == PrevSource)
        {
            memcpy(Target, Target-TgtWidth, TgtWidth*sizeof(*Target));
        }
        else
        {
            ScaleLineAvg(Target, Source, SrcWidth, TgtWidth); //smoothing
            PrevSource = Source;
        } /* if */

        Target += TgtWidth;
        Source += IntPart;
        E += FractPart;

        if (E >= TgtHeight)
        {
            E -= TgtHeight;
            Source += SrcWidth;
        } /* if */
    } /* while */
}

mediasize_t getPageSizeEnum_FromTrayInfo(int num)
{
	unsigned char m_tray_info = 0;
	mediasize_t page_enum = 0;
	m_tray_info = Get_TrayInfo(num);
	
	switch(m_tray_info & 0x70)
	{
		case 0x10: //unknown
			//page_enum = MEDIASIZE_ANY;
			if(gPaperType == A4_BASE)
			{
				page_enum = MEDIASIZE_A4;
			}
			else //LT_BASE
			{
				page_enum = MEDIASIZE_LETTER;
			}
			break;
		case 0x20: //letter
			page_enum = MEDIASIZE_LETTER;
			break;
		case 0x30: //exec
			page_enum = MEDIASIZE_EXECUTIVE;
			break;
		case 0x40: //a4
			page_enum = MEDIASIZE_A4;
			break;
		case 0x50: //b5
			page_enum = MEDIASIZE_B5;
			break;
		case 0x60: //legal
			page_enum = MEDIASIZE_LEGAL;
			break;
		case 0x70: //a5
			page_enum = MEDIASIZE_A5;
			break;
		default:
			XASSERT(0,0);
            break;
	}
	
	printf("\n(YHCHO) %s() : %d",__FUNCTION__,page_enum);
	
	return page_enum;
}


#if 0
int getMPTTraySensorOn(void)
{
	unsigned char m_tray_info = 0;
	int sensorOn = 0;
	m_tray_info = Get_TrayInfo(4);
	
	switch(m_tray_info & 0x03)
	{
		case 0x01: //mpt sensor off
			sensorOn = 0;
			break;
		case 0x03: //mpt sensor on
			sensorOn = 1;
			break;
		default:
			XASSERT(0,0);
	}
	
	return sensorOn;
}
#endif

int faxapp_get_print_tray_auto(int image_height)
{
	t_Tray_info m_Tray[4];
	mediasize_t tray_paper_enum = 0;
	mediasize_t high_order_paper_size = 0;
	mediasize_t middle_order_paper_size = 0;
	int i = 0;
	int default_tray = 0;//1;


	if(gPaperType == A4_BASE)
	{
		high_order_paper_size = MEDIASIZE_A4;
		middle_order_paper_size = MEDIASIZE_LETTER;
	}
	else // LT_BASE
	{
		high_order_paper_size = MEDIASIZE_LETTER;
		middle_order_paper_size = MEDIASIZE_A4;
	}

	if(Get_TrayInfo(1) != 0)
	{
		if(Get_TrayInfo(1)&0x02)
		{
			m_Tray[0].enable = 1;
		}
		tray_paper_enum = faxapp_get_print_media_size_tray_1();
		if(tray_paper_enum == 554)
		{
			tray_paper_enum = getPageSizeEnum_FromTrayInfo(1);
		}

		if(tray_paper_enum == high_order_paper_size)
		{
			if(m_Tray[0].enable == 1)
			{
				return 1;
			}
			else
			{
				m_Tray[0].size = HIGH_ORDER;
			}
		}
		if(tray_paper_enum == middle_order_paper_size)
		{
			m_Tray[0].size = MIDDLE_ORDER;
		}
		else
		{
			m_Tray[0].size = LOW_ORDER;
		}
	}
	
	if(Get_TrayInfo(2) != 0)
	{
		if(Get_TrayInfo(2)&0x02)
		{
			m_Tray[1].enable = 1;
		}
		tray_paper_enum = faxapp_get_print_media_size_tray_2();
		if(tray_paper_enum == 554)
		{
			tray_paper_enum = getPageSizeEnum_FromTrayInfo(2);
		}

		if(tray_paper_enum == high_order_paper_size)
		{
			if(m_Tray[1].enable == 1)
			{
				return 2;
			}
			else
			{
				m_Tray[1].size = HIGH_ORDER;
			}
		}
		if(tray_paper_enum == middle_order_paper_size)
		{
			m_Tray[1].size = MIDDLE_ORDER;
		}
		else
		{
			m_Tray[1].size = LOW_ORDER;
		}
	}
	
	if(Get_TrayInfo(3) != 0)
	{
		if(Get_TrayInfo(3)&0x02)
		{
			m_Tray[2].enable = 1;
		}
		tray_paper_enum = faxapp_get_print_media_size_tray_3();
		if(tray_paper_enum == 554)
		{
			tray_paper_enum = getPageSizeEnum_FromTrayInfo(3);
		}

		if(tray_paper_enum == high_order_paper_size)
		{
			if(m_Tray[2].enable == 1)
			{
				return 3;
			}
			else
			{
				m_Tray[2].size = HIGH_ORDER;
			}
		}
		if(tray_paper_enum == middle_order_paper_size)
		{
			m_Tray[2].size = MIDDLE_ORDER;
		}
		else
		{
			m_Tray[2].size = LOW_ORDER;
		}
	}
	
	if(Get_TrayInfo(4)&0x02)
	{
		m_Tray[3].enable = 1;
	}
	tray_paper_enum = faxapp_get_print_media_size_tray_0();
	if(tray_paper_enum == high_order_paper_size)
	{
		if(m_Tray[3].enable == 1)
		{
			return 4;
		}
		else
		{
			m_Tray[3].size = HIGH_ORDER;
		}
	}
	if(tray_paper_enum == middle_order_paper_size)
	{
		m_Tray[3].size = MIDDLE_ORDER;
	}
	else
	{
		m_Tray[3].size = LOW_ORDER;
	}
	
	//check middle order Tray
	for(i = 0; i < 4; i++)
	{
		if(m_Tray[i].enable == 1 && m_Tray[i].size == MIDDLE_ORDER)
		{
			return i+1;
		}
	}

	default_tray = faxapp_get_print_default_src_tray();
	if(!(default_tray == FAX_INPUT_TRAY_MANUAL
		|| default_tray == FAX_INPUT_TRAY_1
		|| default_tray == FAX_INPUT_TRAY_2
		|| default_tray == FAX_INPUT_TRAY_3))
	{
		default_tray = FAX_INPUT_TRAY_1;
	}

	if(m_Tray[default_tray-1].enable == 1)
	{
		return default_tray;
	}

	for(i = 0; i < 4; i++)
	{
		if(m_Tray[i].enable == 1)
		{
			return i+1;
		}
	}

	//return faxapp_get_print_default_src_tray();
	return 8;
}

int raw_to_fax_image(char * output_filename, UINT16 jnum)
{
    unsigned char res = 0;
    int ret_val = 0;
    uint8_t *rotation_buf = (uint8_t *)NULL;
    int selected_tray = 0;//1;
    int fax_input_tray = 0;

    uint32_t paper_width_mmx100 = 0;
    uint32_t paper_height_mmx100 = 0;

    //int mpt_sensor_check = 0;
    //error_type_t db_retval = FAIL;
    //
    printf("\n(YHCHO) %s() : %x",__FUNCTION__,Get_TrayInfo(1));
    printf("\n(YHCHO) %s() : %x",__FUNCTION__,Get_TrayInfo(2));
    printf("\n(YHCHO) %s() : %x",__FUNCTION__,Get_TrayInfo(3));
    printf("\n(YHCHO) %s() : %x",__FUNCTION__,Get_TrayInfo(4));
    /*
    TRAY_MEDIA_UNKNOWN		0x10
    TRAY_MEDIA_LT			0x20	1
    TRAY_MEDIA_EXEC			0x30	7
    TRAY_MEDIA_A4			0x40	9
    TRAY_MEDIA_B5			0x50	13
    TRAY_MEDIA_LG			0x60	5
    TRAY_MEDIA_A5			0x70	11

    TRAY_MEDIA_NORMAL		0x03
    TRAY_MEDIA_LOW 			0x02
    TRAY_MEDIA_EMPTY		0x01
    */
    while(!Get_TrayInfo(1))
    {
        printf("\n(YHCHO) %s() Wait wake up engine ...\n",__FUNCTION__);
        usleep(100*1000);
    }

    res = fsu_get_resolution(jnum);

    printf("\n(YHCHO) %s() - res = %d\n", __FUNCTION__,res);
    if (res == FAX_VR_STANDARD)
    {
        change_width = m_width*(1.5); //m_width
        change_height = (m_height*3);// * (100+((100*paper_width_pixel)/m_width)))/100;
    }
    else if (res == FAX_VR_FINE)
    {
        change_width = m_width*(1.5);
        change_height = (m_height*(1.5));
    }
    else
    {
        change_width = m_width;
        change_height = (m_height);
    }

    fax_input_tray = getFaxInputTray();
    printf("\n(YHCHO) %s() - FaxInputTray = %d\n", __FUNCTION__, fax_input_tray);

    if(fax_input_tray == FAX_INPUT_TRAY_2 && Get_TrayInfo(2) == 0)
    {
        printf("\n(YHCHO) %s() - Tray[%d]: not setting !!!\n", __FUNCTION__,FAX_INPUT_TRAY_2);
        fax_input_tray = FAX_INPUT_TRAY_AUTO;
    }
    else if(fax_input_tray == FAX_INPUT_TRAY_3 && Get_TrayInfo(3) == 0)
    {
        printf("\n(YHCHO) %s() - Tray[%d]: not setting !!!\n", __FUNCTION__,FAX_INPUT_TRAY_3);
        fax_input_tray = FAX_INPUT_TRAY_AUTO;
    }

    switch(fax_input_tray)
    {
        case FAX_INPUT_TRAY_1:
            paper_enum = faxapp_get_print_media_size_tray_1();
            if(paper_enum == 554)
            {
                paper_enum = getPageSizeEnum_FromTrayInfo(1);
            }
            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_1 -> paper_enum: %d\n", __FUNCTION__, paper_enum);
            paper_get_media_size(paper_enum, &paper_width_mmx100, &paper_height_mmx100);
            paper_width_pixel = (paper_width_mmx100*300) / 2540;
            paper_width_pixel = ((double)paper_width_pixel/32 + 0.5);
            paper_width_pixel = paper_width_pixel * 32;
            paper_height_pixel = (paper_height_mmx100*300) / 2540;
            input_tray = 1;
            break;

        case FAX_INPUT_TRAY_2:
            paper_enum = faxapp_get_print_media_size_tray_2();
            if(paper_enum == 554)
            {
                paper_enum = getPageSizeEnum_FromTrayInfo(2);
            }
            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_2 -> paper_enum: %d\n", __FUNCTION__, paper_enum);
            paper_get_media_size(paper_enum, &paper_width_mmx100, &paper_height_mmx100);
            paper_width_pixel = (paper_width_mmx100*300) / 2540;
            paper_width_pixel = ((double)paper_width_pixel/32 + 0.5);
            paper_width_pixel = paper_width_pixel * 32;
            paper_height_pixel = (paper_height_mmx100*300) / 2540;
            input_tray = 2;
            break;

        case FAX_INPUT_TRAY_3:
            paper_enum = faxapp_get_print_media_size_tray_3();
            if(paper_enum == 554)
            {
                paper_enum = getPageSizeEnum_FromTrayInfo(3);
            }
            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_3 -> paper_enum: %d\n", __FUNCTION__, paper_enum);
            paper_get_media_size(paper_enum, &paper_width_mmx100, &paper_height_mmx100);
            paper_width_pixel = (paper_width_mmx100*300) / 2540;
            paper_width_pixel = ((double)paper_width_pixel/32 + 0.5);
            paper_width_pixel = paper_width_pixel * 32;
            paper_height_pixel = (paper_height_mmx100*300) / 2540;
            input_tray = 3;
            break;

        case FAX_INPUT_TRAY_MANUAL:
            paper_enum = faxapp_get_print_media_size_tray_0();
            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_MANUAL -> paper_enum: %d\n", __FUNCTION__, paper_enum);
            paper_get_media_size(paper_enum, &paper_width_mmx100, &paper_height_mmx100);
            paper_width_pixel = (paper_width_mmx100*300) / 2540;
            paper_width_pixel = ((double)paper_width_pixel/32 + 0.5);
            paper_width_pixel = paper_width_pixel * 32;
            paper_height_pixel = (paper_height_mmx100*300) / 2540;
            input_tray = 0;
            break;

        case FAX_INPUT_TRAY_AUTO:
            selected_tray = faxapp_get_print_tray_auto(change_height);
            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_AUTO -> selected_tray: %d\n", __FUNCTION__, selected_tray);
            if(selected_tray == 1)
            {
                paper_enum = faxapp_get_print_media_size_tray_1();
                if(paper_enum == 554)
                {
                    paper_enum = getPageSizeEnum_FromTrayInfo(1);
                }
            }
            else if(selected_tray == 2)
            {
                paper_enum = faxapp_get_print_media_size_tray_2();
                if(paper_enum == 554)
                {
                    paper_enum = getPageSizeEnum_FromTrayInfo(2);
                }
            }
            else if(selected_tray == 3)
            {
                paper_enum = faxapp_get_print_media_size_tray_3();
                if(paper_enum == 554)
                {
                    paper_enum = getPageSizeEnum_FromTrayInfo(3);
                }
            }
            else if(selected_tray == 4)
            {
                paper_enum = faxapp_get_print_media_size_tray_0();
            }
            else if(selected_tray == 8)
            {
                paper_enum = MEDIASIZE_A4;
            }

            printf("\n(YHCHO) %s() - FAX_INPUT_TRAY_AUTO -> paper_enum: %d\n", __FUNCTION__, paper_enum);
            paper_get_media_size(paper_enum, &paper_width_mmx100, &paper_height_mmx100);
            paper_width_pixel = (paper_width_mmx100*300) / 2540;
            paper_width_pixel = ((double)paper_width_pixel/32 + 0.5);
            paper_width_pixel = paper_width_pixel * 32;
            paper_height_pixel = (paper_height_mmx100*300) / 2540;
#if 0
            if(selected_tray == 4)
            {
                input_tray = 0;
            }
            else
            {
                input_tray = selected_tray;
            }
#else
            input_tray = 8;
#endif
            break;

        default:
            XASSERT(0,0);
            break;
    }

    //only 1st page can setting duplex value.
    if(fsu_get_isDuplex(jnum) == FAX_PRINT_ISDUPLEX_INITIAL)
    {
        if(getDuplexPrintOn() && (paper_enum == MEDIASIZE_A4 || paper_enum == MEDIASIZE_LETTER || paper_enum == MEDIASIZE_LEGAL || paper_enum == MEDIASIZE_CUSTOM_85x13))
        {
            printf("\n(YHCHO) %s() - set Dulex print\n", __FUNCTION__);
            fsu_update_isDuplex(jnum,ENABLE); //duplex
        }
        else
        {
            printf("\n(YHCHO) %s() - set Simplex print\n", __FUNCTION__);
            fsu_update_isDuplex(jnum,DISABLE); //simplex
        }
    }

    if(getAutoReductionOn() == ENABLE)
    {
        if(change_width > paper_width_pixel)
        {
            //change_height = (change_height * paper_width_pixel)/change_width;
            change_width = paper_width_pixel;
        }
    }
    else
    {
        change_height = (change_height * 2560)/change_width;
        change_width = 2560;
    }

    paper_pixel_per_mm = paper_height_pixel/(paper_height_mmx100/100);

    printf("\n(YHCHO) %s() Auto Reduction check : %d\n", __FUNCTION__,getAutoReductionOn());
    printf("\n(YHCHO) %s() %d, %d, %ld, %d\n", __FUNCTION__,change_width,change_height, paper_height_pixel, paper_pixel_per_mm);
    need_image_rotation = 0;
    fsu_update_split_print(jnum, 0);
    if(getAutoReductionOn() == ENABLE)
    {
        if(change_height - (DiscardLength * paper_pixel_per_mm) > paper_height_pixel)
        {
            change_height = paper_height_pixel;
        }
    }
    else
    {
        if(change_height - (DiscardLength * paper_pixel_per_mm) > paper_height_pixel)
        {
            fsu_update_split_print(jnum, 1);
        }

        if(change_width > paper_width_pixel + 100)
        {
            fsu_update_split_print(jnum, 1);
            need_image_rotation = 1;
        }
    }

    if(change_height % 2 == 1)
    {
        printf("\n(YHCHO) %s() video laser do not print 1 pixel\n", __FUNCTION__);
        change_height = change_height - 1;
    }

#if 1
    faxapp_print_buffer = bilinear(m_data, change_width, change_height, m_width, m_height);
#else
    if(res == FAX_VR_STANDARD || res == FAX_VR_FINE)
    {
        faxapp_print_buffer = (uint8_t *)MEM_MALLOC(change_width*change_height);
        if (faxapp_print_buffer == (uint8_t *)NULL)
        {
            XASSERT(0,0);
        }
        ScaleRect(faxapp_print_buffer, m_data, m_width, m_height, change_width, change_height);
    }
    else
    {
        //faxapp_print_buffer = reduce_mipmap(m_data,change_width,change_height,m_width,m_height);
        faxapp_print_buffer = bilinear(m_data, change_width, change_height, m_width, m_height);
    }
#endif

    MEM_FREE_AND_NULL(m_data);

    if(need_image_rotation) // rotation
    {
        printf("\n(YHCHO) %s() need_image_rotation : %d\n", __FUNCTION__,need_image_rotation);

        if(change_height > paper_width_pixel) // 2page print
        {
            rotation_buf = (uint8_t *)MEM_MALLOC(paper_width_pixel*paper_height_pixel*2);
            if (rotation_buf == (uint8_t *)NULL)
            {
                XASSERT(0,0);
            }
            memset(rotation_buf,0, paper_width_pixel*paper_height_pixel*2);
            if(change_width>paper_height_pixel)
            {
                fax_rotate_for_print(&faxapp_print_buffer[0], &rotation_buf[0], paper_width_pixel, paper_height_pixel,
                                        paper_width_pixel,change_width, (change_width-paper_height_pixel)/2);
                if(change_height-paper_width_pixel >= paper_width_pixel) // very long image.
                {
                    printf("\n(YHCHO) %s() need_image_rotation 1-1\n", __FUNCTION__);
                    fax_rotate_for_print(&faxapp_print_buffer[change_width*paper_width_pixel], &rotation_buf[paper_height_pixel*paper_width_pixel],
                            paper_width_pixel, paper_height_pixel, paper_width_pixel, change_width,(change_width-paper_height_pixel)/2);
                }
                else
                {
                    printf("\n(YHCHO) %s() need_image_rotation 1-2\n", __FUNCTION__);
                    fax_rotate_for_print(&faxapp_print_buffer[change_width*paper_width_pixel], &rotation_buf[paper_height_pixel*paper_width_pixel],
                            paper_width_pixel, paper_height_pixel, change_height-paper_width_pixel, change_width,(change_width-paper_height_pixel)/2);
                }
            }
            else
            {
                fax_rotate(&faxapp_print_buffer[0], &rotation_buf[0], paper_width_pixel, change_width, paper_width_pixel, 0);
                if(change_height-paper_width_pixel >= paper_width_pixel)
                {
                    printf("\n(YHCHO) %s() need_image_rotation 2-1\n", __FUNCTION__);
                    fax_rotate(&faxapp_print_buffer[change_width*paper_width_pixel], &rotation_buf[paper_height_pixel*paper_width_pixel],
                            paper_width_pixel, change_width, paper_width_pixel, 0);
                }
                else
                {
                    printf("\n(YHCHO) %s() need_image_rotation 2-2\n", __FUNCTION__);
                    fax_rotate(&faxapp_print_buffer[change_width*paper_width_pixel], &rotation_buf[paper_height_pixel*paper_width_pixel],
                            paper_width_pixel, change_width, change_height-paper_width_pixel, 0);
                }
            }

            change_width = paper_width_pixel;
            change_height = paper_height_pixel * 2;
        }
        else // 1page print
        {
            rotation_buf = (uint8_t *)MEM_MALLOC(paper_width_pixel*paper_height_pixel);
            if (rotation_buf == (uint8_t *)NULL)
            {
                XASSERT(0,0);
            }
            memset(rotation_buf,0, paper_width_pixel*paper_height_pixel);

            if(change_width>paper_height_pixel)
            {
                fax_rotate_for_print(&faxapp_print_buffer[0], &rotation_buf[0], paper_width_pixel, paper_height_pixel,
                                        change_height, change_width, (change_width-paper_height_pixel)/2);
            }
            else
            {
                fax_rotate(&faxapp_print_buffer[0], &rotation_buf[0], paper_width_pixel, change_width, change_height, 0);
            }
            change_width = paper_width_pixel;
            change_height = paper_height_pixel;
        }

        printf("\n(YHCHO) %s() image_rotation complete\n", __FUNCTION__);
        MEM_FREE_AND_NULL(faxapp_print_buffer);
        PTR_ASSIGN(faxapp_print_buffer,rotation_buf);
    }//rotation end

    //area = change_width * change_height;

    //fwrite(&faxapp_print_buffer[0], area, 1, out_image);

    // free buffers
    //MEM_FREE_AND_NULL(faxapp_print_buffer);

    return ret_val;
}


static int _do_pixel(uint8_t* c, short* e, int x, int mode)
{
    int v = 0;
	if(mode)
	{
		v= e[x] + c[x];

		if (v > 127)
		{
			c[x] = 255;
		}
		else
		{
			c[x] = 0;
		}

		return v - c[x];		
	}
	else
	{
		v= e[x] + (255-c[x]);

		if (v > 127)
		{
			c[x] = 255;
		}
		else
		{
			c[x] = 0;
		}

		return v - c[x];		
	}
}


static void _dither(short* p,         /* Prev error */
                    uint8_t* c, /* Current data */
                    short* n,         /* Next error */
                    int w, int dir, int f_mode, uint8_t f_image_type)
{
    /* This is thresholding which does not work well for images */
	if(f_image_type == 0)
	{
		int i;
		
		if(f_mode) //pc fax
		{
			  for (i=0;i<w;i++)
				if (c[i] > txImageThreshold) c[i] = 255;
				else            c[i] = 0;			
		}
		else //fax scan
		{
			  for (i=0;i<w;i++)
				if (c[i] > txImageThreshold) c[i] = 0;
				else            c[i] = 255;
		}
	}
	else //if(f_image_type == 1)
	{
	/* What follows is serpentine floyd stienberg */
	  int i, e;

	  memset(n, 0, w*2);
	  if (dir)
	  {
			e = _do_pixel(c, p, i=0, f_mode);
			p[i+1] +=    e >> 1;
			n[i+1] += (1*e) >> 3;
			n[i]   += (3*e) >> 3;
			for (i=1;i<(w-1);i++)
			{
			  e = _do_pixel(c, p, i, f_mode);
			  p[i+1] += (7*e) >> 4;
			  n[i+1] += (1*e) >> 4;
			  n[i]   += (5*e) >> 4;
			  n[i-1] += (3*e) >> 4;
			}
			e = _do_pixel(c, p, i=w-1, f_mode);
			n[i]   += (5*e) >> 3;
			n[i-1] += (3*e) >> 3;
	  }
	  else
	  {
			e = _do_pixel(c, p, i=w-1, f_mode);
			p[i-1] +=    e >> 1;
			n[i-1] += (1*e) >> 3;
			n[i]   += (3*e) >> 3;
			for (i=w-2;i>0;i--)
			{
			  e = _do_pixel(c, p, i, f_mode);
			  p[i-1] += (7*e) >> 4;
			  n[i-1] += (1*e) >> 4;
			  n[i]   += (5*e) >> 4;
			  n[i+1] += (3*e) >> 4;
			}
			e = _do_pixel(c, p, i=0, f_mode);
			n[i]   += (5*e) >> 3;
			n[i+1] += (3*e) >> 3;
	  }
	}
}


static void _pack(uint8_t* out, uint8_t* in, int w)
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


int gray2bw_convert(void* hdl, uint8_t* data, int numrows, int fax_mode, uint8_t image_type)
{
    _cntxt_t* c = (_cntxt_t*)hdl;
    uint8_t* in = data;
    uint8_t* out = data;

    while (numrows--)
    {
        _dither(c->rows[c->which], in, c->rows[c->which^1], c->width, c->which, fax_mode, image_type);
        _pack(out, in, c->width);
        in += c->width;
        out += (c->width+7)/8;
        c->which ^= 1;
    }

    return 0;
}


uint32_t gray2bw_memreq(int width)
{
    return sizeof(_cntxt_t) + 4*width;
}


void* gray2bw_create(int width, uint32_t* mem, int mem_bytes)
{
    _cntxt_t* hdl;

    if (mem_bytes < gray2bw_memreq(width))
        return NULL;

    memset(mem, 0, gray2bw_memreq(width));
    hdl = (_cntxt_t*)mem;
    mem += sizeof(_cntxt_t) >> 2;
    hdl->rows[0] = (short*)mem;
    hdl->rows[1] = hdl->rows[0] + width;
    hdl->width = width;

    return hdl;
}


int otsuThreshold(uint8_t* img, int height, int width)  
{
	int T = 0;//  
	int step      = width;
	uint8_t* data  = img;
	double gSum0;//  
	double gSum1;//  
	double N0 = 0;//  
	double N1 = 0;//  
	double u0 = 0;//  
	double u1 = 0;//  
	double w0 = 0;//?0  
	double w1 = 0;//?1  
	double u = 0;//  
	double tempg = -1;//  
	double g = 0;//-1;//  
	double Histogram[256]={0};// = new double[256];//  
	double N = width*height;//  
	int i,j,k;
	
#define STATIC_TARGET 185	// samsung SCX-4824 : 140 (low...but use strong sharpen filter)
#define OTSU_LIMIT_MAX (STATIC_TARGET + 50)
#define OTSU_LIMIT_MIN (STATIC_TARGET - 50)
//#define MAX(a,b)        ((a) > (b) ? (a) : (b))
//#define MIN(a,b)        ((a) < (b) ? (a) : (b))
	
	for(i=0;i<height;i++)  
	{//  
		for(j=0;j<width;j++)  
		{  
			int temp =data[i*step + j];
			temp = temp<0? 0:temp;  
			temp = temp>255? 255:temp;  
			Histogram[temp]++;  
		}   
	}  
	//  
	for (i = 0;i<256;i++)  
	{  
		gSum0 = 0;  
		gSum1 = 0;  
		N0 += Histogram[i];           
		N1 = N-N0;  
		if(0==N1)break;//?  
		w0 = N0/N;  
		w1 = 1-w0;  
		for (j = 0;j<=i;j++)  
		{  
			gSum0 += j*Histogram[j];  
		}  
		u0 = gSum0/N0;  
		for(k = i+1;k<256;k++)  
		{  
			gSum1 += k*Histogram[k];  
		}  
		u1 = gSum1/N1;  
		u = w0*u0 + w1*u1;  
		g = w0*w1*(u0-u1)*(u0-u1);  
		if (tempg<g)  
		{  
			tempg = g;  
			T = i;  
		}  
	}  

#if 1	// add.lsh to resolve QP-00010442 (duplex backside show-through problem)
	printf("[IP] Image Processing : min:%d, max:%d, target:%d \n",	OTSU_LIMIT_MIN, OTSU_LIMIT_MAX, STATIC_TARGET);	
	
	/* Otsu Threshold */
	// Range Check
	if(T < OTSU_LIMIT_MIN){
		printf("%s() Under limit(%d)!! : %d -> %d\n", __FUNCTION__, OTSU_LIMIT_MIN, T, (OTSU_LIMIT_MIN+STATIC_TARGET)/2);
		T = (OTSU_LIMIT_MIN+STATIC_TARGET)/2;
	}
	else if(T > OTSU_LIMIT_MAX){
		printf("%s() Over limit(%d)!! : %d -> %d\n", __FUNCTION__, OTSU_LIMIT_MAX, T, (OTSU_LIMIT_MAX+STATIC_TARGET)/2);
		T = (OTSU_LIMIT_MAX+STATIC_TARGET)/2;
	}
	else{
		printf("%s() THRESHOLD (Otsu) : %d -> %d\n", __FUNCTION__, T, (T+STATIC_TARGET)/2);
		T = (T+STATIC_TARGET)/2;	// STATIC_TARGET : Fixed Threshold Target, T = Otsu Result
	}
#endif
	
#if 0	
	for (i = 0;i<256;i++)  
		printf("%d\n", (int)Histogram[i]);
#endif		
	return T;   
}


void conv(int scale, int d_len, unsigned char *src, unsigned char *dst)
{
	int i, j, p;
	int x[4], y;
	x[0] = 255;
	x[1] = *src++;
	x[2] = *src++;
	x[3] = *src++;
	i = j = 0;
	p = 0;
	
	while(i<d_len && j < d_len)
	{
		if(p < scale - 100)
		{
			if(p < (scale>>2))
			{
				y = 0 + x[1] + 0 + 0;
			}
			else if(p < (scale>>1))
			{
				y = -(x[0]>>2) + x[1] + (x[2]*3>>3) - (x[3]>>3);
			}
			else if(p < (scale*3>>2))
			{
				y = -(x[0]>>2) + (x[1]*3>>2) + (x[2]*3>>2) - (x[3]>>2);
			}
			else
			{
				y = -(x[0]>>3) + (x[1]*3>>3) + x[2] - (x[3]>>2);
			}
			
			if( y > 255 )
			{
				*dst++ = 255;
			}
			else if( y < 0 )
			{
				*dst++ = 0;
			}
			else
			{
				*dst++ = y;
			}
			
			i++;
			p = p + 100;
		}
		else if( p < scale )
		{
			if(p < (scale>>2))
			{
				y = 0 + x[1] + 0 + 0;
			}
			else if(p < (scale>>1))
			{
				y = -(x[0]>>2) + x[1] + (x[2]*3>>3) - (x[3]>>3);
			}
			else if(p < (scale*3>>2))
			{
				y = -(x[0]>>2) + (x[1]*3>>2) + (x[2]*3>>2) - (x[3]>>2);
			}
			else
			{
				y = -(x[0]>>3) + (x[1]*3>>3) + x[2] - (x[3]>>2);
			}
			
			if( y > 255 )
			{
				*dst++ = 255;
			}
			else if( y < 0 )
			{
				*dst++ = 0;
			}
			else
			{
				*dst++ = y;
			}
			
			i++;
			x[0] = x[1];
			x[1] = x[2];
			x[2] = x[3];
			x[3] = *src++;
			j++;
			p = p + 100 - scale;		
		}
		else if( p >= scale )
		{
			x[0] = x[1];
			x[1] = x[2];
			x[2] = x[3];
			x[3] = *src++;
			j++;
			p = p - scale;
		}
	}
	
	while( i < d_len )
	{
		*dst++ = 255;
		i++;
	}
}

uint8_t * bilinear(uint8_t *img, int newWidth, int newHeight, int width, int height)
{

    uint8_t * img2 =0;
    img2 = (uint8_t *)malloc(newWidth*newHeight);
    uint8_t * Data = img2;
    uint8_t * data = img;
#if 1
    int x,y;
    float tx = (float)(width-1)/newWidth;
    float ty = (float)(height-1)/newHeight;
    float x_diff, y_diff;
    int i,j;
    int value = 0;
    const int stride = width;
	int count = 0;

	for(i=0; i<newHeight; i++)
	{
		y_diff = (ty * i);
		y = (int)y_diff;
		float fy = y_diff - y;
		float fy1 = 1.0f - fy;
		
		for(j=0; j<newWidth; j++)
		{
			x_diff = (tx * j);
			x = (int)x_diff;

			const uint8_t* p0 = &data[x + y*stride];
			const uint8_t p1 = p0[0+0*stride];
			const uint8_t p2 = p0[1+0*stride];
			const uint8_t p3 = p0[0+1*stride];
			const uint8_t p4 = p0[1+1*stride];

			float fx = x_diff - x;
			float fx1 = 1.0f - fx;

			int w1 = fx1 * fy1 * 256.0f;
			int w2 = fx  * fy1 * 256.0f;
			int w3 = fx1 * fy * 256.0f;
			int w4 = fx  * fy * 256.0f;

			value = p1*w1 + p2*w2 + p3*w3 + p4*w4;

			Data[count++] = (value >> 8);
		}
	}
#else
	int i = 0;
	char *di, *si;
	int scale = 80;
	for(i = 0; i < newHeight; i++)
	{
		si = data + width*i;
		di = Data + newWidth*i;
		conv(scale, width, si, di);
	}
#endif 
	printf("\n(YHCHO) %s() OK", __FUNCTION__);

	return img2;
}

#if __TX_REPORT_IMAGE_PRINT__
uint8_t * bilinear_for_report_image(uint8_t *img, int newWidth, int newHeight, int width, int height)
{

    uint8_t * img2 =0;
    img2 = (uint8_t *)malloc(REPORT_IMAGE_MAX_LEN*REPORT_IMAGE_MAX_LEN);
    uint8_t * Data = img2;
    uint8_t * data = img;
#if 1
    int x,y;
    float tx = (float)(width-1)/newWidth;
    float ty = (float)(height-1)/newHeight;
    float x_diff, y_diff;
    int i,j;
    int value = 0;
    const int stride = width;
	int start_width = (REPORT_IMAGE_MAX_LEN-newWidth)>>1;
	int start_height = 0;
	
	memset(img2, 255, REPORT_IMAGE_MAX_LEN*REPORT_IMAGE_MAX_LEN);

	for(i=0; i<newHeight; i++)
	{
		y_diff = (ty * i);
		y = (int)y_diff;
		float fy = y_diff - y;
		float fy1 = 1.0f - fy;
		start_height = (i*REPORT_IMAGE_MAX_LEN);

		for(j=0; j<newWidth; j++)
		{
			x_diff = (tx * j);
			x = (int)x_diff;

			const uint8_t* p0 = &data[x + y*stride];
			const uint8_t p1 = p0[0+0*stride];
			const uint8_t p2 = p0[1+0*stride];
			const uint8_t p3 = p0[0+1*stride];
			const uint8_t p4 = p0[1+1*stride];

			float fx = x_diff - x;
			float fx1 = 1.0f - fx;

			int w1 = fx1 * fy1 * 256.0f;
			int w2 = fx  * fy1 * 256.0f;
			int w3 = fx1 * fy * 256.0f;
			int w4 = fx  * fy * 256.0f;

			value = p1*w1 + p2*w2 + p3*w3 + p4*w4;

			//Data[count++] = (value >> 8);
			Data[ start_height + start_width + j ] = (value >> 8);
		}
	}
#else
	int i = 0;
	char *di, *si;
	int scale = 80;
	for(i = 0; i < newHeight; i++)
	{
		si = data + width*i;
		di = Data + newWidth*i;
		conv(scale, width, si, di);
	}
#endif 
	printf("\n(YHCHO) %s() OK", __FUNCTION__);

	return img2;
}
#endif

uint8_t* reduce_mipmap(uint8_t *_data, int newWidth, int newHeight, int _width, int _height)
{
	int cy, cx;
    uint8_t *newData = (uint8_t *)NULL;

    newData = MEM_MALLOC(newWidth * newHeight);

    if(newData == (uint8_t *)NULL)
	{
        XASSERT(0,0);

        return NULL;
	}
    else
    {
        double scaleWidth =  (double)newWidth / (double)_width;
        double scaleHeight = (double)newHeight / (double)_height;

        for(cy = 0; cy < newHeight; cy++)
        {
            for(cx = 0; cx < newWidth; cx++)
            {
                int pixel = (cy * newWidth) + (cx);
                int nearestMatch =  (((int)(cy / scaleHeight) * _width) + (int)(cx / scaleWidth) );

                newData[pixel] =  _data[nearestMatch];
            }
        }
    }

	return newData;
}

void SaveImage (int CurrentPage, uint8_t *buffer, int wid, int hig, int m_resolution, UINT16 jnum, int fax_mode)
{
	uint32_t mem_req;
	uint8_t *mem;
	int area = 0;
    void *handle = (void *)NULL;
	FILE *fhandle = (FILE *)NULL;
    char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT8 TotalRecipient = 1;
    UINT8 IndexToRecipient = 0;
    UINT16 doc_id = fsu_get_doc_id(jnum);
    UINT16 OriginalDocId = 0;
    
    if(fax_mode == REQUEST_SOURCE_FAX)
	{
		TotalRecipient = faxapp_get_no_of_recipient();
	}
    else
	{
		TotalRecipient = PCFax_get_no_of_recipient();
    }

	snprintf(PBMFileName, sizeof(PBMFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, doc_id, PATH_SEPARATOR, CurrentPage);
	printf("\n(YHCHO) %s() - PBMFileName: %s\n", __FUNCTION__, PBMFileName);

	fhandle = fopen(PBMFileName, "wb");
	if(fhandle == (FILE *)NULL)
	{
		printf(">>file open error\n");
        XASSERT(0,0);

        return;
	}

    if (CurrentPage == 1)
    {
        if ( (TotalRecipient == 0) || (TotalRecipient == 1) )
        {
            fsu_update_image_width_pixel(jnum, (UINT16)wid);

            fsu_update_image_height_pixel(jnum, (UINT16)hig);
        }
        else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
        {
            OriginalDocId = fsu_get_original_doc_id(jnum);

            for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
            {
                fsu_update_image_width_pixel(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], (UINT16)wid);

                fsu_update_image_height_pixel(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], (UINT16)hig);
            }
        }
    }
    //fax_image_dump(wid, hig, buffer, 1);//0: 1bit 1:8bit
    //otsu algorithm
    if(fsu_get_photo_mode(jnum) == 0) //text mode
    {
		txImageThreshold = otsuThreshold(buffer, hig, wid);
	}

    /* 8bit -> 1bit */
	mem_req = gray2bw_memreq(wid);
	mem = MEM_MALLOC(mem_req);
	handle = gray2bw_create(wid, (uint32_t*)mem, mem_req);

    if(handle == (void *)NULL)
	{
		printf("\n(YHCHO) %s()  gray2bw_create fail\n", __FUNCTION__);
		XASSERT(0,0);
	}

    gray2bw_convert(handle, buffer, hig, 0/*fax_mode*/, fsu_get_photo_mode(jnum));
	//fax_image_dump(wid, hig, buffer, 0);//0: 1bit 1:8bit
	if(!isDKSolution)
	{
		fprintf(fhandle,"P4\n%d %d\n",wid,hig);
	}

	printf("\n(YHCHO) %s()  wid : %d, hig = %d\n", __FUNCTION__,wid, hig);

    fsu_update_width(jnum, fsu_get_width_info(m_resolution, wid));

	area = wid * hig;
    if(fwrite(buffer, sizeof(uint8_t), area >> 3, fhandle) != (area >> 3))
	{
		printf(">> pbm fwrite fail\n");
		XASSERT(0,0);
    }

    if (fflush(fhandle) != 0)
    {
        XASSERT(0,0);
    }

    if (fsync(fhandle->_fileno) == -1)
    {
        XASSERT(0,0);
    }

    if (fclose(fhandle) != 0)
    {
        XASSERT(0,0);
    }
	
	//MEM_FREE_AND_NULL(buffer);
	MEM_FREE_AND_NULL(mem);

	if(!isDKSolution)
	{
#if __TX_REPORT_IMAGE_PRINT__
		if(getgFaxTxResultRptImage() == ENABLE)
		{
			if (fsu_get_flag_manual_tx(jnum) == 1 && CurrentPage == 1 && m_resolution == 3)
			{
				FILE *RptPBM = (FILE *)NULL;
				char RptPBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
				snprintf(RptPBMFileName, sizeof(RptPBMFileName), "/tmp/faxTx.pbm");

				RptPBM = fopen(RptPBMFileName, "wb");
				if(RptPBM == (FILE *)NULL)
				{
					printf(">>(%s) file open error\n", RptPBMFileName);
				}
				else
				{
					fprintf(RptPBM, "P4\n%d %d\n", wid, hig);

					if(fwrite(buffer, 1, (wid*hig) >> 3, RptPBM) != (wid*hig) >> 3)
					{
						printf("%s() (%s) write FAil !!\n",__FUNCTION__, RptPBMFileName);
					}

					fclose(RptPBM);
				}
			}
		}
#endif

		/* pbm -> jbg */
		T4_encoding(CurrentPage, PBMFileName, CODING_MMR, m_resolution, jnum, FAX_APP_JOB_DIRECTION_SCAN, m_resolution);
	}
}


/* fax scan raw -> pbm -> jbg */
void MakeRawToPbm (int CurrentPage, uint8_t *buffer, int wid, int hig, UINT16 jnum, int fax_mode)
{
#if __HAVE_FAXSOLUTION_DK__
#endif
    uint8_t *fine_buffer = (uint8_t *)NULL;
	int fine_wid, fine_hig;
    int image_type = fsu_get_photo_mode(jnum);

	if(isDKSolution)
	{
		SaveImage (CurrentPage, buffer, wid, hig, 3, jnum, fax_mode);
	}
	else
	{
		if(image_type)
		{
			fine_wid = (wid / 1.5);
			fine_hig = (hig / 1.5);
			fine_buffer = bilinear(buffer, fine_wid, fine_hig, wid, hig);
			if(fine_buffer == NULL)
			{
				printf("\n(YHCHO) %s() Failure handling is required.\n", __FUNCTION__);
				ASSERT(0);
			}
			SaveImage (CurrentPage, fine_buffer, fine_wid, fine_hig, 1, jnum, fax_mode);
			SaveImage (CurrentPage, buffer, wid, hig, 3, jnum, fax_mode);
			
			MEM_FREE_AND_NULL(fine_buffer);
		}
		else
		{
			SaveImage (CurrentPage, buffer, wid, hig, 3, jnum, fax_mode);
		}
	}
}

int PBM_Sub_Rescaling(char *PBMFileName, int target_width, int target_height, int current_width, int current_height, uint16_t jnum)
{
	FILE *fhandle = (FILE *)NULL;	
	uint8_t *tmp_buffer = (uint8_t *)NULL;
	uint8_t *target_data = (uint8_t *)NULL;
	uint8_t *buffer = (uint8_t *)NULL;
	int c, size, i, j, Index;
	char type, tempchar;
    int area = 0;

	fhandle = fopen(PBMFileName, "rb");
	if(fhandle == (FILE *)NULL)
	{
		printf("\n(YHCHO) %s() file open error\n",__FUNCTION__);
		XASSERT(0,0);
	}
	
	printf("\n(YHCHO) %s() - wid: %d, hig: %d\n", __FUNCTION__, current_width,current_height);

	area = current_width * current_height;
	tmp_buffer = (uint8_t *)MEM_MALLOC(area >> 3);
	if(tmp_buffer == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() MEM_MALLOC fail\n", __FUNCTION__);
		XASSERT(0,0);
	}

	/* read PBM header */
	while ((c = getc(fhandle)) != EOF && (isspace(c) || c == '#'))
		if (c == '#')
		  while ((c = getc(fhandle)) != EOF && !(c == 13 || c == 10)) ;

	type = (char)getc(fhandle);
	if (c != 'P' || (type != '1' && type != '4')) {
		fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", PBMFileName);
		return 1;
	}
	getint(fhandle); // current_width
	getint(fhandle); // current_height
	fgetc(fhandle);    /* skip line feed */  

	size = fread(tmp_buffer, area >> 3, 1, fhandle);
	if(size < 0)
	{
		printf("\n(YHCHO) %s() read fail\n", __FUNCTION__);
        XASSERT(0,0);
	}

	buffer = (uint8_t *)MEM_MALLOC(area);
	if(buffer == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() MEM_MALLOC fail\n", __FUNCTION__);
		XASSERT(0,0);
	}

	// Convert 1-bit to 8-bit
	j = 0;
	Index = area >> 3;

	for(i = 0; i < Index; )
	{
		tempchar = tmp_buffer[i++];

		buffer[j++]=(tempchar&0x80)? 255 : 0;
		buffer[j++]=(tempchar&0x40)? 255 : 0;
		buffer[j++]=(tempchar&0x20)? 255 : 0;
		buffer[j++]=(tempchar&0x10)? 255 : 0;
		buffer[j++]=(tempchar&0x08)? 255 : 0;
		buffer[j++]=(tempchar&0x04)? 255 : 0;
		buffer[j++]=(tempchar&0x02)? 255 : 0;
		buffer[j++]=(tempchar&0x01)? 255 : 0;
    }
    
    MEM_FREE_AND_NULL(tmp_buffer);
 
	target_data = bilinear(buffer,target_width,target_height,current_width,current_height);
	if(target_data == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() scaling fail\n", __FUNCTION__);

		XASSERT(0,0);
	}
	
	MEM_FREE_AND_NULL(buffer);	
#if 0
    //otsu algorithm
    if(fsu_get_photo_mode(jnum) == 0) //text mode
    {
		txImageThreshold = otsuThreshold(target_data, target_height, target_width);
	}
#endif
	area = target_width * target_height;

	tmp_buffer = (uint8_t *)MEM_MALLOC(area >> 3);
	if(tmp_buffer == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() MEM_MALLOC fail\n", __FUNCTION__);
		XASSERT(0,0);
	}
	//8bit to 1bit
	j = 0;
	Index = area;
	for(i = 0; i < Index; )
	{
		tempchar = 0;

		if (target_data[i++] > 0) tempchar |= 0x80;
		if (target_data[i++] > 0) tempchar |= 0x40;
		if (target_data[i++] > 0) tempchar |= 0x20;
		if (target_data[i++] > 0) tempchar |= 0x10;
		if (target_data[i++] > 0) tempchar |= 0x08;
		if (target_data[i++] > 0) tempchar |= 0x04;
		if (target_data[i++] > 0) tempchar |= 0x02;
		if (target_data[i++] > 0) tempchar |= 0x01;

		tmp_buffer[j++] = tempchar;
	}
	
	printf("\n(YHCHO) %s()  = %d, %d\n", __FUNCTION__,target_width,target_height);
	MEM_FREE_AND_NULL(target_data);	
	fclose(fhandle);

	unlink(PBMFileName);
	
	fhandle = fopen(PBMFileName, "wb");

	if(fhandle == (FILE *)NULL)
	{
		printf(">>file open error\n");
		XASSERT(0,0);
	}

	area = target_width * target_height;

	fprintf(fhandle,"P4\n%d %d\n",target_width,target_height);
	if(fwrite(tmp_buffer, sizeof(uint8_t), (area >> 3), fhandle) != (area >> 3))
	{
		printf(">> pbm fwrite fail\n");
		XASSERT(0,0);
	}
	
	MEM_FREE_AND_NULL(tmp_buffer);
    
	if (fflush(fhandle) != 0)
	{
		XASSERT(0,0);
	}

	if (fsync(fhandle->_fileno) == -1)
	{
		XASSERT(0,0);
	}

	if (fclose(fhandle) != 0)
	{
		XASSERT(0,0);
	}

	return 1;

}

void PBM_Rescaling (char *PBMFileName, uint8_t faxRes, uint16_t jnum, uint8_t image_width_type)
{
	int TgtWidth = 0, TgtHeight = 0, wid = fsu_get_image_width_pixel(jnum), hig = fsu_get_image_height_pixel(jnum);
	int tempWidth = 0, tempHeight = 0;
	uint8_t current_res = 3;
	BOOLEAN isTTIOn = FALSE;
	uint8_t current_job_image_width_type = 0;

	printf("\n(YHCHO) %s() - PBMFileName: %s\n", __FUNCTION__, PBMFileName);
	
    if(fsu_get_photo_mode(jnum) == 1)
	{
		printf("(YHCHO) %s() - photo mode no rescaling\n", __FUNCTION__);
		//TTI
		isTTIOn = fsu_get_tti();
		if (isTTIOn == TRUE)
		{
			fsu_gen_fax_header(PBMFileName, faxRes, image_width_type);
		}
		return;	
	}
	
	if (fsu_get_fax_forward(jnum) == 1)
	{
        current_res = fsu_get_resolution(jnum);
	}
	else
	{
		current_res = 3;
	}
	
	if(current_res == 3)
	{
		if(wid == 2592) //A4
		{
			current_job_image_width_type = 0;
		}
		else if(wid == 3072) //B4
		{
			current_job_image_width_type = 1;
		}
		else if(wid == 3648) //A3
		{
			current_job_image_width_type = 2;
		}
		else
		{
			faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_1);
		}
	}
	else
	{
		current_job_image_width_type = 0;
	}

	printf("\n(YHCHO) %s() - image_width_type = %d, current_job_image_width_type = %d\n", __FUNCTION__, image_width_type, current_job_image_width_type);
	
	if(current_res == faxRes)
	{
		if(current_job_image_width_type != image_width_type)//Need Rescaling
		{
			if(current_job_image_width_type == 2 && image_width_type == 0)// A3 -> A4
			{
				TgtWidth = 2592;
			}
			else if(current_job_image_width_type == 2 && image_width_type == 1)// A3 -> B4
			{
				TgtWidth = 3072;
			}
			else if(current_job_image_width_type == 1 && image_width_type == 0)// B4 -> A4
			{
				TgtWidth = 2592;
			}
			else
			{
				ASSERT(0);
			}
			
			TgtHeight = (hig * TgtWidth)/wid;

			if(!PBM_Sub_Rescaling(PBMFileName, TgtWidth, TgtHeight, wid, hig, jnum))
			{
				faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_2);
			}
		    
			fsu_update_image_width_pixel(jnum, TgtWidth);
			fsu_update_image_height_pixel(jnum, TgtHeight);
		}		
	}
	else
	{
		if(current_job_image_width_type != image_width_type)//type & resolution no match : rescaling
		{
			if(current_res == 3)
			{
				if(faxRes == 0)
				{
					tempWidth = wid / (1.5);
					tempHeight = hig / 3;

					if(current_job_image_width_type == 2 && image_width_type == 0)// A3 -> A4
					{
						TgtWidth = 1728;
					}
					else if(current_job_image_width_type == 2 && image_width_type == 1)// A3 -> B4
					{
						TgtWidth = 2048;
					}
					else if(current_job_image_width_type == 1 && image_width_type == 0)// B4 -> A4
					{
						TgtWidth = 1728;
					}

					TgtHeight = (tempHeight * TgtWidth)/tempWidth;
				}
				else if(faxRes == 1)
				{
					tempWidth = wid / (1.5);
					tempHeight = hig / (1.5);
					
					if(current_job_image_width_type == 2 && image_width_type == 0)// A3 -> A4
					{
						TgtWidth = 1728;
					}
					else if(current_job_image_width_type == 2 && image_width_type == 1)// A3 -> B4
					{
						TgtWidth = 2048;
					}
					else if(current_job_image_width_type == 1 && image_width_type == 0)// B4 -> A4
					{
						TgtWidth = 1728;
                    }

					TgtHeight = (tempHeight * TgtWidth)/tempWidth;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(current_res == 1)
			{
				if (faxRes == 0)
				{
					tempWidth = wid;
                    tempHeight = hig >> 1;

					TgtHeight = (tempHeight * TgtWidth)/tempWidth;
				}
				else
				{
                    faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_3);
				}
			}
			else
			{
				ASSERT(0);
			}			
		}
		else // resolution no match : rescaling
		{
			if(current_res == 3)
			{
				if (faxRes == 0)
				{
					TgtWidth = wid / (1.5);
					TgtHeight = hig / 3;
				}
				else
				{
					TgtWidth = wid / (1.5);
					TgtHeight = hig / (1.5);
				}
			}
			else if(current_res == 1)
			{
				if (faxRes == 0)
				{
					TgtWidth = wid;
                    TgtHeight = hig >> 1;
				}
				else
				{
					faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_4);
				}
			}
			else
			{
				ASSERT(0);
			}
		}

		if(!PBM_Sub_Rescaling(PBMFileName, TgtWidth, TgtHeight, wid, hig, jnum))
		{
			faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_5);
		}

		fsu_update_image_width_pixel(jnum, TgtWidth);
		fsu_update_image_height_pixel(jnum, TgtHeight);
	}

	//TTI
    isTTIOn = fsu_get_tti();
    if (isTTIOn == TRUE && fsu_get_fax_forward(jnum) != 1)
    {
        fsu_gen_fax_header(PBMFileName, faxRes, image_width_type);
    }
}


void fax_rotate(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int margin)
{
	int i,j;
	
	for(i =0; i<strip_height; i++)
	{
		for(j=0; j<strip_width; j++)
		{
			*(pD + (margin+i) + j*type_width) = *(pS + (strip_width-j-1) + (i*strip_width));
		}
	}
}

void fax_rotate_for_print(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int strip_real_width, int margin)
{
	int i,j;
	
	for(i =0; i<strip_height; i++)
	{
		for(j=0; j<strip_width; j++)
		{
			*(pD + i + j*type_width) = *(pS + (strip_real_width-j-1-margin) + (i*strip_real_width));
		}
	}
}

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS	0
#endif
#ifndef EXIT_FAILURE
# define EXIT_FAILURE	1
#endif

#define TIFFhowmany8(x) (((x)&0x07)?((uint32)(x)>>3)+1:(uint32)(x)>>3)

int	copyDecodeFaxFile(TIFF* tifin, TIFF* tifout, char *outFileName, uint32 x_size, uint8_t *refbuffer, uint8_t *rowbuffer, uint16 *badruncheck);
#if __MR_TX_ERR_WORKAROUND__
int	copyEncodeFaxFile(FILE* rawin, TIFF* tifout, uint32 x_size, char *rowbuffer, uint32 Src_Height, int type);
#else /* __MR_TX_ERR_WORKAROUND__ */
int	copyEncodeFaxFile(FILE* rawin, TIFF* tifout, uint32 x_size, char *rowbuffer, uint32 Src_Height);
#endif /* __MR_TX_ERR_WORKAROUND__ */

int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction)
{
	uint8_t	*rowbuf = NULL;
	uint8_t	*refbuf = NULL;
	uint32	xsize = 0;
	uint32  rx_res_check = 0;
	uint16 badrun = 0;

	FILE *in;
	TIFF *out = NULL;
	TIFF	*faxTIFF = NULL;
	int compression_in = COMPRESSION_CCITTFAX4; //COMPRESSION_CCITTFAX3,COMPRESSION_CCITTFAX4
	int compression_out = COMPRESSION_CCITTFAX3; //COMPRESSION_CCITTFAX3,COMPRESSION_CCITTFAX4
	int fillorder_in = FILLORDER_LSB2MSB;
	int fillorder_out = FILLORDER_LSB2MSB;
	uint32 group3options_in = 0;	/* 1d-encoded */ // &= ~GROUP3OPT_2DENCODING, |= GROUP3OPT_2DENCODING
	uint32 group3options_out = 0;	/* 1d-encoded */
	uint32 group4options_in = 0;	/* compressed */
	//uint32 group4options_out = 0;	/* compressed */
	//uint32 defrowsperstrip = (uint32) 0;
	uint32 rowsperstrip;
	int photometric_in = PHOTOMETRIC_MINISWHITE;
	int photometric_out = PHOTOMETRIC_MINISWHITE;
	int mode = FAXMODE_CLASSF;
	int rows;
	//int c, size = 0;
	float resY = 0.0;
	float resX = 0.0;
	extern int optind;
	extern char* optarg;
    char TmpFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    snprintf(TmpFileName, sizeof(TmpFileName), "%s%cDoc%04X%cfax%02X.tif", 
				pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, 0);
    

	printf("\n(YHCHO) %s() - coding %d \n", __FUNCTION__, codingtype);

	if(codingtype == CODING_MH)
	{
		compression_in = COMPRESSION_CCITTFAX3;
		group3options_in &= ~GROUP3OPT_2DENCODING;
	}
	else if(codingtype == CODING_MR)
	{
		compression_in = COMPRESSION_CCITTFAX3;
		group3options_in |= GROUP3OPT_2DENCODING;
	}
    else
	{
		compression_in = COMPRESSION_CCITTFAX4;
		group4options_in = 0;
	}

	if(width_type == 0) //A4 width
	{
		if(rx_resolution == 0 || rx_resolution == 1)
		{
			xsize = 1728;
		}
		else
		{
			xsize = 2592;
		}
	}
	else if(width_type == 1) //B4 width
	{
		if(rx_resolution == 0 || rx_resolution == 1)
		{
			xsize = 2048;
		}
		else
		{
			xsize = 3072;
		}		
	}
    else //A3 width
	{
		if(rx_resolution == 0 || rx_resolution == 1)
		{
			xsize = 2432;
		}
		else
		{
			xsize = 3648;
		}				
	}
	rx_res_check = rx_resolution;
	if(rx_resolution == 0)
	{
		resX = 200.0;
		resY = 100.0;
	}
	else if(rx_resolution == 1)
	{
		resX = 200.0;
		resY = 200.0;		
	}
	else
	{
		resX = 300.0;
		resY = 300.0;		
	}
	
	if(Direction == FAX_APP_JOB_DIRECTION_PRINT)
	{
		xsize = fsu_get_image_width_pixel(jnum);
	}
	
	rowbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	refbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	if (rowbuf == NULL || refbuf == NULL ) 
	{
		fprintf(stderr, "Not enough memory\n");
		_TIFFfree(rowbuf);
		_TIFFfree(refbuf);

		return (EXIT_FAILURE);
	}
	memset(rowbuf,0,TIFFhowmany8(xsize));
	memset(refbuf,0,TIFFhowmany8(xsize));

    out = TIFFOpen(TmpFileName, "w");
	if (out == NULL) {
        fprintf(stderr, "Can not create %s\n", TmpFileName);
        _TIFFfree(rowbuf);
        _TIFFfree(refbuf);
        
		return (EXIT_FAILURE);
	}
		
	faxTIFF = TIFFClientOpen("(FakeInput)", "w",
	/* TIFFClientOpen() fails if we don't set existing value here */
				 TIFFClientdata(out),
				 TIFFGetReadProc(out), TIFFGetWriteProc(out),
				 TIFFGetSeekProc(out), TIFFGetCloseProc(out),
				 TIFFGetSizeProc(out), TIFFGetMapFileProc(out),
				 TIFFGetUnmapFileProc(out));
	if (faxTIFF == NULL) 
	{
		fprintf(stderr, "Can not create fake input file\n");
		_TIFFfree(rowbuf);
		_TIFFfree(refbuf);
		
		return (EXIT_FAILURE);
	}
	TIFFSetMode(faxTIFF, O_RDONLY);
	TIFFSetField(faxTIFF, TIFFTAG_IMAGEWIDTH,	xsize);
	TIFFSetField(faxTIFF, TIFFTAG_SAMPLESPERPIXEL,	1);
	TIFFSetField(faxTIFF, TIFFTAG_BITSPERSAMPLE,	1);
	TIFFSetField(faxTIFF, TIFFTAG_FILLORDER,	fillorder_in);
	TIFFSetField(faxTIFF, TIFFTAG_PLANARCONFIG,	PLANARCONFIG_CONTIG);
	TIFFSetField(faxTIFF, TIFFTAG_PHOTOMETRIC,	photometric_in);
	TIFFSetField(faxTIFF, TIFFTAG_YRESOLUTION,	resY);
	TIFFSetField(faxTIFF, TIFFTAG_RESOLUTIONUNIT,	RESUNIT_INCH);
	
	/* NB: this must be done after directory info is setup */
	TIFFSetField(faxTIFF, TIFFTAG_COMPRESSION, compression_in);
	if (compression_in == COMPRESSION_CCITTFAX3)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP3OPTIONS, group3options_in);
	else if (compression_in == COMPRESSION_CCITTFAX4)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP4OPTIONS, group4options_in);

	in = fopen(FileName, "rb");
	if (in == NULL) {
		fprintf(stderr,"%s: Can not open\n",FileName);
		_TIFFfree(rowbuf);	
		_TIFFfree(refbuf);

		return (EXIT_FAILURE);
	}
	
	TIFFSetClientdata(faxTIFF, (thandle_t)fileno(in));

	TIFFSetFileName(faxTIFF, FileName);
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, xsize);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 1);
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression_out);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric_out);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
	switch (compression_out) {
		/* g3 */
		case COMPRESSION_CCITTFAX3:
		TIFFSetField(out, TIFFTAG_GROUP3OPTIONS,
				 group3options_out);
		TIFFSetField(out, TIFFTAG_FAXMODE, mode);
		rowsperstrip = TIFFDefaultStripSize(out, 0);
		break;
#if 0
		/* g4 */
		case COMPRESSION_CCITTFAX4:
		TIFFSetField(out, TIFFTAG_GROUP4OPTIONS,
				 group4options_out);
		TIFFSetField(out, TIFFTAG_FAXMODE, mode);
		rowsperstrip = (uint32)-1L;
		break;

		default:
		rowsperstrip = (defrowsperstrip) ?
			defrowsperstrip : TIFFDefaultStripSize(out, 0);
#endif
	}
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_FILLORDER, fillorder_out);
	//TIFFSetField(out, TIFFTAG_SOFTWARE, "fax2tiff");
	TIFFSetField(out, TIFFTAG_XRESOLUTION, resX); //204.0 ??
	TIFFSetField(out, TIFFTAG_YRESOLUTION, resY); // 196. ??
	TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	//TIFFSetField(out, TIFFTAG_PAGENUMBER, pn, npages);

	printf("\n(YHCHO) %s() - Decoding start : %d, %d\n", __FUNCTION__,rx_res_check,xsize);

	rows = copyDecodeFaxFile(faxTIFF, out, out_FileName, xsize, refbuf, rowbuf, &badrun);

	printf("\n(YHCHO) %s() - Decoding end\n", __FUNCTION__);

	fclose(in);

	TIFFSetField(out, TIFFTAG_IMAGELENGTH, rows);
#if 0
	if (compression_out == COMPRESSION_CCITTFAX3 &&
		mode == FAXMODE_CLASSF) {
		TIFFSetField(out, TIFFTAG_BADFAXLINES, badfaxlines);
		TIFFSetField(out, TIFFTAG_CLEANFAXDATA, badfaxlines ?
			CLEANFAXDATA_REGENERATED : CLEANFAXDATA_CLEAN);
		TIFFSetField(out, TIFFTAG_CONSECUTIVEBADFAXLINES, badfaxrun);
	}
	TIFFWriteDirectory(out);
#endif	
	TIFFClose(out);
	
    fsu_update_image_width_pixel(jnum, xsize);
    fsu_update_image_height_pixel(jnum, rows);
	
    fsu_delete_file_on_file_system(TmpFileName);
	
	_TIFFfree(rowbuf);
	_TIFFfree(refbuf);
	
	if(badrun == 10) //fail decoding
	{
		unlink(out_FileName);
		return (EXIT_FAILURE);
	}

    if ( (Direction == FAX_APP_JOB_DIRECTION_PRINT) && (fsu_get_print_reason(jnum) == PRINT_REASON_RESET) )
    {
        is1stPageDataTransfered = 1;
    }	
	
	return (EXIT_SUCCESS);
}

#define ERROR_LINE 5
int copyDecodeFaxFile(TIFF* tifin, TIFF* tifout, char *outFileName, uint32 x_size, uint8_t *refbuffer, uint8_t *rowbuffer, uint16 *badruncheck)
{
	uint32 row;
	int linesize = TIFFhowmany8(x_size);
	int ok;
	FILE *outpbm;
	int datasize = 0;
	int total_error_line = 0;
	int tempHeight = 3432;
	uint16 goodrun;
	uint16_t error_index = 0;
	uint8_t *errorBuffer = MEM_MALLOC(ERROR_LINE*linesize);

	outpbm = fopen(outFileName, "w");
	if (outpbm == NULL) 
	{
		fprintf(stderr, "%s: Can not open\n",outFileName);
		return (EXIT_FAILURE);
	}
	fprintf(outpbm,"P4\n%d %d\n",x_size,tempHeight);

	tifin->tif_rawdatasize = (tmsize_t)TIFFGetFileSize(tifin);
	datasize = tifin->tif_rawdatasize;
	printf("\n(YHCHO) %s() start 1 : %d\n", __FUNCTION__,(int) tifin->tif_rawdatasize);
	tifin->tif_rawdata = _TIFFmalloc(tifin->tif_rawdatasize);

	if (tifin->tif_rawdata == NULL) 
	{
		TIFFError(tifin->tif_name, "Not enough memory");
		fclose(outpbm);
		return (0);
	}
	if (!ReadOK(tifin, tifin->tif_rawdata, tifin->tif_rawdatasize)) 
	{
		TIFFError(tifin->tif_name, "Read error at scanline 0");
		fclose(outpbm);
		return (0);
	}
	
	tifin->tif_rawcp = tifin->tif_rawdata;
	tifin->tif_rawcc = tifin->tif_rawdatasize;

	(*tifin->tif_setupdecode)(tifin);
	(*tifin->tif_predecode)(tifin, (tsample_t) 0);

	tifin->tif_row = 0;

	printf("\n(YHCHO) %s() start 7\n", __FUNCTION__);

	_TIFFmemset(refbuffer, 0, linesize);
	row = 0;
	*badruncheck = 0;		/* current run of bad lines */
	goodrun = 0;
	
	while (tifin->tif_rawcc > 0) 
	{
		ok = (*tifin->tif_decoderow)(tifin, (tdata_t) rowbuffer, linesize, 0);
		if (ok == -1) //error 
		{
			printf("\n(YHCHO) %s() !ok (%d)(%d)\n", __FUNCTION__,(int)tifin->tif_rawcc, tifin->tif_row);
			*badruncheck += 1;
			total_error_line++;

			//memset(rowbuffer,0x00,linesize);
			error_index = 1;
			
			if(((tifin->tif_rawcc < datasize*0.05) && *badruncheck > 3) || (*badruncheck == 10))
				break;
		}
		else
		{
			if(error_index > 0 && (error_index < (ERROR_LINE + 2)))
			{
				if(error_index > 1)
				{
					memcpy(&errorBuffer[(error_index-2)*linesize], rowbuffer, linesize);
				}

				error_index++;

				continue;
			}
			else if(error_index >= (ERROR_LINE + 2))
			{
				//write
				if(!(fwrite(&errorBuffer[0],linesize,ERROR_LINE,outpbm)))
				{
					fprintf(stderr, "%s: Write error at row %ld.\n",
					   outFileName,(long) row);
					break;				
				}
				row += ERROR_LINE;

				error_index = 0;
			}
			else
			{
				error_index = 0;
			}
		}

		if (ok == 1) //End Of Line
		{
			printf("\n(YHCHO) %s() EOL ok (%d)\n", __FUNCTION__,(int)tifin->tif_rawcc);
			break;
		}
		else   // Line decode ok
		{	
			if(*badruncheck>0)
			{
				goodrun++;
				if(goodrun == 5)
				{
					*badruncheck -= 1;
					goodrun = 0;
				}
			}
			_TIFFmemcpy(refbuffer, rowbuffer, linesize);
		}
		tifin->tif_row++;

		if(ok != -1)
		{
			if(!(fwrite(rowbuffer,linesize,1,outpbm)))
			{
				fprintf(stderr, "%s: Write error at row %ld.\n",
				   outFileName,(long) row);
				break;				
			}
		    row++;
		}
        
        //printf("\n(YHCHO) %s() row = %d, tifin->tif_rawcc = %d, tifin->tif_rawcp = %d\n", __FUNCTION__,row,tifin->tif_rawcc,tifin->tif_rawcp);
	}
	_TIFFfree(tifin->tif_rawdata);
	
    if (fseek(outpbm,0,SEEK_SET) != 0)
    {
        ASSERT(0);
    }

	fprintf(outpbm,"P4\n%d %d\n",x_size,row);

	printf("\n(YHCHO) %s() row = %d\n", __FUNCTION__,row);
	
	printf("\n(YHCHO) %s() total_error_line = %d\n", __FUNCTION__,total_error_line);
	
	fclose(outpbm);
	
	MEM_FREE_AND_NULL(errorBuffer);
	
	return (row);
}

int T4_encoding(int Page, char *FileName, int codingtype, int faxRes, int job_num_send, UINT8 Direction, int photo_image_resolution)
{
	char	*rowbuf = NULL;
	uint32	xsize = 0;
	uint32 SrcHeight = 0;

	FILE *in = NULL;
	TIFF *out = NULL;
	FILE *outraw = NULL;
	int compression_out = COMPRESSION_CCITTFAX4;
	int fillorder_out = FILLORDER_LSB2MSB;
	uint32 group3options_out = 0;	/* 1d-encoded */
	uint32 group4options_out = 0;	/* compressed */
	//uint32 defrowsperstrip = (uint32) 0;
	uint32 rowsperstrip;
	int photometric_out = PHOTOMETRIC_MINISWHITE;
	int mode = FAXMODE_CLASSIC;
	int rows; 
	float resX = 0.0;
	float resY = 0.0;
	extern int optind;
	extern char* optarg;

	char *buf;
	tstrip_t strip;
	uint32* bc;
	uint32 stripsize;
	int c;
	char type;
	
    char JBIGFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char TempFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

    isEncodeComplete = 0;

    if ( (Direction == FAX_APP_JOB_DIRECTION_SCAN) || (Direction == FAX_APP_JOB_DIRECTION_RECEIVE) )
    {
		if(photo_image_resolution == FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI)
		{
			snprintf(JBIGFileName, sizeof(JBIGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderARIA, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
		}
		else //FAX_APP_PHOTO_MODE_IMAGE_RES_FINE
		{
			snprintf(JBIGFileName, sizeof(JBIGFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderARIA, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
		}
		snprintf(TempFileName, sizeof(TempFileName), "%s%cDoc%04X%cfax%02X.tif", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
	}
	else
	{
		snprintf(JBIGFileName, sizeof(JBIGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
		snprintf(TempFileName, sizeof(TempFileName), "%s%cDoc%04X%cfax%02X.tif", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);

		//xsize = fsu_get_image_width_pixel(job_num_send);
		//SrcHeight = fsu_get_image_height_pixel(job_num_send);
	}
	
	if(faxRes == FAX_VR_STANDARD)
	{
		resX = 204.0;
		resY = 98.0;
	}
	else if(faxRes == FAX_VR_FINE)
	{
		resX = 204.0;
		resY = 196.0;		
	}
	else if(faxRes ==FAX_VR_300)
	{
		resX = 300.0;
		resY = 300.0;		
	}
	
	if(codingtype == CODING_MH)
	{
		compression_out = COMPRESSION_CCITTFAX3;
		group3options_out &= ~GROUP3OPT_2DENCODING;
		mode = FAXMODE_CLASSIC;
	}
	else if(codingtype == CODING_MR)
	{
		compression_out = COMPRESSION_CCITTFAX3;
		group3options_out |= GROUP3OPT_2DENCODING;
		mode = FAXMODE_CLASSIC;
	}
	else if(codingtype == CODING_MMR)
	{
		compression_out = COMPRESSION_CCITTFAX4;
		group4options_out = 0;
		mode = FAXMODE_CLASSF;
	}
	else
	{
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_T4_ENCODING_ERROR);
	}

	if(Direction == FAX_APP_JOB_DIRECTION_TRANSMIT)
	{
		T30Query(IOCTL_MIN_BYTES_PER_LINE, &minBytePerLine, sizeof(UINT8));
		printf("\n(YHCHO) %s() - minBytePerLine : %d\n", __FUNCTION__, minBytePerLine);
		if(minBytePerLine != 0)
		{
			group3options_out |= GROUP3OPT_FILLBITS;
		}
	}

	in = fopen(FileName,"r");
	if(in == NULL)
	{
		fprintf(stderr,"%s: Can not open\n",FileName);
		return (EXIT_FAILURE);
	}

	/* read PBM header */
	while ((c = getc(in)) != EOF && (isspace(c) || c == '#'))
		if (c == '#')
			while ((c = getc(in)) != EOF && !(c == 13 || c == 10)) ;
	type = (char)getc(in);
	if (c != 'P' || (type != '1' && type != '4')) {
		fprintf(stderr, "T4_encoding : Input file does not look like a PBM file!\n");
		fclose(in);

		return (EXIT_FAILURE);
	}
	xsize = getint(in); // wid
	SrcHeight = getint(in); // hig
	fgetc(in);    /* skip line feed */
	if(c == EOF)
	{
		fprintf(stderr, "Input file '%s' is Empty or Read Fail (skip line feed detect fail)\n", FileName);
		fclose(in);

		return (EXIT_FAILURE);
	}

	printf("\n(YHCHO) %s() xsize = %d, res = %d, codingType = %d\n", __FUNCTION__,xsize,faxRes,codingtype);

	outraw = fopen(JBIGFileName,"w");
	if(outraw == NULL)
	{
		fprintf(stderr,"%s: Can not open\n",JBIGFileName);
		fclose(in);
		return (EXIT_FAILURE);
	}
	
	out = TIFFOpen(TempFileName, "w");
	if (out == NULL) 
	{
		fprintf(stderr, "Can not create %s\n",TempFileName);
		fclose(in);
		fclose(outraw);
		return (EXIT_FAILURE);
	}

	rowbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	if (rowbuf == NULL) 
	{
		fprintf(stderr, "rowbuf Not enough memory\n");
		ASSERT(0);
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, xsize);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 1);
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression_out);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric_out);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);	

	switch (compression_out) 
	{
		/* g3 */
		case COMPRESSION_CCITTFAX3:
		TIFFSetField(out, TIFFTAG_GROUP3OPTIONS,
				 group3options_out);
		TIFFSetField(out, TIFFTAG_FAXMODE, mode);
		rowsperstrip = (uint32)-1L;
		break;

		/* g4 */
		case COMPRESSION_CCITTFAX4:
		TIFFSetField(out, TIFFTAG_GROUP4OPTIONS,
				 group4options_out);
		TIFFSetField(out, TIFFTAG_FAXMODE, mode);
		rowsperstrip = (uint32)-1L;
		break;
	}
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_FILLORDER, fillorder_out);
	//TIFFSetField(out, TIFFTAG_SOFTWARE, "fax2tiff");
	TIFFSetField(out, TIFFTAG_XRESOLUTION, resX);
	TIFFSetField(out, TIFFTAG_YRESOLUTION, resY);
	TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	//TIFFSetField(out, TIFFTAG_PAGENUMBER, pn, npages);

#if __MR_TX_ERR_WORKAROUND__
	rows = copyEncodeFaxFile(in, out, xsize, rowbuf, SrcHeight, codingtype);
#else /* __MR_TX_ERR_WORKAROUND__ */
	rows = copyEncodeFaxFile(in, out, xsize, rowbuf, SrcHeight);
#endif /* __MR_TX_ERR_WORKAROUND__ */
	fclose(in);
	unlink(FileName);
	
	//printf(">> copyFaxFile ok\n");

	TIFFSetField(out, TIFFTAG_IMAGELENGTH, rows);
	printf(">> out row = %d\n", rows);
	TIFFWriteDirectory(out);

	TIFFClose(out);

//re open tif.
	out = TIFFOpen(TempFileName, "r");
	if (out == NULL) 
	{
		fprintf(stderr, "Can not read encoded.tif\n");
		fclose(outraw);
		_TIFFfree(rowbuf);
		
		return (EXIT_FAILURE);
	}	

	TIFFGetField(out, TIFFTAG_STRIPBYTECOUNTS, &bc);
	stripsize = bc[0];
	buf = _TIFFmalloc(stripsize);
	if(buf == NULL)
	{
		fclose(outraw);
		TIFFClose(out);
		unlink(TempFileName);
		_TIFFfree(rowbuf);
		
		return (EXIT_FAILURE);		
	}
	
	for (strip = 0; strip < TIFFNumberOfStrips(out); strip++) 
	{
		if (bc[strip] > stripsize) 
		{
			buf = _TIFFrealloc(buf, bc[strip]);
			if(buf == NULL)
			{
				fclose(outraw);
				TIFFClose(out);
				unlink(TempFileName);
				_TIFFfree(rowbuf);
				
				return (EXIT_FAILURE);		
			}
			stripsize = bc[strip];
		}
		TIFFReadRawStrip(out, strip, buf, bc[strip]);
		
		if(!fwrite(buf,stripsize,1,outraw))
		{
			fprintf(stderr, "%s: Write error at strip %ld.\n", JBIGFileName,(long) strip);
			break;	
		}
	}

    if (fflush(outraw) != 0)
    {
        XASSERT(0,0);
    }

    if (fsync(outraw->_fileno) == -1)
    {
        XASSERT(0,0);
    }

    if (fclose(outraw) != 0)
    {
        XASSERT(0,0);
    }
		
	_TIFFfree(buf);
	TIFFClose(out);	
	_TIFFfree(rowbuf);
	unlink(TempFileName);

	if(isSDCardEnable)
	{
		if( (Direction == FAX_APP_JOB_DIRECTION_SCAN) || (Direction == FAX_APP_JOB_DIRECTION_RECEIVE) )
		{
			char ARIAFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
			if(photo_image_resolution == FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI)
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
			}
			else //FAX_APP_PHOTO_MODE_IMAGE_RES_FINE
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(job_num_send), PATH_SEPARATOR, Page);
			}
	#ifdef HAVE_IMAGELOG
			if(ARIA_encode_path(JBIGFileName, ARIAFileName, 0, 0) == -1)
			{
				return (EXIT_FAILURE);
			}
	#endif
		}
	}

	isEncodeComplete = 1;

	return (EXIT_SUCCESS);	
}

#if __MR_TX_ERR_WORKAROUND__
int copyEncodeFaxFile(FILE* rawin, TIFF* tifout, uint32 x_size, char *rowbuffer, uint32 Src_Height, int type)
#else /* __MR_TX_ERR_WORKAROUND__ */
int copyEncodeFaxFile(FILE* rawin, TIFF* tifout, uint32 x_size, char *rowbuffer, uint32 Src_Height)
#endif /* __MR_TX_ERR_WORKAROUND__ */
{
	uint32 row = 0;
	uint32 linesize = TIFFhowmany8(x_size);

	printf(">> encode start\n");

#if __MR_TX_ERR_WORKAROUND__
	if( type == CODING_MR )
	{
		Src_Height = ((uint32)(Src_Height/4)*4);
	}
#endif /* __MR_TX_ERR_WORKAROUND__ */
	while(1)
	{
		if(fread(&rowbuffer[0],sizeof(char),linesize,rawin) != linesize)
		{
			if(row < Src_Height)
			{
				printf(">> read fail row =%d\n",row);
				break;
			}
			else
			{
				printf(">>read end row = %d\n",row);
				break;
			}
		}
			
		if (TIFFWriteScanline(tifout, rowbuffer, row, 0) < 0) 
		{
			fprintf(stderr, "tiff Write error at row %ld.\n",(long) row);
			break;
		}
		
		row++;

#if __MR_TX_ERR_WORKAROUND__
		if( (type == CODING_MR) && (row >= Src_Height) )
		{
			printf(">> reach height = %d\n",row);
			break;
		}
#endif /* __MR_TX_ERR_WORKAROUND__ */
	}

	return (row);
}


int image_validation_check (int res, int height_pixel)
{
	printf("\n(YHCHO) %s() - res : %d, height : %d\n", __FUNCTION__,res, height_pixel);
    if(res == FAX_VR_STANDARD)
    {
		if(height_pixel > 300) //A5 584
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(res == FAX_VR_FINE)
	{
		if(height_pixel > 600) //A5 1169
		{
			return 1;
		}
		else
		{
			return 0;
		}		
	}
	else // 300DPI
	{
		if(height_pixel > 900) //A5 1753
		{
			return 1;
		}
		else
		{
			return 0;
		}		
	}
}

int T4_encoded_image_validation_check(UINT16 docID, char *FileName, int image_width, int rx_resolution, int coding_type)
{
	char	*rowbuf = NULL;
	char	*refbuf = NULL;
	uint32	xsize = 0;
	uint32  rx_res_check = 0;
	uint16 badrun = 0;
    uint8 codingtype = coding_type; //CODING_MMR
	FILE *in;
	TIFF *out = NULL;
	TIFF	*faxTIFF = NULL;
	int compression_in = COMPRESSION_CCITTFAX4; //COMPRESSION_CCITTFAX3,COMPRESSION_CCITTFAX4
	int compression_out = COMPRESSION_CCITTFAX3; //COMPRESSION_CCITTFAX3,COMPRESSION_CCITTFAX4
	int fillorder_in = FILLORDER_LSB2MSB;
	int fillorder_out = FILLORDER_LSB2MSB;
	uint32 group3options_in = 0;	/* 1d-encoded */ // &= ~GROUP3OPT_2DENCODING, |= GROUP3OPT_2DENCODING
	uint32 group3options_out = 0;	/* 1d-encoded */
	uint32 group4options_in = 0;	/* compressed */

	uint32 rowsperstrip;
	int photometric_in = PHOTOMETRIC_MINISWHITE;
	int photometric_out = PHOTOMETRIC_MINISWHITE;
	int mode = FAXMODE_CLASSF;
	int rows;

	float resY = 0.0;
	float resX = 0.0;
	extern int optind;
	extern char* optarg;
    char TmpFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

	printf("\n(YHCHO) %s() - %s \n", __FUNCTION__, FileName);

    snprintf(TmpFileName, sizeof(TmpFileName), "%s%cDoc%04X%cfax%02X.tif", pRootFolderNameRAM, PATH_SEPARATOR, docID, PATH_SEPARATOR, 0);

	if(codingtype == CODING_MH)
	{
		compression_in = COMPRESSION_CCITTFAX3;
		group3options_in &= ~GROUP3OPT_2DENCODING;
	}
	else if(codingtype == CODING_MR)
	{
		compression_in = COMPRESSION_CCITTFAX3;
		group3options_in |= GROUP3OPT_2DENCODING;
	}
    else
	{
		compression_in = COMPRESSION_CCITTFAX4;
		group4options_in = 0;
	}

	xsize = image_width;

	rx_res_check = rx_resolution;
	if(rx_resolution == FAX_VR_STANDARD)
	{
		resX = 204.0;
		resY = 98.0;
	}
	else if(rx_resolution == FAX_VR_FINE)
	{
		resX = 204.0;
		resY = 196.0;		
	}
	else
	{
		resX = 300.0;
		resY = 300.0;		
	}
	
	rowbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	refbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	if (rowbuf == NULL || refbuf == NULL ) 
	{
		fprintf(stderr, "Not enough memory\n");
		if(rowbuf != NULL)
		{
			_TIFFfree(rowbuf);
		}
		if(refbuf != NULL)
		{
			_TIFFfree(refbuf);
		}
		return (EXIT_FAILURE);
	}
	memset(rowbuf,0,TIFFhowmany8(xsize));
	memset(refbuf,0,TIFFhowmany8(xsize));

    out = TIFFOpen(TmpFileName, "w");
	if (out == NULL) {
        fprintf(stderr, "Can not create %s\n", TmpFileName);
        _TIFFfree(rowbuf);
        _TIFFfree(refbuf);

		return (EXIT_FAILURE);
	}
		
	faxTIFF = TIFFClientOpen("(FakeInput)", "w",
	/* TIFFClientOpen() fails if we don't set existing value here */
				 TIFFClientdata(out),
				 TIFFGetReadProc(out), TIFFGetWriteProc(out),
				 TIFFGetSeekProc(out), TIFFGetCloseProc(out),
				 TIFFGetSizeProc(out), TIFFGetMapFileProc(out),
				 TIFFGetUnmapFileProc(out));
	if (faxTIFF == NULL) 
	{
		fprintf(stderr, "Can not create fake input file\n");
		_TIFFfree(rowbuf);
		_TIFFfree(refbuf);

		return (EXIT_FAILURE);
	}
	TIFFSetMode(faxTIFF, O_RDONLY);
	TIFFSetField(faxTIFF, TIFFTAG_IMAGEWIDTH,	xsize);
	TIFFSetField(faxTIFF, TIFFTAG_SAMPLESPERPIXEL,	1);
	TIFFSetField(faxTIFF, TIFFTAG_BITSPERSAMPLE,	1);
	TIFFSetField(faxTIFF, TIFFTAG_FILLORDER,	fillorder_in);
	TIFFSetField(faxTIFF, TIFFTAG_PLANARCONFIG,	PLANARCONFIG_CONTIG);
	TIFFSetField(faxTIFF, TIFFTAG_PHOTOMETRIC,	photometric_in);
	TIFFSetField(faxTIFF, TIFFTAG_YRESOLUTION,	resY);
	TIFFSetField(faxTIFF, TIFFTAG_RESOLUTIONUNIT,	RESUNIT_INCH);
	
	/* NB: this must be done after directory info is setup */
	TIFFSetField(faxTIFF, TIFFTAG_COMPRESSION, compression_in);
	if (compression_in == COMPRESSION_CCITTFAX3)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP3OPTIONS, group3options_in);
	else if (compression_in == COMPRESSION_CCITTFAX4)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP4OPTIONS, group4options_in);

	in = fopen(FileName, "rb");
	if (in == NULL) {
		fprintf(stderr,
			"%s: Can not open\n",FileName);
		_TIFFfree(rowbuf);
		_TIFFfree(refbuf);

		return (EXIT_FAILURE);
	}
	
	TIFFSetClientdata(faxTIFF, (thandle_t)fileno(in));

	TIFFSetFileName(faxTIFF, FileName);
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, xsize);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 1);
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression_out);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric_out);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
	switch (compression_out) {
		/* g3 */
		case COMPRESSION_CCITTFAX3:
		TIFFSetField(out, TIFFTAG_GROUP3OPTIONS,
				 group3options_out);
		TIFFSetField(out, TIFFTAG_FAXMODE, mode);
		rowsperstrip = TIFFDefaultStripSize(out, 0);
		break;
	}
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_FILLORDER, fillorder_out);
	//TIFFSetField(out, TIFFTAG_SOFTWARE, "fax2tiff");
	TIFFSetField(out, TIFFTAG_XRESOLUTION, resX); //204.0 ??
	TIFFSetField(out, TIFFTAG_YRESOLUTION, resY); // 196. ??
	TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	//TIFFSetField(out, TIFFTAG_PAGENUMBER, pn, npages);

	printf("\n(YHCHO) %s() - Decoding start : %d, %d\n", __FUNCTION__,rx_res_check,xsize);
	rows = copyDecodeFaxFileTest(faxTIFF, out, xsize, refbuf, rowbuf, &badrun);
	printf("\n(YHCHO) %s() - Decoding end\n", __FUNCTION__);

	fclose(in);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, rows);
	TIFFClose(out);
	
    fsu_delete_file_on_file_system(TmpFileName);
	
	_TIFFfree(rowbuf);
	_TIFFfree(refbuf);
	
	if(badrun == 10) //fail decoding
	{
		return (EXIT_FAILURE);
	}
	
	if(!image_validation_check(rx_resolution, rows))
	{
		return (EXIT_FAILURE);
	}
	
	return (EXIT_SUCCESS);
}


int copyDecodeFaxFileTest(TIFF* tifin, TIFF* tifout, uint32 x_size, char *refbuffer, char *rowbuffer, uint16 *badruncheck)
{
	uint32 row;
	uint32 linesize = TIFFhowmany8(x_size);
	int ok;
	int datasize = 0;
	int total_error_line = 0;
	uint16 goodrun;	

	tifin->tif_rawdatasize = (tmsize_t)TIFFGetFileSize(tifin);
	datasize = tifin->tif_rawdatasize;
	printf("\n(YHCHO) %s() start 1 : %d\n", __FUNCTION__,(int) tifin->tif_rawdatasize);
	tifin->tif_rawdata = _TIFFmalloc(tifin->tif_rawdatasize);

	if (tifin->tif_rawdata == NULL) 
	{
		TIFFError(tifin->tif_name, "Not enough memory");
		return (0);
	}
	if (!ReadOK(tifin, tifin->tif_rawdata, tifin->tif_rawdatasize)) 
	{
		TIFFError(tifin->tif_name, "Read error at scanline 0");
		return (0);
	}
	
	tifin->tif_rawcp = tifin->tif_rawdata;
	tifin->tif_rawcc = tifin->tif_rawdatasize;

	(*tifin->tif_setupdecode)(tifin);
	(*tifin->tif_predecode)(tifin, (tsample_t) 0);

	tifin->tif_row = 0;

	printf("\n(YHCHO) %s() start 7\n", __FUNCTION__);

	_TIFFmemset(refbuffer, 0, linesize);
	row = 0;
	*badruncheck = 0;		/* current run of bad lines */
	goodrun = 0;
	
	while (tifin->tif_rawcc > 0) 
	{
		ok = (*tifin->tif_decoderow)(tifin, (tdata_t) rowbuffer, linesize, 0);
		if (ok == -1) //error
		{
			printf("\n(YHCHO) %s() !ok\n", __FUNCTION__);
			*badruncheck += 1;
			total_error_line++;

			memset(rowbuffer,0x00,linesize);
			
			if(((tifin->tif_rawcc < datasize*0.1) && *badruncheck == 3) || (*badruncheck == 10))
				break;
		}
		else if( ok == 1) // End Of Line
		{
			printf("\n(YHCHO) %s() EOL ok\n", __FUNCTION__);
			break;
		}
		else // 1 line decode OK
		{	
			if(*badruncheck>0)
			{
				goodrun++;
				if(goodrun == 5)
				{
					*badruncheck -= 1;
					goodrun = 0;
				}
			}
			_TIFFmemcpy(refbuffer, rowbuffer, linesize);
		}
		tifin->tif_row++;

        row++;
	}
	_TIFFfree(tifin->tif_rawdata);

	printf("\n(YHCHO) %s() row = %d\n", __FUNCTION__,row);
	printf("\n(YHCHO) %s() total_error_line = %d\n", __FUNCTION__,total_error_line);
	
	return (row);
}


#if __TX_REPORT_IMAGE_PRINT__
void tx_report_image_setting(UINT16 jnum, int type)
{
    char JBGFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
	int FaxSendCurrentPage = 0;
	UINT16 OriginalDocId = 0;
	UINT16 DocId = fsu_get_doc_id(jnum);

	printf("\n(YHCHO) %s() jnum = %d, (Type = %d)!!\n", __FUNCTION__,jnum, type);

	if(type == 0) //scan
	{
		FaxSendCurrentPage = 1;
	}
	else //tx
	{
		if (fsu_get_photo_mode(jnum) == 1)
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				OriginalDocId = fsu_get_original_doc_id(jnum);

				if (OriginalDocId == DocId)
				{
					FaxSendCurrentPage = fsu_requset_get_current_page_no_from_Job_Queue_File(jnum) + 1;
				}
				else
				{
					FaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
				}
			}
			else
			{
				FaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
			}
		}
		else
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				OriginalDocId = fsu_get_original_doc_id(jnum);

				if (OriginalDocId == DocId)
				{
					FaxSendCurrentPage = fsu_requset_get_current_page_no_from_Job_Queue_File(jnum) + 1;
				}
				else
				{
					FaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
				}
			}
			else
			{
				FaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
			}
		}
	}

    // Step 1. T4 (MMR) Decode
	if (fsu_get_broadcast(jnum) == 1)
	{
		snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, fsu_get_original_doc_id(jnum), PATH_SEPARATOR, FaxSendCurrentPage);
	}
	else
	{
		snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, FaxSendCurrentPage);
	}
	
	snprintf(PBMFileName, sizeof(PBMFileName), "/tmp/faxTx.pbm");

	if (access(PBMFileName, F_OK) == 0)
	{
		printf("\n(YHCHO) %s() delete faxTx.pbm!!\n", __FUNCTION__);
		fsu_delete_file_on_file_system(PBMFileName);
	}

    if (fsu_get_fax_forward(jnum) == 1)
    {
        T4_Decoding(jnum, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(jnum), fsu_get_resolution(jnum), FAX_APP_JOB_DIRECTION_TRANSMIT);
    }
    else
    {
        T4_Decoding(jnum, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(jnum), 3, FAX_APP_JOB_DIRECTION_TRANSMIT);
    }
}

void tx_report_image_scaling(UINT16 jnum)
{
	char RAWFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
	char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
	FILE *outRAW = (FILE *)NULL;
	FILE *outPBM = (FILE *)NULL;
	uint8_t *target_data = (uint8_t *)NULL;
	uint8_t *tmp_buffer = (uint8_t *)NULL;
	uint8_t *buffer = (uint8_t *)NULL;
	int newWidth = 0;
	int newHeight = 0;
	int tempWidth = 0;
	int tempHeight = 0;
	int area = 0;
	int current_width = 0;
	int current_height = 0;
	uint32_t mem_req;
	uint8_t *mem;
	void *handle = (void *)NULL;
    int size, c, j, i, Index;
    char type, tempchar;

	printf("\n(YHCHO) %s() !!\n", __FUNCTION__);

	snprintf(RAWFileName, sizeof(RAWFileName), "%s", pTxResultRptImageName);
	snprintf(PBMFileName, sizeof(PBMFileName), "/tmp/faxTx.pbm");

	outPBM = fopen(PBMFileName, "rb");
	if(outPBM == (FILE *)NULL)
	{
		if (access(RAWFileName, F_OK) == 0)
		{
			printf(">>%s() %s is not exist !! delete %s\n",__FUNCTION__, PBMFileName, RAWFileName);
			fsu_delete_file_on_file_system(RAWFileName);

			return;
		}
		else
		{
			printf(">>%s() %s is not exist\n",__FUNCTION__, PBMFileName);
			return;
		}
	}

	outRAW = fopen(RAWFileName, "wb");
	if(outRAW == (FILE *)NULL)
	{
		printf(">>%s() file open error %s\n",__FUNCTION__, RAWFileName);
		XASSERT(0,0);

		return;
	}

	/* read PBM header */
	while ((c = getc(outPBM)) != EOF && (isspace(c) || c == '#'))
		if (c == '#')
		  while ((c = getc(outPBM)) != EOF && !(c == 13 || c == 10)) ;

    type = (char)getc(outPBM);
	if (c != 'P' || (type != '1' && type != '4')) {
		fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", PBMFileName);
		return;
	}
	current_width = getint(outPBM); // current_width
	current_height = getint(outPBM); // current_height
	fgetc(outPBM);    /* skip line feed */  

    area = current_width * current_height;
    tmp_buffer = (uint8_t *)MEM_MALLOC(area >> 3);
    if(tmp_buffer == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() MEM_MALLOC fail\n", __FUNCTION__);
		XASSERT(0,0);
	}

    size = fread(tmp_buffer, area >> 3, 1, outPBM);
	if(size < 0)
	{
		printf("\n(YHCHO) %s() read fail\n", __FUNCTION__);
        XASSERT(0,0);
	}

    buffer = (uint8_t *)MEM_MALLOC(area);
    if(buffer == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() MEM_MALLOC fail\n", __FUNCTION__);
		XASSERT(0,0);
	}

	// Convert 1-bit to 8-bit
	j = 0;
    Index = area >> 3;

    for(i = 0; i < Index; )
	{
		tempchar = tmp_buffer[i++];

		buffer[j++]=(tempchar&0x80)? 0 : 255;
		buffer[j++]=(tempchar&0x40)? 0 : 255;
		buffer[j++]=(tempchar&0x20)? 0 : 255;
		buffer[j++]=(tempchar&0x10)? 0 : 255;
		buffer[j++]=(tempchar&0x08)? 0 : 255;
		buffer[j++]=(tempchar&0x04)? 0 : 255;
		buffer[j++]=(tempchar&0x02)? 0 : 255;
		buffer[j++]=(tempchar&0x01)? 0 : 255;
    }
    
    MEM_FREE_AND_NULL(tmp_buffer);

	printf("\n(YHCHO) %s() wid = %d, hig = %d\n", __FUNCTION__, current_width, current_height);

	tempWidth = current_width;
	tempHeight = current_height;

	if(tempWidth > tempHeight)
	{
		if(tempWidth > REPORT_IMAGE_MAX_LEN)
		{
			newWidth = REPORT_IMAGE_MAX_LEN;
			newHeight = (tempHeight * newWidth) / tempWidth;
		}
		else
		{
			newWidth = tempWidth;
			newHeight = tempHeight;
		}
	}
	else
	{
		if(tempHeight > REPORT_IMAGE_MAX_LEN)
		{
			newHeight = REPORT_IMAGE_MAX_LEN;
			newWidth = (tempWidth * newHeight) / tempHeight;
		}
		else
		{
			newWidth = tempWidth;
			newHeight = tempHeight;
		}
	}
	printf("\n(YHCHO) %s() newWidth = %d, newHeight = %d\n", __FUNCTION__, newWidth, newHeight);

	target_data = bilinear_for_report_image(buffer,newWidth,newHeight,current_width,current_height);
	if(target_data == (uint8_t *)NULL)
	{
		printf("\n(YHCHO) %s() scaling fail\n", __FUNCTION__);

		XASSERT(0,0);
	}
	
	MEM_FREE_AND_NULL(buffer);
	
	/* 8bit -> 1bit */
	mem_req = gray2bw_memreq(REPORT_IMAGE_MAX_LEN);
	mem = MEM_MALLOC(mem_req);
	handle = gray2bw_create(REPORT_IMAGE_MAX_LEN, (uint32_t*)mem, mem_req);
	if(handle == (void *)NULL)
	{
		printf("\n(YHCHO) %s()  gray2bw_create fail\n", __FUNCTION__);
		XASSERT(0,0);
	}
	gray2bw_convert(handle, target_data, REPORT_IMAGE_MAX_LEN, 0/*scan*/, fsu_get_photo_mode(jnum));

	area = REPORT_IMAGE_MAX_LEN * REPORT_IMAGE_MAX_LEN;
	if(fwrite(target_data, sizeof(uint8_t), (area >> 3), outRAW) != (area >> 3))
	{
		printf(">> raw file fwrite fail\n");
		XASSERT(0,0);
	}
	
	MEM_FREE_AND_NULL(mem);
	MEM_FREE_AND_NULL(target_data);

	fclose(outPBM);
	fclose(outRAW);

	fsu_delete_file_on_file_system(PBMFileName);
}

void tx_report_image_delete(void)
{
	char RAWFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
	
	snprintf(RAWFileName, sizeof(RAWFileName), "%s", pTxResultRptImageName);
	
	if (access(RAWFileName, F_OK) == 0)
	{
		fsu_delete_file_on_file_system(RAWFileName);
	}
}
#endif
