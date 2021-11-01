/* ************************************************************************* **
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                Copyright (c) 2005 Oasis Semiconductor Inc.                **
**                                                                           **
**  This software contains proprietary and confidential material which is    **
**  property of Oasis Semiconductor Inc.                                     **
**                                                                           **
** ************************************************************************* */
/*
    File: fsu_mem.c
    Date: March 16, 2005
    By:   S.J. Busak

    Contains functions for using dynamic memory.

    November 1, 2005: Modified to use NEW_MEM_C option

*/

#define __FSU_MEM_C__    // Allocates storage for variables in fsu_mem.h
#include "fax_memAPI.h"
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "SDHFaxCommonHeader.h"
#include "dc_type.h"
#include "mosi.h"
#include "dc_fwcxdxx_lnx_api.h"
#include "fsu_api.h"
#include "fshs.h"
#include "fsu.h"
#include "fsu_call.h"
#include "fxcb.h"
#include <stdio.h>
#include "jbig85.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include "lassert.h"
#include "faxapp_notify.h"
#include "errno.h"

void fsu_delete_file_on_file_system (const char *Path);
UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
UINT16 fsu_get_last_page_no_from_file_name (UINT16 TotalPageNo, char **FileName, UINT8 FileType, UINT8 Mode);

extern void fsu_update_image_width_pixel (UINT16 jnum, UINT32 width);
extern void fsu_update_image_height_pixel (UINT16 jnum, UINT32 height);
extern UINT8 fsu_convert_char_to_int (char No);
extern UINT8 fsu_get_print_reason (UINT16 jnum);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern unsigned long getint(FILE *f);

extern char *pCurrentDirName;
extern char *pPreviousDirName;
extern char *pRootFolderName;
extern UINT8 gFaxSendCurrentPageOnHook;
extern int is1stPageDataTransfered;
extern int isEncodeComplete;


#define JBG85_DECODE_BUF_SIZE_RCV     (5*1*1024*1024)

unsigned long y_0;
fpos_t ypos;
int ypos_error = 1;
unsigned long ymax = 0;
extern char *pRootFolderNameRAM;


int line_out(const struct jbg85_dec_state *jbg85_dec_rcv, unsigned char *start, size_t len, unsigned long y, void *file)
{
    if (y == 0)
    {
        /* prefix first line with PBM header */
        fprintf((FILE *) file, "P4\n");
        fprintf((FILE *) file, "%10lu\n", jbg85_dec_getwidth(jbg85_dec_rcv));

        /* store file position of height, so we can update it after NEWLEN */
        y_0 = jbg85_dec_getheight(jbg85_dec_rcv);

        ypos_error = fgetpos((FILE *) file, &ypos);
        fprintf((FILE *) file, "%10lu\n", y_0); /* pad number to 10 bytes */
    }

    fwrite(start, len, 1, (FILE *) file);

    return y == ymax - 1;
}


void *checkedmalloc(size_t n)
{
    void *p;

    if ((p = MEM_MALLOC(n)) == NULL)
    {
        fprintf(stderr, "Sorry, not enough memory available! : %d\n",n);

        XASSERT(0,0);
    }

    return p;
}

#if 0
/*
 * Read an ASCII integer number from file f and skip any PBM
 * comments which are encountered.
 */
static unsigned long getint(FILE *f)
{
    int c;
    unsigned long i = 0;

    while ((c = getc(f)) != EOF && !isdigit(c))
        if (c == '#')
             while ((c = getc(f)) != EOF && !(c == 13 || c == 10))
                 ;

    if (c != EOF)
    {
        ungetc(c, f);

        if(fscanf(f, "%lu", &i) == -1)
        {
			assert(0);
		}
    }

    return i;
}
#endif

static void data_out(unsigned char *start, size_t len, void *file)
{
    fwrite(start, len, 1, (FILE *) file);
    return;
}


int sw_jbig_decode_tx(int Page, UINT16 jnum, char *FileName, UINT8 Direction)
{
    FILE *jbg85_decode_out = (FILE *)NULL;
    FILE *jbg85_decode_in = (FILE *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    size_t readCnt = 0;
    size_t readCnt2 = 0;
    size_t readLen = 0;
    unsigned long xmax = 8192;
    size_t outbuflen = 0;
    size_t bytes_read = 0;
    int result = 0;
    struct jbg85_dec_state jbg85_dec_rcv;
    unsigned char *jbg85_dec_rcv_in_buffer = (unsigned char *)NULL;
    unsigned char *jbg85_dec_rcv_out_buffer = (unsigned char *)NULL;
    unsigned long width = 0;
    unsigned long height = 0;

    memset(&jbg85_dec_rcv, 0x00, sizeof(jbg85_dec_rcv));

    snprintf(PathName, sizeof(PathName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, Page);

    jbg85_decode_in = fopen (FileName, "r");
    if ( jbg85_decode_in == (FILE *)NULL )
    {
        printf("\n(YHCHO) %s() - File %s Does Not Exist [1]\n", __FUNCTION__, FileName);

        XASSERT(0,0);
    }

    jbg85_decode_out = fopen (PathName, "wr");
    if ( jbg85_decode_out == (FILE *)NULL )
    {
        printf("\n(YHCHO) %s() - File %s Does Not Exist [2]\n", __FUNCTION__, PathName);

        XASSERT(0,0);
    }

    jbg85_dec_rcv_in_buffer = (unsigned char *) MEM_MALLOC(JBG85_DECODE_BUF_SIZE_RCV);
    if ( jbg85_dec_rcv_in_buffer == (unsigned char *)NULL )
    {
        printf("\n(YHCHO) %s() - Input Buffer Does Not Exist...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    outbuflen = ((xmax >> 3) + !!(xmax & 7)) * 3;
    printf("\n(YHCHO) %s() - outbuflen: %d\n", __FUNCTION__, outbuflen);
    jbg85_dec_rcv_out_buffer = (unsigned char *) MEM_MALLOC(outbuflen);

    if ( jbg85_dec_rcv_out_buffer == (unsigned char *)NULL )
    {
        printf("\n(YHCHO) %s() - Output Buffer Does Not Exist...\n", __FUNCTION__);

        XASSERT(0,0);
    }

    /* Step 1. Init. parameter struct for JBIG decoder */
    jbg85_dec_init(&jbg85_dec_rcv, jbg85_dec_rcv_out_buffer, outbuflen, line_out, jbg85_decode_out);

    result = JBG_EAGAIN;

    /* Step 2. Send input file to decoder */
    while ( (readLen = fread(jbg85_dec_rcv_in_buffer, 1, JBG85_DECODE_BUF_SIZE_RCV, jbg85_decode_in)) )
    {
        result = jbg85_dec_in(&jbg85_dec_rcv, jbg85_dec_rcv_in_buffer, readLen, &readCnt);

        bytes_read += readCnt;

        while ( result == JBG_EOK_INTR )
        {
            /* demonstrate decoder interrupt at given line number */
            printf("\n(YHCHO) %s() - Decoding interrupted after %lu lines and %lu BIE bytes... continuing... [1]\n", __FUNCTION__, jbg85_dec_rcv.y, (unsigned long) bytes_read);

            /* and now continue decoding */
            result = jbg85_dec_in(&jbg85_dec_rcv, (jbg85_dec_rcv_in_buffer + readCnt), (readLen - readCnt), &readCnt2);

            bytes_read += readCnt2;

            readCnt += readCnt2;
        }

        if (result != JBG_EAGAIN)
            break;
    }

    /* Step 3. */
    while (result == JBG_EAGAIN || result == JBG_EOK_INTR)
    {
        /* signal end-of-BIE explicitely */
        result = jbg85_dec_end(&jbg85_dec_rcv);
        if (result == JBG_EOK_INTR)
        {
            /* demonstrate decoder interrupt at given line number */
            printf("\n (YHCHO) %s() - Decoding interrupted after %lu lines and %lu BIE bytes... continuing ... [2]\n", __FUNCTION__, jbg85_dec_rcv.y, (unsigned long) bytes_read);
        }
    }
    
	if (result != JBG_EOK) 
	{
		fprintf(stderr, "Problem with input file '%s':\n%s\n"
		"(error code 0x%02x, %lu = 0x%04lx BIE bytes "
		"and %lu pixel rows processed)\n",
		FileName, jbg85_strerror(result), result,
		(unsigned long) bytes_read, (unsigned long) bytes_read, jbg85_dec_rcv.y);
		
		fclose(jbg85_decode_in);
		fclose(jbg85_decode_out);

		MEM_FREE_AND_NULL(jbg85_dec_rcv_in_buffer);
		MEM_FREE_AND_NULL(jbg85_dec_rcv_out_buffer);

		fsu_delete_file_on_file_system(PathName);

		return 1; //decoding failure
	}    

    /* do we have to update the image height in the PBM header? */
    if (!ypos_error && y_0 != jbg85_dec_getheight(&jbg85_dec_rcv))
    {
        if (fsetpos(jbg85_decode_out, &ypos) == 0)
        {
            fprintf(jbg85_decode_out, "%10lu", jbg85_dec_getheight(&jbg85_dec_rcv)); /* pad to 10 bytes */
        }
        else
        {
            fprintf(stderr, "Problem while updating height in output file '%s", PathName);

            printf("\n(YHCHO) %s() - Problem while updating height in output file '%s'\n", __FUNCTION__, PathName);

			fclose(jbg85_decode_in);
			fclose(jbg85_decode_out);

			MEM_FREE_AND_NULL(jbg85_dec_rcv_in_buffer);
			MEM_FREE_AND_NULL(jbg85_dec_rcv_out_buffer);
			
			fsu_delete_file_on_file_system(PathName);

            return 1; //decoding failure
        }
    }

	width = jbg85_dec_getwidth(&jbg85_dec_rcv);
	height = jbg85_dec_getheight(&jbg85_dec_rcv);

	fsu_update_image_width_pixel(jnum, width);
	fsu_update_image_height_pixel(jnum, height);

    fclose(jbg85_decode_in);
    fclose(jbg85_decode_out);

    MEM_FREE_AND_NULL(jbg85_dec_rcv_in_buffer);
    MEM_FREE_AND_NULL(jbg85_dec_rcv_out_buffer);
#if 0
    if ( (Direction == FAX_APP_JOB_DIRECTION_PRINT) && (fsu_get_print_reason(jnum) == PRINT_REASON_RESET) )
    {
        is1stPageDataTransfered = 1;
    }
#endif

    return 0;
}


int sw_jbig_encode_tx (int Page, char *FileName, UINT16 jnum, UINT8 Direction)
{
    FILE *fin = NULL, *fout = NULL;
    const char *fnin = NULL, *fnout = NULL;
    int c;
    unsigned long x, y;
    unsigned long width, height;
    size_t bpl;
    char type;
    unsigned char *p, *lines, *next_line;
    unsigned char *prev_line = NULL, *prevprev_line = NULL;
    int mx = -1;
    unsigned long l0 = 0;
    int options = 0x08;
    struct jbg85_enc_state s;
    char JBIGFileName[MAX_LEN_FILE_PATH_NAME]= {0, };

    if ( (Direction == FAX_APP_JOB_DIRECTION_SCAN) || (Direction == FAX_APP_JOB_DIRECTION_RECEIVE) )
    {
        snprintf(JBIGFileName, sizeof(JBIGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, Page);
    }
    else
    {
        snprintf(JBIGFileName, sizeof(JBIGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, Page);
    }

    fnin  = FileName; // .pbm
    fnout = JBIGFileName; // .jbg

    // Step 1. Open Input File
    fin = fopen(fnin, "rb");
    if (!fin)
    {
        fprintf(stderr, "Can't open input file '%s", fnin);
        perror("'");

        XASSERT(0,0);
    }

    // Step 2. Read PBM header (e.g.: P4)
    while ((c = getc(fin)) != EOF && (isspace(c) || c == '#'))
        if (c == '#')
            while ((c = getc(fin)) != EOF && !(c == 13 || c == 10)) ;

    type = (char)getc(fin);

    if (c != 'P' || (type != '1' && type != '4'))
    {
        fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", fnin);

        exit(1);
    }

    // Step 3. Get width and height info. for image buffer
    width = getint(fin);
    height = getint(fin);

	printf("\n(YHCHO) %s()  width : %ld, height = %ld\n", __FUNCTION__,width, height);

    fgetc(fin);    /* skip line feed */

    /* Test for valid parameters */
    if (width < 1 || height < 1)
    {
        fprintf(stderr, "Image dimensions must be positive!\n");

        exit(1);
    }

    // Step 4. Allocate buffer for a single image line
    bpl = (width >> 3) + !!(width & 7);     /* bytes per line */
    lines = (unsigned char *) checkedmalloc(bpl * 3);

    // Step 5. Open Output File
    fout = fopen(fnout, "wb");
    if (!fout)
    {
        fprintf(stderr, "Can't open input file '%s", fnout);
        perror("'");

        exit(1);
    }

    // Step 6. Initialize parameter struct for JBIG encoder*/
    jbg85_enc_init(&s, width, height, data_out, fout);

    // Step 7. Specify a few other options (each is ignored if negative)
    jbg85_enc_options(&s, options, l0, mx);

    for (y = 0; y < height; y++)
    {
        /* Use a 3-line ring buffer, because the encoder requires that the two
         * previously supplied lines are still in memory when the next line is
         * processed. */

        next_line = lines + (y%3)*bpl;

        switch (type)
        {
            case '1':
                /* PBM text format */
                p = next_line;
                for (x = 0; x <= ((width-1) | 7); x++)
                {
                    *p <<= 1;

                    if (x < width)
                            *p |= getint(fin) & 1;

                    if ((x & 7) == 7)
                            ++p;
                }
                break;

            case '4':
                /* PBM raw binary format */
                if(fread(next_line, bpl, 1, fin) < 1)
                {
					exit(1);
				  }
                break;

            default:
                fprintf(stderr, "Unsupported PBM type P%c!\n", type);

                exit(1);
        }

        if (ferror(fin))
        {
            fprintf(stderr, "Problem while reading input file '%s", fnin);
            perror("'");

            exit(1);
        }

        /* JBIG compress another line and write out result via callback */
        jbg85_enc_lineout(&s, next_line, prev_line, prevprev_line);
        prevprev_line = prev_line;
        prev_line = next_line;

        /* adjust final image height via NEWLEN */
        //if (yi && y == yr)
        //{
        //    jbg85_enc_newlen(&s, height);
        //}
    }

    /* check for file errors and close fout */
    if (ferror(fout) || fclose(fout))
    {
        fprintf(stderr, "Problem while writing output file '%s", fnout);
        perror("'");

        exit(1);
    }

    unlink(FileName);

    fclose(fin);
    
    MEM_FREE_AND_NULL(lines);
    
    return 0;
}


/***********************************************************************
 *  Function: fsu_get_start_page_no_in_mem()
 *
 *  Description:
 *    get the start page number in /DocMan/DocXXXX/
 *
 *  Inputs:
 *    UINT16 DocId: doc id
 *    UINT8 FileType: .jbg, .pbm, .raw, .photo
 *
 *  Output:
 *	   start page number
 **********************************************************************/
UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType)
{
    UINT8 StartPageNo = 0;
    DIR *pDir = (DIR *)NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char PagePathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *FileName = NULL;
    struct dirent *FileInfo = NULL;
    int isFileNameExist = 0;
    int IndexToPageNo = 0;

    if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PHOTO) )
    {
		snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);
	}
	else
	{
		snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);
	}
	
    // Step 1. Check to see whether the /DocMan/DocXXXX exists...
    pDir = opendir(DocIdPathName);
    if (pDir == (DIR *)NULL)
    {
        XASSERT(0,0);
    }
    else
    {
        char FileExtension[10] = {0, };
        UINT8 Len = 0;
        UINT8 IndexToExtension = 0;
        UINT8 ExtensionLen = 0;
        UINT8 i = 0;

        snprintf(JobQueueFileName, sizeof(JobQueueFileName), "JobQueue%04X.info", DocId);

        // Step 2. Save all file names to FileName array...
        errno = 0;
        while ( (FileInfo = readdir(pDir)) != NULL )
        {
			isFileNameExist = 0;
			
            if ( (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) || (strcmp (FileInfo->d_name, JobQueueFileName) == 0) )
            {
                ; // Don't save '.', '..' into FileName array...
            }
            else
            {
                memset (FileExtension, 0x00, sizeof(FileExtension));

                if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PBM) || (FileType == FAX_FILE_TYPE_RAW) || (FileType == FAX_FILE_TYPE_TMP_JBG))
                {
                    ExtensionLen = 3;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (FileType == FAX_FILE_TYPE_JBG || FileType == FAX_FILE_TYPE_TMP_JBG)
                    {
                        if (strcmp("jbg", FileExtension) == 0)
                        {
                            FileName = FileInfo->d_name;

                            isFileNameExist = 1;
                        }
                    }
                    else if (FileType == FAX_FILE_TYPE_RAW)
                    {
                        if (strcmp("raw", FileExtension) == 0)
                        {
                            FileName = FileInfo->d_name;

                            isFileNameExist = 1;
                        }
                    }
                    else
                    {
                        if (strcmp("pbm", FileExtension) == 0)
                        {
                            FileName = FileInfo->d_name;

                            isFileNameExist = 1;
                        }
                    }
                }
                else
                {
                    ExtensionLen = 5;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (strcmp("photo", FileExtension) == 0)
                    {
                        FileName = FileInfo->d_name;

                        isFileNameExist = 1;
                    }
                }

				if( isFileNameExist == 1)
				{
					// Step 3. Get the start page number and return it
					for (IndexToPageNo = 1; IndexToPageNo < MAX_NO_IMAGE_FILES; IndexToPageNo++)
					{
						switch (FileType)
						{
							case FAX_FILE_TYPE_JBG:
								snprintf(PagePathName, sizeof(PagePathName), "fax%02X.jbg", IndexToPageNo);
								break;

							case FAX_FILE_TYPE_PBM:
								snprintf(PagePathName, sizeof(PagePathName), "fax%02X.pbm", IndexToPageNo);
								break;

							case FAX_FILE_TYPE_RAW:
								snprintf(PagePathName, sizeof(PagePathName), "fax%02X.raw", IndexToPageNo);
								break;

							case FAX_FILE_TYPE_PHOTO:
								snprintf(PagePathName, sizeof(PagePathName), "fax%02X.photo", IndexToPageNo);
								break;
								
							case FAX_FILE_TYPE_TMP_JBG:
								snprintf(PagePathName, sizeof(PagePathName), "fax%02X.jbg", IndexToPageNo);
								break;

							default:
								break;
						}

						if ( strcmp(FileName, PagePathName) == 0 )
						{
							//printf("\n(YHCHO) %s() - PagePathName: %s, %d, %d\n", __FUNCTION__, PagePathName, StartPageNo, IndexToPageNo);
							
							if(StartPageNo == 0 || (StartPageNo > IndexToPageNo) )
							{
								StartPageNo = IndexToPageNo;
							}

							break;
						}
					}
				}

				errno = 0;
			}
        }

		if(errno != 0)
		{
			printf("\n(YHCHO) %s() - error reading directory: %d\n", __FUNCTION__, errno);
		}

        // Step 4. Close the /DocMan/DocXXXX
        closedir(pDir);

        printf("\n(YHCHO) %s() - isFileNameExist: %d\n", __FUNCTION__, isFileNameExist);
    }

    printf("\n(YHCHO) %s() - FileType: %d, StartPageNo: %d\n", __FUNCTION__, FileType, StartPageNo);

    return StartPageNo;
}


/***********************************************************************
 *  Function: fsu_get_total_page_no_in_mem()
 *
 *  Description:
 *    get the total page number
 *
 *  Inputs:
 *    UINT16 DocId: doc id
 *    UINT8 FileType: type of file
 *
 *  Output:
 *	   start page number
 **********************************************************************/
UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType)
{
    UINT8 TotalPageNo = 0;
    DIR *pDir = (DIR *)NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    struct dirent *FileInfo = NULL;

    if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PHOTO) )
    {
        snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);
    }
    else
    {
        snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);
    }

    pDir = opendir(DocIdPathName);
    if (pDir == (DIR *)NULL)
    {
        XASSERT(0,0);
    }
    else
    {
        char FileExtension[10] = {0, };
        UINT8 Len = 0;
        UINT8 IndexToExtension = 0;
        UINT8 ExtensionLen = 0;
        UINT8 i = 0;

        snprintf(JobQueueFileName, sizeof(JobQueueFileName), "JobQueue%04X.info", DocId);

        while ( (FileInfo = readdir(pDir)) != NULL )
        {
            if ( (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) || (strcmp (FileInfo->d_name, JobQueueFileName) == 0) )
            {
                ; // No Action Here
            }
            else
            {
                if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PBM) || (FileType == FAX_FILE_TYPE_RAW) || (FileType == FAX_FILE_TYPE_TMP_JBG))
                {
                    ExtensionLen = 3;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (FileType == FAX_FILE_TYPE_JBG || FileType == FAX_FILE_TYPE_TMP_JBG)
                    {
                        if (strcmp("jbg", FileExtension) == 0)
                        {
                            TotalPageNo++;
                        }
                    }
                    else if (FileType == FAX_FILE_TYPE_RAW)
                    {
                        if (strcmp("raw", FileExtension) == 0)
                        {
                            TotalPageNo++;
                        }
                    }
                    else
                    {
                        if (strcmp("pbm", FileExtension) == 0)
                        {
                            TotalPageNo++;
                        }
                    }
                }
                else
                {
                    ExtensionLen = 5;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (strcmp("photo", FileExtension) == 0)
                    {
                        TotalPageNo++;
                    }
                }
            }
        }

        closedir(pDir);
    }

    printf("\n(YHCHO) %s() - FileType: %d, TotalPageNo: %d\n", __FUNCTION__, FileType, TotalPageNo);

    return TotalPageNo;
}


/***********************************************************************
 *  Function: fsu_get_last_page_no_in_mem()
 *
 *  Description:
 *    get the last page number of /DocMan/DocXXXX/
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *    UINT8 FileType: type of file
 *    UINT8 Mode: check mode
 *
 *  Output:
 *	   last page number
 **********************************************************************/
UINT16 fsu_get_last_page_no_in_mem (UINT16 DocId, UINT8 FileType, UINT8 Mode)
{
    UINT16 LastPageNo = 0;
    DIR *pDir = (DIR *)NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    pDir = opendir(DocIdPathName);
    if (pDir == (DIR *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        int IndexToPageNo = 0;
        char *FileName[MAX_NO_IMAGE_FILES];
        char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
        char FileExtension[10] = {0, };
        struct dirent *FileInfo = NULL;
        UINT8 Len = 0;
        UINT8 IndexToExtension = 0;
        UINT8 ExtensionLen = 0;
        UINT8 i = 0;

        memset(FileName, 0x00, sizeof(FileName));

        snprintf(JobQueueFileName, sizeof(JobQueueFileName), "JobQueue%04X.info", DocId);

        while ( (FileInfo = readdir(pDir)) != NULL )
        {
            if ( (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) || (strcmp (FileInfo->d_name, JobQueueFileName) == 0) )
            {
                ; // No Action Here
            }
            else
            {
                memset (FileExtension, 0x00, sizeof(FileExtension));

                FileName[IndexToPageNo] = FileInfo->d_name;

                if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PBM) || (FileType == FAX_FILE_TYPE_RAW) )
                {

                    ExtensionLen = 3;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (FileType == FAX_FILE_TYPE_JBG)
                    {
                        if (strcmp("jbg", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                    else if (FileType == FAX_FILE_TYPE_PBM)
                    {
                        if (strcmp("raw", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                    else
                    {
                        if (strcmp("pbm", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                }
                else
                {
                    ExtensionLen = 5;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (strcmp("photo", FileExtension) == 0)
                    {
                        IndexToPageNo++;
                    }
                    else
                    {
                        FileName[IndexToPageNo] = 0;
                    }
                }
            }
        }

        LastPageNo = fsu_get_last_page_no_from_file_name(IndexToPageNo, FileName, FileType, Mode);

		printf("\n(YHCHO) %s() - FileType: %d, LastPageNo: %d\n", __FUNCTION__, FileType, LastPageNo);

        closedir(pDir);

        return LastPageNo;
    }
}


/***********************************************************************
 *  Function: fsu_get_last_page_no_from_file_name()
 *
 *  Description:
 *    get the last page number of /DocMan/DocXXXX/
 *
 *  Inputs:
 *
 *
 *  Output:
 *	   last page number
 **********************************************************************/
UINT16 fsu_get_last_page_no_from_file_name (UINT16 TotalPageNo, char **FileName, UINT8 FileType, UINT8 Mode)
{
    UINT8 Len = 0;
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 SubTotal = 0;
    UINT8 Max_SubTotal = 0;
    UINT8 Tmp[1] = {0, };
    UINT16 LastPageNo = 0;
    UINT16 IndexToPageNo = 0;
    char *TmpFileName[MAX_NO_IMAGE_FILES];
    
    for (IndexToPageNo = 0; IndexToPageNo < TotalPageNo; IndexToPageNo++)
    {
        Len = strlen(FileName[IndexToPageNo]);

        if (FileType == FAX_FILE_TYPE_JBG)
        {
            Tmp[0] = FileName[IndexToPageNo][Len - 6];

            First = fsu_convert_char_to_int (Tmp[0]);

            First = First * 16;

            Tmp[0] = FileName[IndexToPageNo][Len - 5];

            Second = fsu_convert_char_to_int (Tmp[0]);
        }
        else
        {
            Tmp[0] = FileName[IndexToPageNo][Len - 8];

            First = fsu_convert_char_to_int (Tmp[0]);

            First = First * 16;

            Tmp[0] = FileName[IndexToPageNo][Len - 7];

            Second = fsu_convert_char_to_int (Tmp[0]);
        }

        SubTotal = First + Second;
        
        if(Max_SubTotal < SubTotal)
        {
			Max_SubTotal = SubTotal;
        }

        TmpFileName[SubTotal] = FileName[IndexToPageNo];
    }

    if (Mode == 1) // On-Hook
    {
        if (gFaxSendCurrentPageOnHook == 1)
        {
            Len = strlen(TmpFileName[TotalPageNo]);

            if (FileType == FAX_FILE_TYPE_JBG)
            {
                Tmp[0] = TmpFileName[TotalPageNo][Len - 6];

                First = fsu_convert_char_to_int (Tmp[0]);

                First = First * 16;

                Tmp[0] = TmpFileName[TotalPageNo][Len - 5];

                Second = fsu_convert_char_to_int (Tmp[0]);
            }
            else
            {
                Tmp[0] = TmpFileName[TotalPageNo][Len - 8];

                First = fsu_convert_char_to_int (Tmp[0]);

                First = First * 16;

                Tmp[0] = TmpFileName[TotalPageNo][Len - 7];

                Second = fsu_convert_char_to_int (Tmp[0]);
            }
        }
        else
        {
            UINT8 NewPageIndex = TotalPageNo + gFaxSendCurrentPageOnHook - 1;

            Len = strlen(TmpFileName[NewPageIndex]);

            if (FileType == FAX_FILE_TYPE_JBG)
            {
                Tmp[0] = TmpFileName[NewPageIndex][Len - 6];

                First = fsu_convert_char_to_int (Tmp[0]);

                First = First * 16;

                Tmp[0] = TmpFileName[NewPageIndex][Len - 5];

                Second = fsu_convert_char_to_int (Tmp[0]);
            }
            else
            {
                Tmp[0] = TmpFileName[NewPageIndex][Len - 8];

                First = fsu_convert_char_to_int (Tmp[0]);

                First = First * 16;

                Tmp[0] = TmpFileName[NewPageIndex][Len - 7];

                Second = fsu_convert_char_to_int (Tmp[0]);
            }
        }
    }
    else
    {
        Len = strlen(TmpFileName[Max_SubTotal]);

        if (FileType == FAX_FILE_TYPE_JBG)
        {
            Tmp[0] = TmpFileName[Max_SubTotal][Len - 6];

            First = fsu_convert_char_to_int (Tmp[0]);

            First = First * 16;

            Tmp[0] = TmpFileName[Max_SubTotal][Len - 5];

            Second = fsu_convert_char_to_int (Tmp[0]);
        }
        else
        {
            Tmp[0] = TmpFileName[Max_SubTotal][Len - 8];

            First = fsu_convert_char_to_int (Tmp[0]);

            First = First * 16;

            Tmp[0] = TmpFileName[Max_SubTotal][Len - 7];

            Second = fsu_convert_char_to_int (Tmp[0]);
        }
    }

    LastPageNo = First + Second;

    return LastPageNo;
}


/***********************************************************************
 *  Function: fsu_check_remaining_image_file()
 *
 *  Description:
 *    check the remaining image file located in /DocMan/DocXXXX/
 *
 *  Inputs:
 *    UINT16 jnum: job number to check
 *    UINT8 FileType: type of file
 *
 *  Output:
 *	  TRUE: there is one more faxXX.jbg
 *    FALSE: there is no faxXX.jbg
 **********************************************************************/
BOOLEAN fsu_check_remaining_image_file (UINT16 jnum, UINT8 FileType)
{
    BOOLEAN ret_val = FALSE;
    UINT16 DocId = fsu_get_doc_id(jnum);
    int IndexToPageNo = 0;
    DIR *pDir = (DIR *)NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    pDir = opendir(DocIdPathName);
    if (pDir == (DIR *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        struct dirent *FileInfo = NULL;
        char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
        char FileExtension[10] = {0, };
        UINT8 Len = 0;
        UINT8 IndexToExtension = 0;
        UINT8 ExtensionLen = 0;
        UINT8 i = 0;

        snprintf(JobQueueFileName, sizeof(JobQueueFileName), "JobQueue%04X.info", DocId);

        while ( (FileInfo = readdir(pDir)) != NULL )
        {
            if ( (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) || (strcmp (FileInfo->d_name, JobQueueFileName) == 0) )
            {
                ; // No Action Here
            }
            else
            {
                memset (FileExtension, 0x00, sizeof(FileExtension));

                if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_RAW) || (FileType == FAX_FILE_TYPE_PBM) )
                {
                    ExtensionLen = 3;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (FileType == FAX_FILE_TYPE_JBG)
                    {
                        if (strcmp("jbg", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                    else if (FileType == FAX_FILE_TYPE_RAW)
                    {
                        if (strcmp("raw", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                    else
                    {
                        if (strcmp("pbm", FileExtension) == 0)
                        {
                            IndexToPageNo++;
                        }
                    }
                }
                else
                {
                    ExtensionLen = 5;

                    Len = strlen(FileInfo->d_name);

                    for (IndexToExtension = Len - ExtensionLen; IndexToExtension < Len; IndexToExtension++)
                    {
                        FileExtension[i] = FileInfo->d_name[IndexToExtension];

                        i++;
                    }

                    FileExtension[i] = 0;

                    i = 0;

                    if (strcmp("photo", FileExtension) == 0)
                    {
                        IndexToPageNo++;
                    }
                }
            }
        }

        closedir(pDir);
    }

    if (IndexToPageNo == 0)
    {
        ret_val = FALSE;
    }
    else
    {
        ret_val = TRUE;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_get_doc_exist()
 *
 *  Description:
 *    Check DocXXXX is exist
 *
 *  Inputs:
 *    UINT16 DocId: DocId
 *
 *  Output:
 *	   BOOLEAN
 **********************************************************************/
BOOLEAN fsu_get_doc_exist (UINT16 DocId)
{
    DIR *pDir = (DIR *)NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    BOOLEAN ret = FALSE;

    snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    pDir = opendir(DocIdPathName);
    if (pDir == (DIR *)NULL)
    {
        return ret;
    }

    closedir(pDir);
    
    ret = TRUE;
    
    return ret;
}


/***********************************************************************
 *  Function: fsu_delete_file_on_file_system()
 *
 *  Description:
 *    delete a file on file system
 *
 *  Inputs:
 *    const char *Path: path to file
 *
 *  Output:
 *
 **********************************************************************/
void fsu_delete_file_on_file_system (const char *Path)
{
    if (unlink(Path) < 0)
    {
        printf("\n(YHCHO) %s() - errorno: %d", __FUNCTION__, errno);

        ASSERT(0);
    }
}


/***********************************************************************
 *  Function: fsu_make_dir_on_file_system()
 *
 *  Description:
 *    make a directory on file system
 *
 *  Inputs:
 *    const char *Path: path to directory
 *
 *  Output:
 *
 **********************************************************************/
void fsu_make_dir_on_file_system (const char *Path)
{
    if (mkdir(Path, 0777) < 0)
    {
        printf("\n(YHCHO) %s() - errorno: %d, %s", __FUNCTION__, errno, Path);

        ASSERT(0);
    }
}


/***********************************************************************
 *  Function: fsu_delete_dir_on_file_system()
 *
 *  Description:
 *    delete a directory on file system
 *
 *  Inputs:
 *    const char *Path: path to directory
 *
 *  Output:
 *
 **********************************************************************/
void fsu_delete_dir_on_file_system (const char *Path)
{
    if (rmdir(Path) < 0)
    {
        printf("\n(YHCHO) %s() - errorno: %d, %s", __FUNCTION__, errno, Path);

        ASSERT(0);
    }
}


/***********************************************************************
 *  Function: fsu_check_job_queue_file()
 *
 *  Description:
 *    determine whether the job queue file (JobQueue%04X.info) exists
 *
 *  Inputs:
 *    const char *Path: path to job queue file
 *
 *  Output:
 *    TRUE - If exists
 *    FALSE - Do not exist
 *
 **********************************************************************/
BOOLEAN fsu_check_job_queue_file (const char *Path)
{
    BOOLEAN ret_val = FALSE;

    if (access(Path, F_OK) == 0)
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_check_doc_man_dir_in_nand()
 *
 *  Description:
 *    determine whether the /DocMan/DocXXXX/ exists
 *
 *  Inputs:
 *    const char *Path: path to the directory in NAND
 *
 *  Output:
 *    TRUE - If exists
 *    FALSE - Do not exist
 *
 **********************************************************************/
BOOLEAN fsu_check_doc_man_dir_in_nand (const char *Path)
{
    BOOLEAN ret_val = FALSE;

    if (access(Path, F_OK) == 0)
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_check_doc_man_dir_in_ram()
 *
 *  Description:
 *    determine whether the /tmp/DocMan/DocXXXX/ exists
 *
 *  Inputs:
 *    const char *Path: path to the directory in RAM
 *
 *  Output:
 *    TRUE - If exists
 *    FALSE - Do not exist
 *
 **********************************************************************/
BOOLEAN fsu_check_doc_man_dir_in_ram (const char *Path)
{
    BOOLEAN ret_val = FALSE;

    if (access(Path, F_OK) == 0)
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_check_1st_image_file_in_mem()
 *
 *  Description:
 *    check the fax01.jbg or fax01.photo in /DocMan/DocXXXX/
 *
 *  Inputs:
 *    UINT16 jnum: job number to check
 *    UINT8 FileType: type of file
 *
 *  Output:
 *    TRUE - If exists
 *    FALSE - Do not exist
 *
 **********************************************************************/
BOOLEAN fsu_check_1st_image_file_in_mem (UINT16 jnum, UINT8 FileType)
{
    BOOLEAN ret_val = FALSE;
    UINT8 IndexToPage = 1;
    UINT16 DocId = fsu_get_doc_id(jnum);
    char FileName[MAX_LEN_FILE_PATH_NAME] = {0, };

    switch (FileType)
    {
        case FAX_FILE_TYPE_JBG:
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            break;

        case FAX_FILE_TYPE_PBM:
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            break;

        case FAX_FILE_TYPE_RAW:
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.raw", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            break;

        case FAX_FILE_TYPE_PHOTO:
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            break;

        default:
            break;
    }

    if (access(FileName, F_OK) == 0)
    {
        if (isEncodeComplete == 1)
        {
            ret_val = TRUE;
        }
        else
        {
            ret_val = FALSE;
        }
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}

