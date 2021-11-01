#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include "lassert.h"
#include "SDHFaxCommonHeader.h"
#include "faxapp_make2tif.h"
#include "faxapp_make2raw.h"
#include "../../fax_memmgr/include/fax_memAPI.h"


void faxapp_make_tif_init (TTIFOUT_GLOBALS *gtTIFOut);
void faxapp_make_tif_write_header (TTIFOUT_GLOBALS *gtTIFOut);
void faxapp_make_tif_write_descritptor (TIF_CLIENT tParam, TTIFOUT_GLOBALS *gtTIFOut, BOOLEAN isLastPage);
void faxapp_make_tif_close (TIF_CLIENT tParam, TTIFOUT_GLOBALS *gtTIFOut);
void faxapp_make_tif_end (int jnum, int StartPageNo, int TotalPageNo, int PageType);


static int faxapp_tif_seq = 0;

TIF_CLIENT tifParam;
TTIFOUT_GLOBALS tifOutParam;


extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void ScaleRect(uint8_t *Target, uint8_t *Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern BOOLEAN fsu_check_remaining_image_file (UINT16 jnum, UINT8 FileType);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 *reduce_mipmap(uint8_t *_data, int newWidth, int newHeight, int _width, int _height);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern UINT32 fsu_get_image_width_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_size (UINT16 jnum, UINT8 PageNo, UINT8 FileType);
extern int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction);
extern unsigned long getint(FILE *f);
#ifdef HAVE_IMAGELOG
extern int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
#endif
#if __HAVE_FAXSOLUTION_DK__
extern char *fsu_get_file_name_tx (UINT16 jnum);

extern UINT8 isDKSolution;
#endif

extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern char *pRootFolderARIA;
extern UINT8 isSDCardEnable;


void faxapp_make_tif_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType)
{
    UINT8 IndexToPage = 0;
//#if __HAVE_FAXSOLUTION_DK__
//	UINT8 res = 3;
//#else
    UINT8 res = fsu_get_resolution(jnum);
//#endif
    UINT16 DocId = fsu_get_doc_id(jnum);
    char tifFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char ariaFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char pbmFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char Raw8bppFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    struct timeval CurrentTime;
    struct tm *ptm;
    FILE *pbmFile = (FILE *)NULL;
    FILE *rawFile = (FILE *)NULL;
    char type = 0;
    int c = 0;
    int wid = 0;
    int hig = 0;
    int DataSize = 0;
    size_t m_width = 0;
    size_t m_height = 0;
    int i, j, tempchar;
    int change_width = 0;
    int change_height = 0;
    int area = 0;
    int len = 0;
    UINT8 *m_data = (UINT8 *)NULL;
    UINT8 *Data = (UINT8 *)NULL;
    UINT8 *change_8_to_1_data = (UINT8 *)NULL;
    UINT8 *change_data = (UINT8 *)NULL;
    
    if(isDKSolution)
    {
		res = 3;
	}

    #define CODING_MMR     2

    gettimeofday(&CurrentTime, NULL);

    ptm = localtime(&CurrentTime.tv_sec);
#if __HAVE_FAXSOLUTION_DK__
#endif
	if(isDKSolution)
	{
		snprintf(tifFileName, sizeof(tifFileName), "%s/%s.tif", Path, fsu_get_file_name_tx(jnum));
	}
	else
	{
		if (ptm == NULL)
		{
			snprintf(tifFileName, sizeof(tifFileName), "%s%c%04d%02d%02d_%02d%02d%02d.tif", Path, PATH_SEPARATOR, 1900, 1, 1, 0, 0, 0);
		}
		else
		{
			snprintf(tifFileName, sizeof(tifFileName), "%s%c%04d%02d%02d_%02d%02d%02d.tif", Path, PATH_SEPARATOR, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		}
	}

    // Step 1. Open .tiff and save the stream ('/SolDocMan/ImgDocXXXX/' where XXXX is jnum not the document ID...)
    tifOutParam.SaveFile = fopen(tifFileName, "wb");

    if (tifOutParam.SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - fopen() Error: %s\n", __FUNCTION__, tifFileName);

        XASSERT(0,0);
    }

    printf("\n(YHCHO) %s() - tifFileName: %s", __FUNCTION__, tifFileName);

    for (IndexToPage = StartPageNo; IndexToPage < StartPageNo + TotalPageNo; IndexToPage++)
    {
        // Step 2.T.4 Decode (MMR)
        snprintf(ariaFileName, sizeof(ariaFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderARIA, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        snprintf(pbmFileName, sizeof(pbmFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
		{
			if(ARIA_encode_path(ariaFileName, jbgFileName, 1, 1) == -1)
			{
				printf("\n(YHCHO) %s() - ARIA Decoding Fail\n", __FUNCTION__);
			}
		}
#endif
        T4_Decoding((UINT16)jnum, jbgFileName, pbmFileName, CODING_MMR, image_width, res, FAX_APP_JOB_DIRECTION_RECEIVE_AND_SAVE);

		if(isSDCardEnable)
			fsu_delete_file_on_file_system (jbgFileName);

        pbmFile = fopen(pbmFileName, "rb");

        if (pbmFile == (FILE *)NULL)
        {
            printf("\n(YHCHO) %s() - fopen() Error: %s\n", __FUNCTION__, pbmFileName);

            XASSERT(0,0);
        }

        wid = (fsu_get_image_width_pixel(jnum) / 8);

        hig = fsu_get_image_height_pixel(jnum);

        Data = (UINT8 *)MEM_MALLOC(wid * hig);

        if (Data == (UINT8 *)NULL)
        {
            XASSERT(0,0);
        }

        // Step 3. Delete PBM's Header

        /* read PBM header */
        while ((c = getc(pbmFile)) != EOF && (isspace(c) || c == '#'))
        {
            if (c == '#')
            {
                while ((c = getc(pbmFile)) != EOF && !(c == 13 || c == 10))
                {
                    ;
                }
            }
        }

        type = (char)getc(pbmFile);

        if (c != 'P' || (type != '1' && type != '4'))
        {
            fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", pbmFileName);

            exit(1);
        }

        m_width = getint(pbmFile);
        m_height = getint(pbmFile);
        c = fgetc(pbmFile);    /* skip line feed */

        // Step 5. Resolution Re-Scaling...

        while ( ( len = fread(Data, wid * hig, 1, pbmFile) ) != 0 )
        {
            DataSize += len;
        }

        if (DataSize < 0)
        {
            XASSERT(0, DataSize);
        }

        if (fclose(pbmFile) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system(pbmFileName);

        m_width = fsu_get_image_width_pixel(jnum);

        m_height = fsu_get_image_height_pixel(jnum);

        m_data = (UINT8 *)MEM_MALLOC(m_width * m_height);

        // Convert 1-bit to 8-bit..
        j = 0;

        area = wid * hig;

        for(i = 0; i < area; )
        {
            tempchar = Data[i++];

            m_data[j++]=(tempchar&0x80)? 255 : 0;
            m_data[j++]=(tempchar&0x40)? 255 : 0;
            m_data[j++]=(tempchar&0x20)? 255 : 0;
            m_data[j++]=(tempchar&0x10)? 255 : 0;
            m_data[j++]=(tempchar&0x08)? 255 : 0;
            m_data[j++]=(tempchar&0x04)? 255 : 0;
            m_data[j++]=(tempchar&0x02)? 255 : 0;
            m_data[j++]=(tempchar&0x01)? 255 : 0;
        }

        MEM_FREE_AND_NULL(Data);
#if __HAVE_FAXSOLUTION_DK__
#endif
		if(isDKSolution)
		{
			res = 3;
		}
		else
		{
			res = fsu_get_resolution(jnum);
		}

        if (res == 0)
        {
            change_width = m_width*(1.5);
            change_height = (m_height*3);
        }
        else if (res == 1)
        {
            change_width = m_width*(1.5);
            change_height = (m_height*(1.5));
        }
        else
        {
            change_width = m_width;
            change_height = m_height;
        }

        change_height = (change_height * 2592)/change_width;

        change_width = 2592;

        if(res == 0 || res == 1)
        {
            change_data = (UINT8 *)MEM_MALLOC(change_width * change_height);

            ScaleRect(change_data, m_data, m_width, m_height, change_width, change_height);
        }
        else
        {
            change_data = reduce_mipmap(m_data, change_width, change_height, m_width, m_height);
        }

        MEM_FREE_AND_NULL(m_data);

        area = change_width * change_height;

        change_8_to_1_data = (UINT8 *)MEM_MALLOC(area >> 3);
        if (change_8_to_1_data == (UINT8 *)NULL)
        {
            XASSERT(0,0);
        }

       // Convert 8-bit to 1-bit..
        j = 0;

        for(i = 0; i < area; )
        {
            tempchar = 0;
            if (change_data[i++] > 0) tempchar |= 0x80;
            if (change_data[i++] > 0) tempchar |= 0x40;
            if (change_data[i++] > 0) tempchar |= 0x20;
            if (change_data[i++] > 0) tempchar |= 0x10;
            if (change_data[i++] > 0) tempchar |= 0x08;
            if (change_data[i++] > 0) tempchar |= 0x04;
            if (change_data[i++] > 0) tempchar |= 0x02;
            if (change_data[i++] > 0) tempchar |= 0x01;

            change_8_to_1_data[j++] = tempchar;
        }

        snprintf(Raw8bppFileName, sizeof(Raw8bppFileName), "%s%cDoc%04X%cfax%02X.raw", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        if((rawFile = fopen(Raw8bppFileName, "wb")) == NULL)
        {
            XASSERT(0,0);
        }

        // Write the information to the file
        fwrite(&change_8_to_1_data[0], (area >> 3), 1, rawFile);

        if (fflush(rawFile) != 0)
        {
            XASSERT(0,0);
        }

        if (fsync(rawFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        if (fclose(rawFile) != 0)
        {
            XASSERT(0,0);
        }

        MEM_FREE_AND_NULL(change_data);

        MEM_FREE_AND_NULL(change_8_to_1_data);

        // Step 3. Set TIF_CLIENT struct
        tifParam.ucRotate = 0;
        tifParam.dXResolution = 300;
        tifParam.dYResolution = 300;

        /* Received Image Always Should Be Re-Scaled To 300 X 300... */
        tifParam.nHeight = change_height;
        tifParam.nWidth = change_width;

        tifParam.nImageSize = fsu_get_image_size((UINT16)jnum, IndexToPage, FAX_FILE_TYPE_RAW);

        // Step 4. Init
        // Step 5. Write Header
        if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
        {
            if (IndexToPage == 1)
            {
                faxapp_make_tif_init(&tifOutParam);

                faxapp_make_tif_write_header(&tifOutParam);
            }
        }
        else
        {
            faxapp_make_tif_init(&tifOutParam);

            faxapp_make_tif_write_header(&tifOutParam);
        }

        snprintf(Raw8bppFileName, sizeof(Raw8bppFileName), "%s%cDoc%04X%cfax%02X.raw", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        if((rawFile = fopen(Raw8bppFileName, "rb")) == NULL)
        {
            XASSERT(0,0);
        }

        if (fread(tifOutParam.stream, tifParam.nImageSize, 1, rawFile) == 0)
        {
            printf("\n(YHCHO) %s() - fread Error... %s\n", __FUNCTION__, pbmFileName);

            XASSERT(0,0);
        }

        if (fclose(rawFile) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system(Raw8bppFileName);

        // Step 6. Write Descriptor
        if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
        {
            if (IndexToPage == (TotalPageNo + StartPageNo) - 1)
            {
                faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, TRUE);
            }
            else
            {
                faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, FALSE);
            }
        }
        else
        {
            faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, TRUE);
        }
    }

    // Step 7. Close
    faxapp_make_tif_close(tifParam, &tifOutParam);
#if __HAVE_FAXSOLUTION_DK__
#endif
	if(isDKSolution)
	{
		faxapp_tif_seq = 0;
	}
	else
	{
		// Step 8. End
		faxapp_make_tif_end(jnum, StartPageNo, TotalPageNo, PageType);
	}
}


void faxapp_make_tif_start_dk (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType)
{
    UINT8 IndexToPage = 0;
    UINT16 DocId = fsu_get_doc_id(jnum);
    char tifFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char pbmFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    FILE *rawFile = (FILE *)NULL;
    int change_width = 0;
    int change_height = 0;

	if(isDKSolution)
	{
		snprintf(tifFileName, sizeof(tifFileName), "%s/%s.tif", Path, fsu_get_file_name_tx(jnum));
	}

    // Step 1. Open .tiff and save the stream ('/SolDocMan/ImgDocXXXX/' where XXXX is jnum not the document ID...)
    tifOutParam.SaveFile = fopen(tifFileName, "wb");

    if (tifOutParam.SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - fopen() Error: %s\n", __FUNCTION__, tifFileName);

        XASSERT(0,0);
    }

    printf("\n(YHCHO) %s() - tifFileName: %s", __FUNCTION__, tifFileName);

    for (IndexToPage = StartPageNo; IndexToPage < StartPageNo + TotalPageNo; IndexToPage++)
    {
		change_width = fsu_get_image_width_pixel(jnum);
		change_height = fsu_get_image_height_pixel(jnum);

        // Step 3. Set TIF_CLIENT struct
        tifParam.ucRotate = 0;
        tifParam.dXResolution = 300;
        tifParam.dYResolution = 300;

        /* Received Image Always Should Be Re-Scaled To 300 X 300... */
        tifParam.nHeight = change_height;
        tifParam.nWidth = change_width;

        tifParam.nImageSize = (change_height*change_width) >> 3; //fsu_get_image_size((UINT16)jnum, IndexToPage, FAX_FILE_TYPE_RAW);

        // Step 4. Init
        // Step 5. Write Header
        if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
        {
            if (IndexToPage == 1)
            {
                faxapp_make_tif_init(&tifOutParam);

                faxapp_make_tif_write_header(&tifOutParam);
            }
        }
        else
        {
            faxapp_make_tif_init(&tifOutParam);

            faxapp_make_tif_write_header(&tifOutParam);
        }

        snprintf(pbmFileName, sizeof(pbmFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        if((rawFile = fopen(pbmFileName, "rb")) == NULL)
        {
            XASSERT(0,0);
        }

        if (fread(tifOutParam.stream, tifParam.nImageSize, 1, rawFile) == 0)
        {
            printf("\n(YHCHO) %s() - fread Error... %s\n", __FUNCTION__, pbmFileName);

            XASSERT(0,0);
        }

        if (fclose(rawFile) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system(pbmFileName);

        // Step 6. Write Descriptor
        if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
        {
            if (IndexToPage == (TotalPageNo + StartPageNo) - 1)
            {
                faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, TRUE);
            }
            else
            {
                faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, FALSE);
            }
        }
        else
        {
            faxapp_make_tif_write_descritptor(tifParam, &tifOutParam, TRUE);
        }
    }

    // Step 7. Close
    faxapp_make_tif_close(tifParam, &tifOutParam);

	if(isDKSolution)
	{
		faxapp_tif_seq = 0;
	}
	else
	{
		// Step 8. End
		faxapp_make_tif_end(jnum, StartPageNo, TotalPageNo, PageType);
	}
}



void faxapp_make_tif_init (TTIFOUT_GLOBALS *gtTIFOut)
{
    if ( faxapp_tif_seq != 0 )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    gtTIFOut->stream = (unsigned char *)MEM_MALLOC(MAX_STREAM_SIZE * sizeof (unsigned char));

    if (gtTIFOut->stream == (unsigned char *)NULL)
    {
        XASSERT(0,0);
    }

    faxapp_tif_seq = TIFF_INIT;
}


void faxapp_make_tif_write_header (TTIFOUT_GLOBALS *gtTIFOut)
{
    TIFHEAD	tIFH = { };

    if ( faxapp_tif_seq != TIFF_INIT )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    gtTIFOut->pageno = 0;

    gtTIFOut->filepos = 0;

    tIFH.Identifier = 0x4949;

    tIFH.Version = 0x2A;

    tIFH.IFDOffset = 8;

    if( fwrite(&tIFH, 1, sizeof(tIFH), gtTIFOut->SaveFile) <= 0 )
    {
        printf("\n(YHCHO) %s() - fwrite() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    gtTIFOut->filepos += sizeof(tIFH);

    faxapp_tif_seq = TIFF_IFH;
}


void faxapp_make_tif_write_descritptor (TIF_CLIENT tParam, TTIFOUT_GLOBALS *gtTIFOut, BOOLEAN isLastPage)
{
    TIFIFD tIFD = { };
    unsigned char ucCnt = 0;

    if ( (faxapp_tif_seq != TIFF_IFH) && (faxapp_tif_seq != TIFF_IFD) )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (gtTIFOut->SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - gtTIFOut->SaveFile's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (tParam.nImageSize == 0)
    {
        printf("\n(YHCHO) %s() - tParam.nImageSize is 0...\n", __FUNCTION__);

        XASSERT(0,0);
    }

#if 1
	UINT32 isEncodeOK = 0;
	Fax3CodecState sp;
	sp.rowpixels = tParam.nWidth;
	sp.imagesize = tParam.nImageSize;
	Fax3SetupState(&sp);
	Fax3PreEncode(&sp);
	isEncodeOK = Fax3Encode(&sp, gtTIFOut->stream);
	printf("\n(YHCHO) %s() - sp.rowpixels: %d, sp.imagesize: %d, isEncodeOK: %ld\n", __FUNCTION__, sp.rowpixels, sp.imagesize, isEncodeOK);
	if (isEncodeOK)
	{
		tParam.nImageSize = sp.tif_rawcc;
	}
#endif

    tIFD.NumDirEntries = NO_OF_TAGS;

    tIFD.TagList[ucCnt].TagId = 0x00FF;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x0100;
    tIFD.TagList[ucCnt].DataType = 4;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = tParam.nWidth;

    tIFD.TagList[ucCnt].TagId = 0x0101;
    tIFD.TagList[ucCnt].DataType = 4;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = tParam.nHeight;

    tIFD.TagList[ucCnt].TagId = 0x0102;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x0103;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    if (isEncodeOK)
        tIFD.TagList[ucCnt++].DataOffset = 3;//fax;
    else
        tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x0106;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 0;

    tIFD.TagList[ucCnt].TagId = 0x0107;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x010A;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x010F;
    tIFD.TagList[ucCnt].DataType = 2;
    tIFD.TagList[ucCnt].DataCount = 7;
    tIFD.TagList[ucCnt++].DataOffset = gtTIFOut->filepos + sizeof(tIFD) - 23;

    tIFD.TagList[ucCnt].TagId = 0x0111;
    tIFD.TagList[ucCnt].DataType = 4;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = gtTIFOut->filepos + sizeof(tIFD);

    tIFD.TagList[ucCnt].TagId = 0x0115;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x0116;
    tIFD.TagList[ucCnt].DataType = 4;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = tParam.nHeight;

    tIFD.TagList[ucCnt].TagId = 0x0117;
    tIFD.TagList[ucCnt].DataType = 4;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = tParam.nImageSize;

    tIFD.TagList[ucCnt].TagId = 0x0118;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 0;

    tIFD.TagList[ucCnt].TagId = 0x0119;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 1;

    tIFD.TagList[ucCnt].TagId = 0x011A;
    tIFD.TagList[ucCnt].DataType = 5;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = gtTIFOut->filepos + sizeof(tIFD) - 16;

    tIFD.TagList[ucCnt].TagId = 0x011B;
    tIFD.TagList[ucCnt].DataType = 5;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = gtTIFOut->filepos + sizeof(tIFD) - 8;

    tIFD.TagList[ucCnt].TagId = 0x0128;
    tIFD.TagList[ucCnt].DataType = 3;
    tIFD.TagList[ucCnt].DataCount = 1;
    tIFD.TagList[ucCnt++].DataOffset = 2;

    memcpy(tIFD.Manufacture, "SINDOH", 6);

    tIFD.XDpi[0] = tParam.dXResolution;
    tIFD.XDpi[1] = 1;
    tIFD.YDpi[0] = tParam.dYResolution;
    tIFD.YDpi[1] = 1;

    if (isLastPage == FALSE)
    {
        tIFD.NextIFDOffset = gtTIFOut->filepos + sizeof(tIFD) + tParam.nImageSize;
    }
    else
    {
        tIFD.NextIFDOffset = 0;
    }

    if (fwrite(&tIFD, 1, sizeof(tIFD), gtTIFOut->SaveFile) <= 0)
    {
        printf("\n(YHCHO) %s() - fwrite Error...\n", __FUNCTION__);

        XASSERT(0,0);
    }
    else
    {
        gtTIFOut->filepos += sizeof(tIFD);

        if (isEncodeOK)
        {
            if (fwrite(sp.tif_rawbase, 1, sp.tif_rawcc, gtTIFOut->SaveFile) <= 0)
            {
                printf("\n(YHCHO) %s() - EncodeOK fwrite Error..\n", __FUNCTION__);

                XASSERT(0,0);
            }
        }
        else
        {
            if (fwrite(gtTIFOut->stream, 1, tParam.nImageSize, gtTIFOut->SaveFile) <= 0)
            {
                printf("\n(YHCHO) %s() - RAW fwrite Error..\n", __FUNCTION__);

                XASSERT(0,0);
            }
        }

        gtTIFOut->filepos += tParam.nImageSize;
    }

    gtTIFOut->pageno++;

    faxapp_tif_seq = TIFF_IFD;
}


void faxapp_make_tif_close (TIF_CLIENT tParam, TTIFOUT_GLOBALS *gtTIFOut)
{
    if ( faxapp_tif_seq != TIFF_IFD )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    MEM_FREE_AND_NULL(gtTIFOut->stream);

    if (fflush(gtTIFOut->SaveFile) != 0)
    {
        XASSERT(0,0);
    }

    if (fsync(fileno(gtTIFOut->SaveFile)) == -1)
    {
        XASSERT(0,0);
    }

    if (fclose(gtTIFOut->SaveFile) != 0)
    {
        XASSERT(0,0);
    }

    faxapp_tif_seq = TIFF_CLOSE;
}


void faxapp_make_tif_end (int jnum, int StartPageNo, int TotalPageNo, int PageType)
{
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 DocId = fsu_get_doc_id((UINT16)jnum);
    UINT16 IndexToPage = 0;

    if ( faxapp_tif_seq != TIFF_CLOSE )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    for (IndexToPage = StartPageNo; IndexToPage < TotalPageNo + StartPageNo; IndexToPage++)
    {
        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        fsu_delete_file_on_file_system(jbgFileName);
    }

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

    snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

    if (PageType == SAVE_FILE_PAGE_TYPE_MULTI_PAGE)
    {
        if (fsu_check_remaining_image_file((UINT16)jnum, FAX_FILE_TYPE_JBG) == FALSE)
        {
            if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
            {
                fsu_delete_file_on_file_system(JobQueueFileName);
            }

            fsu_delete_dir_on_file_system(DocDirPathNAND);

            fsu_delete_dir_on_file_system(DocDirPathRAM);
        }
    }
    else
    {
        if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
        {
            fsu_delete_file_on_file_system(JobQueueFileName);
        }

        fsu_delete_dir_on_file_system(DocDirPathNAND);

        fsu_delete_dir_on_file_system(DocDirPathRAM);
    }

    faxapp_tif_seq = 0;
}

