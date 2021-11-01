#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include "lassert.h"
#include "SDHFaxCommonHeader.h"
#include "faxapp_make2pdf.h"
#include "faxapp_make2raw.h"
#include "../../fax_memmgr/include/fax_memAPI.h"


void faxapp_make_pdf_init (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut);
void faxapp_make_pdf_write_header (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut);
void faxapp_make_pdf_write_trailer (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut);
void faxapp_make_pdf_close (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut);
void faxapp_make_pdf_end (int jnum, int StartPageNo, int TotalPageNo, int PageType, int Reason);
void faxapp_invert_mem (void *src, unsigned int count);


static int faxapp_pdf_seq = 0;

PDF_CLIENT pdfParam;
TPDFOUT_GLOBALS pdfOutParam;


extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void ScaleRect(uint8_t *Target, uint8_t *Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern BOOLEAN fsu_check_remaining_image_file (UINT16 jnum, UINT8 FileType);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 *reduce_mipmap(uint8_t *_data, int newWidth, int newHeight, int _width, int _height);
extern char* get_il_filename(int _jobid);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern UINT32 fsu_get_image_width_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_size (UINT16 jnum, UINT8 PageNo, UINT8 FileType);
extern UINT32 fsu_get_image_width_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);
extern int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction);
#ifdef HAVE_IMAGELOG
extern int set_metafile_start_info(char _jobtype, int _subJobType, int _jobid, char* _username);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
#endif
extern unsigned long getint(FILE *f);

#if __HANA_FINANCIAL_GROUP_REQ_021__
extern char *fsu_get_file_name_tx (UINT16 jnum);
#endif
#ifdef HAVE_IMAGELOG
extern int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
#endif
extern char gFaxImageLogSrcPath[MAX_STRING_LEN];
extern char gFaxImageLogDstPath[MAX_STRING_LEN];
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
#ifdef HAVE_IMAGELOG
extern char gFaxUserId[MAX_LEN_USER_ID + 1];
#endif
extern int FileFormat;

#if __HANA_FINANCIAL_GROUP_REQ_021__
extern char *pRootFolderNameSolDocMan;
extern char *pRootFolderARIA;
extern UINT8 isSDCardEnable;
#endif

void faxapp_make_pdf_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType, int Request, int Reason)
{
    UINT8 IndexToPage = 0;
    UINT8 res = fsu_get_resolution(jnum);
    UINT16 DocId = fsu_get_doc_id(jnum);
    char pdfFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char pdfFileNameARIA[MAX_LEN_FILE_PATH_NAME] = {0, };
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
#ifdef HAVE_IMAGELOG
    char *pImageLog = (char *)NULL;
#endif

    #define CODING_MMR     2

    if (Path != NULL)
    {
        gettimeofday(&CurrentTime, NULL);

        ptm = localtime(&CurrentTime.tv_sec);

        if (ptm == NULL)
        {
            snprintf(pdfFileName, sizeof(pdfFileName), "%s%c%04d%02d%02d_%02d%02d%02d.pdf", Path, PATH_SEPARATOR, 1900, 1, 1, 0, 0, 0);
        }
        else
        {
            snprintf(pdfFileName, sizeof(pdfFileName), "%s%c%04d%02d%02d_%02d%02d%02d.pdf", Path, PATH_SEPARATOR, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        }
    }
    else
    {
#ifdef HAVE_IMAGELOG
        if (Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG || Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG_FOR_PCFAX)
        {
            if (StartPageNo == 1)
            {
                if (Request == REQUEST_SOURCE_FAX)
                {
                    set_metafile_start_info('F', IMAGE_LOG_JOB_TYPE_RX, fsu_get_unique_job_id(jnum), gFaxUserId);
                }
                else
                {
                    set_metafile_start_info('F', IMAGE_LOG_JOB_TYPE_TX, fsu_get_unique_job_id(jnum), gFaxUserId);
                }
            }

            pImageLog = get_il_filename(fsu_get_unique_job_id(jnum));

			snprintf(pdfFileName, sizeof(pdfFileName), "%s%cDoc%04X%cfax%02X.pdf", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            snprintf(pdfFileNameARIA, sizeof(pdfFileNameARIA), "%s/%s/%s_%d.pdf", IMAGE_LOG_PATH, pImageLog, pImageLog, StartPageNo);
		}
#else
#endif

#if __HANA_FINANCIAL_GROUP_REQ_021__
		if(Reason == FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION_FOR_HANA)
		{
			if(PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
			{
				snprintf(pdfFileName, sizeof(pdfFileName), "%s%cImgDoc%04X%c%s.pdf", pRootFolderNameSolDocMan, PATH_SEPARATOR, jnum, PATH_SEPARATOR, fsu_get_file_name_tx(jnum));
			}
			else
			{
				snprintf(pdfFileName, sizeof(pdfFileName), "%s%cImgDoc%04X%c%s_%d.pdf", pRootFolderNameSolDocMan, PATH_SEPARATOR, jnum, PATH_SEPARATOR, fsu_get_file_name_tx(jnum), StartPageNo);
			}
			printf("\n(YHCHO) %s() - pdfFileName: %s\n", __FUNCTION__, pdfFileName);
		}
#endif
    }

    // Step 1. Open .pdf and save the stream ('/SolDocMan/ImgDocXXXX/' where XXXX is jnum not the document ID...)
    pdfOutParam.SaveFile = fopen(pdfFileName, "wb");

    if (pdfOutParam.SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - fopen() Error: %s\n", __FUNCTION__, pdfFileName);

        XASSERT(0,0);
    }

    printf("\n(YHCHO) %s() - pdfFileName: %s", __FUNCTION__, pdfFileName);

    for (IndexToPage = StartPageNo; IndexToPage < StartPageNo + TotalPageNo; IndexToPage++)
    {
#if 0
		if(Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG)
		{
			snprintf(pbmFileName, sizeof(pbmFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
		}
		else
#endif
		//{
			// Step 2. T4. Decode (MMR)
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
			if (Path != NULL)
			{
				T4_Decoding((UINT16)jnum, jbgFileName, pbmFileName, CODING_MMR, image_width, res, FAX_APP_JOB_DIRECTION_RECEIVE_AND_SAVE);
			}
			else
			{
				if (Request == REQUEST_SOURCE_FAX)
				{
					T4_Decoding((UINT16)jnum, jbgFileName, pbmFileName, CODING_MMR, image_width, res, FAX_APP_JOB_DIRECTION_RECEIVE_AND_SAVE);
				}
				else // PC Fax only...
				{
					T4_Decoding((UINT16)jnum, jbgFileName, pbmFileName, CODING_MMR, image_width, 3, FAX_APP_JOB_DIRECTION_RECEIVE_AND_SAVE);
				}
			}

			if(isSDCardEnable)
				fsu_delete_file_on_file_system (jbgFileName);
		//}
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

		if(Reason != FAXAPP_MAKE_PDF_REASON_IMAGE_LOG)
		{
			fsu_delete_file_on_file_system(pbmFileName);
		}

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

        res = fsu_get_resolution(jnum);

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

        printf("\n(YHCHO) %s() - m_width: %d, m_height: %d, change_width: %d, change_height: %d\n", __FUNCTION__, m_width, m_height, change_width, change_height);

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

        // Step 36. Set PDF_CLIENT struct
        pdfParam.ucBitDepth = 1;
        pdfParam.uTotalPlanes = 1;
        pdfParam.ucRotate = 0;
        pdfParam.dXResolution = 300;
        pdfParam.dYResolution = 300;

        /* Received Image Always Should Be Re-Scaled To 300 X 300... */
        pdfParam.nHeight = change_height;
        pdfParam.nWidth = change_width;

        pdfParam.nImageSize = fsu_get_image_size((UINT16)jnum, IndexToPage, FAX_FILE_TYPE_RAW);

        // Step 4. Init.
        if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
        {
            if (IndexToPage == 1)
            {
                faxapp_make_pdf_init(pdfParam, &pdfOutParam);
            }
        }
        else
        {
            faxapp_make_pdf_init(pdfParam, &pdfOutParam);
        }

        snprintf(Raw8bppFileName, sizeof(Raw8bppFileName), "%s%cDoc%04X%cfax%02X.raw", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

        if((rawFile = fopen(Raw8bppFileName, "rb")) == NULL)
        {
            XASSERT(0,0);
        }

        if (fread(pdfOutParam.stream, pdfParam.nImageSize, 1, rawFile) == 0)
        {
            printf("\n(YHCHO) %s() - fread Error... %s\n", __FUNCTION__, Raw8bppFileName);

            XASSERT(0,0);
        }

        if (fclose(rawFile) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system(Raw8bppFileName);

        // Step 4. Write Header
        faxapp_make_pdf_write_header(pdfParam, &pdfOutParam);

        // Step 6. Write Trailer
        faxapp_make_pdf_write_trailer(pdfParam, &pdfOutParam);
    }

    // Step 7. Close
    faxapp_make_pdf_close(pdfParam, &pdfOutParam);

    if (Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG || Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG_FOR_PCFAX)
    {
		if(ARIA_encode_path(pdfFileName, pdfFileNameARIA, 0, 0) == -1)
		{
			XASSERT(0,0);
		}
	}

    // Step 8. End
    faxapp_make_pdf_end(jnum, StartPageNo, TotalPageNo, PageType, Reason);
}


void faxapp_make_pdf_init (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut)
{

    UINT32 length = 0;

    if ( (faxapp_pdf_seq != PDF_CLOSE) && (faxapp_pdf_seq != 0) )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    gtPDFOut->pageno = 0;

    gtPDFOut->nobjects = 2;

    gtPDFOut->filepos = 0;

    memset(gtPDFOut->offsets, 0, sizeof(gtPDFOut->offsets));

    if (gtPDFOut->fkids)
    {
        if (fclose(gtPDFOut->fkids) != 0)
        {
            XASSERT(0,0);
        }

        gtPDFOut->fkids = (FILE *)NULL;

        fsu_delete_file_on_file_system("/tmp/fkids.sh");
    }

    if (gtPDFOut->foffsets)
    {
        if (fclose(gtPDFOut->foffsets) != 0)
        {
            XASSERT(0,0);
        }

        gtPDFOut->foffsets = (FILE *)NULL;

        fsu_delete_file_on_file_system("/tmp/foffsets.sh");
    }

    gtPDFOut->fkids = fopen("/tmp/fkids.sh", "w+");

    if ( gtPDFOut->fkids == (FILE *)NULL )
    {
        printf("\n(YHCHO) %s() Open fkids.sh Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    gtPDFOut->foffsets = fopen("/tmp/foffsets.sh", "w+");

    if (gtPDFOut->foffsets == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() Open foffsets.sh Error!\n", __FUNCTION__);

        if (fclose(gtPDFOut->fkids) != 0)
        {
            XASSERT(0,0);
        }

        gtPDFOut->fkids = (FILE *)NULL;

        XASSERT(0,0);
    }

    if (gtPDFOut->SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - fopen() Error...\n", __FUNCTION__);

        if (fclose(gtPDFOut->fkids) != 0)
        {
            XASSERT(0,0);
        }

        if (fclose(gtPDFOut->foffsets) != 0)
        {
            XASSERT(0,0);
        }

        XASSERT(0,0);
    }

    length = fprintf(gtPDFOut->SaveFile,
                     "%%PDF-1.5\n"
                     "%%\343\342\317\323\n");

    gtPDFOut->filepos += length;

    if (tParam.ucBitDepth == 1)
    {
        gtPDFOut->stream = (unsigned char *)MEM_MALLOC(MAX_STREAM_SIZE * sizeof (unsigned char));
    }

    faxapp_pdf_seq = PDF_INIT;
}


void faxapp_make_pdf_write_header (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut)
{
    UINT32 length = 0;

    /* Write the PDF header, then the whole compressed raster, then
    * the trailer. Use PDF 1.5 as we are now supporting 16 bit. */

    if ( (faxapp_pdf_seq != PDF_INIT) && (faxapp_pdf_seq != PDF_TRAILER) )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (gtPDFOut->SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - gtPDFOut->SaveFile's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n"
                     "<<\n"
                     "/Type /XObject\n"
                     "/Subtype /Image\n"
                     "/Width %d\n"
                     "/Height %d\n"
                     "/BitsPerComponent %d\n"
                     "/Length %ld 0 R\n"
                     "/ColorSpace ",
                     gtPDFOut->nobjects,
                     tParam.nWidth,
                     tParam.nHeight,
                     tParam.ucBitDepth,
                     (gtPDFOut->nobjects + 1));

    gtPDFOut->filepos += length;

    /* Write the color space */
    if (tParam.uTotalPlanes == 1)
    {
        length = fprintf(gtPDFOut->SaveFile,
                         "/DeviceGray\n");
    }
    else if (tParam.uTotalPlanes == 3)
    {
        length = fprintf(gtPDFOut->SaveFile,
                         "/DeviceRGB\n");
    }
    else if (tParam.uTotalPlanes == 4)
    {
        length = fprintf(gtPDFOut->SaveFile,
                         "/DeviceCMYK\n");
    }

    gtPDFOut->filepos += length;

    if (tParam.ucBitDepth != 1)
    {
        length = fprintf(gtPDFOut->SaveFile,
                         "/Filter /DCTDecode\n"
                         ">> stream\n");

        gtPDFOut->filepos += length;
    }

    faxapp_pdf_seq = PDF_HEADER;
}


void faxapp_make_pdf_write_trailer (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut)
{
    UINT32 streamlength = 0;
    UINT32 length = 0;
    UINT32 nRotate = 0;
    UINT32 retval = 0;
    UINT32 isEncodeOK = 0;
    Fax3CodecState sp;

    if (faxapp_pdf_seq != PDF_HEADER)
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (gtPDFOut->SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - gtPDFOut->SaveFile's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (tParam.ucBitDepth == 1)
    {
        sp.rowpixels = tParam.nWidth;
        sp.imagesize = tParam.nImageSize;

        Fax3SetupState(&sp);

        Fax3PreEncode(&sp);

        isEncodeOK = Fax3Encode(&sp, gtPDFOut->stream);

        printf("\n(YHCHO) %s() - sp.rowpixels: %d, sp.imagesize: %d, isEncodeOK: %ld\n", __FUNCTION__, sp.rowpixels, sp.imagesize, isEncodeOK);

        if (isEncodeOK)
        {
            length = fprintf(gtPDFOut->SaveFile,
                             "/Filter [/CCITTFaxDecode]\n"
                             "  /DecodeParms[<</Columns %d /Rows %d>>]\n"
                             ">> stream\n",
                             tParam.nWidth,
                             tParam.nHeight);

            gtPDFOut->filepos += length;

            printf("\n(YHCHO) %s() - sp.tif_rawcc: %d\n", __FUNCTION__, sp.tif_rawcc);

            retval = fwrite(sp.tif_rawbase, 1, sp.tif_rawcc, gtPDFOut->SaveFile);

            if(retval <= 0)
            {
                printf("\n(YHCHO) %s() - fwrite Error..\n", __FUNCTION__);

                XASSERT(0,0);
            }

            gtPDFOut->filepos += sp.tif_rawcc;

            tParam.nImageSize = sp.tif_rawcc;
        }
        else
        {
            length = fprintf(gtPDFOut->SaveFile,
                             ">> stream\n");

            gtPDFOut->filepos += length;

            faxapp_invert_mem(gtPDFOut->stream, tParam.nImageSize);

            retval = fwrite(gtPDFOut->stream, 1, tParam.nImageSize, gtPDFOut->SaveFile);

            if(retval <= 0)
            {
                printf("\n(YHCHO) %s() - fwrite Error..\n", __FUNCTION__);

                XASSERT(0,0);
            }

            gtPDFOut->filepos += tParam.nImageSize;
        }

        Fax3PostEncode(&sp);
    }
    else
    {
        gtPDFOut->filepos += tParam.nImageSize;
    }

    length = fprintf(gtPDFOut->SaveFile,
                     "\nendstream\n"
                     "endobj\n");

    gtPDFOut->filepos += length;

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n" /* Length of image */
                     "%d\n"
                     "endobj\n",
                     gtPDFOut->nobjects,
                     tParam.nImageSize);

    gtPDFOut->filepos += length;

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n" /* Page resources */
                     "<<\n"
                     "/XObject << /Im%ld %ld 0 R >>\n"
                     "/ProcSet [/PDF /ImageB /ImageC]\n"
                     ">>\n"
                     "endobj\n",
                     gtPDFOut->nobjects,
                     (gtPDFOut->pageno + 1),
                     (gtPDFOut->nobjects - 2));

    gtPDFOut->filepos += length;

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n" /* Page contents stream */
                     "<< /Length %ld 0 R >>\n"
                     "stream\n",
                     gtPDFOut->nobjects,
                     (gtPDFOut->nobjects + 1));

    gtPDFOut->filepos += length;

    streamlength = fprintf(gtPDFOut->SaveFile,
                           "%f 0 0 %f 0 0 cm\nq\n%d 0 0 %d 0 0 cm\n/Im%ld Do\nQ\n",
                           ((tParam.nWidth * 72.0 / tParam.dXResolution)/tParam.nWidth),
                           (72.0/tParam.dYResolution),
                           tParam.nWidth,
                           tParam.nHeight,
                           (gtPDFOut->pageno + 1));

    gtPDFOut->filepos += streamlength;

    length = fprintf(gtPDFOut->SaveFile,
                     "endstream\n"
                     "endobj\n");

    gtPDFOut->filepos += length;

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n" /* Length of page contents stream */
                     "%ld\n"
                     "endobj\n",
                     gtPDFOut->nobjects,
                     streamlength);

    gtPDFOut->filepos += length;

    fprintf(gtPDFOut->foffsets,
            "%ld\n",
            gtPDFOut->filepos);

    gtPDFOut->nobjects++;

    fprintf(gtPDFOut->fkids,
            "%ld\n",
            gtPDFOut->nobjects);

    gtPDFOut->pageno++;

    nRotate = (tParam.ucRotate * 90);

    length = fprintf(gtPDFOut->SaveFile,
                     "%ld 0 obj\n" /* Page object */
                     "<<\n"
                     "/Type /Page\n"
                     "/Resources %ld 0 R\n"
                     "/Contents %ld 0 R\n"
                     "/Parent 1 0 R\n"
                     "/MediaBox [0 0 %.2f %.2f]\n"
                     "/Rotate %ld\n"
                     ">>\n"
                     "endobj\n",
                     gtPDFOut->nobjects,
                     (gtPDFOut->nobjects - 3),
                     (gtPDFOut->nobjects - 2),
                     ((tParam.nWidth * 72.0) / tParam.dXResolution),
                     ((tParam.nHeight * 72.0) / tParam.dYResolution),
                     nRotate);

    gtPDFOut->filepos += length;

    tParam.nImageSize = 0;

    faxapp_pdf_seq = PDF_TRAILER;
}


void faxapp_make_pdf_close (PDF_CLIENT tParam, TPDFOUT_GLOBALS *gtPDFOut)
{
    UINT32 length = 0;
    UINT32 i = 0;
    UINT32 nkids = 0;
    UINT32 noffset = 0;

    if (faxapp_pdf_seq != PDF_TRAILER)		// trailer
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if (gtPDFOut->SaveFile == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - gtPDFOut->SaveFile's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if ( gtPDFOut->fkids == (FILE *)NULL )
    {
        printf("\n(YHCHO) %s() - gtPDFOut->fkids's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }
    else
    {
        if (fflush(gtPDFOut->fkids) != 0)
        {
            XASSERT(0,0);
        }

        if (fsync(fileno(gtPDFOut->fkids)) == -1)
        {
            XASSERT(0,0);
        }

        rewind(gtPDFOut->fkids);
    }

    if ( gtPDFOut->foffsets == (FILE *)NULL)
    {
        printf("\n(YHCHO) %s() - gtPDFOut->foffsets's NULL...\n", __FUNCTION__);

        XASSERT(0,0);
    }
    else
    {
        if (fflush(gtPDFOut->foffsets) != 0)
        {
            XASSERT(0,0);
        }

        if (fsync(fileno(gtPDFOut->foffsets)) == -1)
        {
			XASSERT(0,0);
        }

        rewind(gtPDFOut->foffsets);
    }

    gtPDFOut->offsets[0] = gtPDFOut->filepos;

    gtPDFOut->nobjects++;

    /* pages */
    length = fprintf(gtPDFOut->SaveFile,
                     "1 0 obj\n"
                     "<<\n"
                     "/Type /Pages\r"
                     "/Kids [ ");

    gtPDFOut->filepos += length;

    for ( i = 0; i < gtPDFOut->pageno; ++i )
    {
        if (fscanf(gtPDFOut->fkids,
               "%ld",
               &nkids) == -1)
        {
            XASSERT(0,0);
        }

        length = fprintf(gtPDFOut->SaveFile,
                         "%ld 0 R\r\n",
                         nkids);

        gtPDFOut->filepos += length;
    }

    length = fprintf(gtPDFOut->SaveFile,
                     "  ]\n"
                     "  /Count %ld\n"
                     ">>\n"
                     "endobj\n",
                     gtPDFOut->pageno);

    gtPDFOut->filepos += length;

    gtPDFOut->offsets[1] = gtPDFOut->filepos;

    gtPDFOut->nobjects++;

    length = fprintf(gtPDFOut->SaveFile,
                     "2 0 obj\n" /* Info */
                     "<<\n"
                     "/Type /Catalog\n"
                     "/Pages 1 0 R\n"
                     ">>\n"
                     "endobj\n");

    gtPDFOut->filepos += length;

    length = fprintf(gtPDFOut->SaveFile,
                     "xref\n"
                     "0 %ld\n"
                     "0000000000 65535 f \n",
                     (gtPDFOut->nobjects - 1));

    length = fprintf(gtPDFOut->SaveFile,
                     "%010ld 00000 n \n",
                     gtPDFOut->offsets[0]);

    length = fprintf(gtPDFOut->SaveFile,
                     "%010ld 00000 n \n",
                     gtPDFOut->offsets[1]);

    if (fscanf(gtPDFOut->foffsets,
           "%ld",
           &noffset) == -1)
    {
        XASSERT(0,0);
    }

    while(!feof(gtPDFOut->foffsets))
    {
        length = fprintf(gtPDFOut->SaveFile,
                         "%010ld 00000 n \n",
                         noffset);

        fscanf(gtPDFOut->foffsets,
               "%ld",
               &noffset);
    }

    length = fprintf(gtPDFOut->SaveFile,
                     "trailer\n"
                     "<<\n"
                     "/Size %ld\n"
                     "/Root 2 0 R\n"
                     ">>\n"
                     "startxref\n"
                     "%ld\n"
                     "%%%%EOF\r\n",
                     (gtPDFOut->nobjects - 1),
                     gtPDFOut->filepos);

    if (fflush(gtPDFOut->SaveFile) != 0)
    {
        XASSERT(0,0);
    }

    if (fsync(fileno(gtPDFOut->SaveFile)) == -1)
    {
        XASSERT(0,0);
    }

    if (fclose(gtPDFOut->SaveFile) != 0)
    {
        XASSERT(0,0);
    }

    gtPDFOut->SaveFile = (FILE *)NULL;

    if (tParam.ucBitDepth == 1)
    {
        MEM_FREE_AND_NULL(gtPDFOut->stream);
    }

    if ( gtPDFOut->fkids != (FILE *)NULL )
    {
        if (fclose(gtPDFOut->fkids) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system("/tmp/fkids.sh");

        gtPDFOut->fkids = (FILE *)NULL;
    }

    if ( gtPDFOut->foffsets != (FILE *)NULL)
    {
        if (fclose(gtPDFOut->foffsets) != 0)
        {
            XASSERT(0,0);
        }

        fsu_delete_file_on_file_system("/tmp/foffsets.sh");

        gtPDFOut->foffsets = (FILE *)NULL;
    }

    faxapp_pdf_seq = PDF_CLOSE;
}


void faxapp_make_pdf_end (int jnum, int StartPageNo, int TotalPageNo, int PageType, int Reason)
{
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 DocId = fsu_get_doc_id((UINT16)jnum);
    UINT16 IndexToPage = 0;

    if ( faxapp_pdf_seq != PDF_CLOSE )
    {
        printf("\n(YHCHO) %s() Error!\n", __FUNCTION__);

        XASSERT(0,0);
    }

    if ( Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG || Reason == FAXAPP_MAKE_PDF_REASON_IMAGE_LOG_FOR_PCFAX)
    {
        // No Action
    }
    else
    {
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
    }

    faxapp_pdf_seq = 0;
}


void faxapp_invert_mem (void *src, unsigned int count)
{
    UINT8 *pSrc = (UINT8 *)src;

    if (src == (UINT8 *)NULL)
    {
        printf("\n(YHCHO) %s() - src is NULL...\n", __FUNCTION__);

        return;
    }
    else
    {
        if (count == 0)
        {
            printf("\n(YHCHO) %s() - count is 0...\n", __FUNCTION__);

            return;
        }
        else
        {
            while (count-- > 0)
            {
                *pSrc = ~(*pSrc);
                (*pSrc)++;
            }
        }
    }
}

