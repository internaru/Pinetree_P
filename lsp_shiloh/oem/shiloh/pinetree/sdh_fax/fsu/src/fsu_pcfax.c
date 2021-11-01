#include "fsu_pcfax.h"
#include "fax_memAPI.h"
#include "stdio.h"
#include "memory.h"
#include "fsu.h"
#include "SDHFaxCommonHeader.h"
#include "faxapp_param.h"
#include "assert.h"
#include "faxapp_notify.h"
#include "lassert.h"
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#include "../../../../common/scan/apps/scanapp_imagelog.h"
#endif

extern void set_txq_nxt (void);
extern void fsu_make_doc_dir_in_ram(UINT16 DocId);
extern void Fax_API_T30Attribute(UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg);
extern void fsu_update_tx_time (UINT16 jnum, long TimeToTx);
extern void MakeRawToPbm (int CurrentPage, uint8_t *buffer, int wid, int hig, UINT16 jnum, int fax_mode);
extern void fsu_update_total_pages (UINT16 jnum, UINT8 Page);
extern void fsu_update_tx_total_page_no (UINT16 jnum, UINT8 TxTotalPageNo);
extern void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId);
extern void set_tx_job_ready(UINT16 jnum);
extern void fsu_make_image_file_for_broadcast (UINT16 SrcJobNo, UINT16 DstJobNo, UINT8 Page);
extern void fsu_update_resolution (UINT16 jnum, UINT8 res);
extern void fsu_update_doc_id (UINT16 jnum, UINT16 DocId);
extern void fsu_update_dialno (UINT16 jnum, UINT8 *dialno);
extern void fax_rotate(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int margin);
extern void fsu_update_pcfax_job_init (UINT16 jnum);
extern void fsu_update_width (UINT16 jnum, UINT8 width);
extern void fsu_update_retry (UINT16 jnum, UINT8 retry);
extern void fsu_update_retry_tmr (UINT16 jnum, UINT8 retry_tmr);
extern void fsu_update_photo_mode (UINT16 jnum, UINT8 photomode);
extern void fsu_remove_tx_job(UINT16 jnum);
extern void fsu_purge_doc_dir(UINT16 jnum);
extern void fsu_update_original_doc_id (UINT16 jnum, UINT16 DocId);
extern void fsu_update_user_id (UINT16 jnum, char *pUserId);
extern void fsu_update_broadcast (UINT16 jnum, UINT8 broadcast);
extern void faxapp_request_update_user_id (void);
extern void fsu_update_remote_no_for_image_log (UINT8 type, UINT16 jnum);
#ifdef HAVE_IMAGELOG
extern bool IsImageLogEnabled(char jobtype, int subJobType);
#endif
extern UINT8 fsu_get_broadcast (UINT16 jnum);
#ifdef HAVE_IMAGELOG
extern UINT8 *fsu_get_dial_no (UINT16 jnum);
extern UINT32 create_unique_job_id( void );
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
#endif
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern UINT16 fsu_make_doc_dir_in_nand(void);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern UINT32 faxapp_create_shared_mem_for_pcfax(PCFAX_Recipient_info *DialNo, UINT8 recipient_num);
extern int fsu_get_memory_full_status_nand(void);
extern int fsu_get_job_queue_full_status(void);
#ifdef HAVE_IMAGELOG
#ifdef META_END_WITH_JOBID
extern int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid);
#else
extern int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages);
#endif
#endif
extern UINT8 fsu_get_status (UINT16 jnum);

extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern UINT8 isFaxDocInit;
extern char gFaxUserId[MAX_LEN_USER_ID + 1];
extern UINT16 fsu_job_num_scan_image_log;

PCFAX_Header m_pcfax_header;
PCFAX_Image_Header m_pcfax_image_header;
uint8_t *image_buffer = NULL;
uint8_t *sub_image_buffer = NULL;
int image_data_count = 0;
UINT16 fsu_job_num_pcfax = 0;
int need_rotation = 0;
int pcfax_image_width_type = 0; 

#define READ_DATA_STRIP 64
#define A4_WIDTH_VALUE 2592
#define B4_WIDTH_VALUE 3072


UINT8 PCFax_get_no_of_recipient(void)
{
    return (m_pcfax_header.recipient_num);
}


int PCFax_full_check(void)
{
    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FAIL;//FSU_MEMORY_FULL_NAND;
    }

    if (fsu_get_memory_full_status_ram() == 1)
    {
        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FAIL; //FSU_MEMORY_FULL_RAM;
    }

    if (fsu_get_job_queue_full_status() == 1)
    {
        printf("\n(YHCHO) %s() - Job Queue Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FAIL;//FSU_JOBQ_FULL;
    }

    return OK;	
}


int PCFax_job_setting(void)
{
    UINT8 IndexToRecipient = 0;
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT16 DocId = 0;
    UINT16 OriginalDocId = 0;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;

    fsu_job_num_pcfax = txq.nxt;
    printf("\n(YHCHO) %s() - fsu_job_num_pcfax: %d\n", __FUNCTION__, fsu_job_num_pcfax);

    // Step 1. Point to the next txq.nxt for the next job
	set_txq_nxt ();   
	
	ptx_job = &txq.job[fsu_job_num_pcfax];

    // Step 2. Make DocXXXX directory
    // Step 2-1. Make /DocMan/DocXXXX/
    DocId = fsu_make_doc_dir_in_nand();

    // Step 2-2. Make /tmp/DocMan/DocXXXX
    fsu_make_doc_dir_in_ram(DocId);

    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    faxapp_request_update_user_id();

    // Step 3. Store job queue information
    if ( m_pcfax_header.recipient_num == 1 )
	{
		fsu_update_pcfax_job_init(fsu_job_num_pcfax);
		fsu_update_dialno(fsu_job_num_pcfax,&m_pcfax_header.Recipient[0].recipient_phone_num[0]);
        fsu_update_retry(fsu_job_num_pcfax, retry);
        fsu_update_retry_tmr(fsu_job_num_pcfax, retry_tmr);
        fsu_update_resolution(fsu_job_num_pcfax, m_pcfax_header.fax_quality);
        fsu_update_doc_id(fsu_job_num_pcfax, DocId);
        fsu_update_original_doc_id(fsu_job_num_pcfax, DEFAULT_ORIGINAL_DOC_ID);
        fsu_update_photo_mode(fsu_job_num_pcfax, m_pcfax_header.fax_original);
        fsu_update_broadcast(fsu_job_num_pcfax, 0); //not broadcast
        fsu_update_user_id(fsu_job_num_pcfax, gFaxUserId);
    }
    else
    {
        OriginalDocId = DocId;

        for( IndexToRecipient = 0 ; IndexToRecipient < m_pcfax_header.recipient_num; IndexToRecipient++ )
		{
            if( IndexToRecipient != 0 )
			{
				fsu_job_num_pcfax = txq.nxt;
                set_txq_nxt ();
				ptx_job = &txq.job[fsu_job_num_pcfax];
                DocId = fsu_make_doc_dir_in_nand();
                fsu_make_doc_dir_in_ram(DocId);
			}
			
			fsu_update_pcfax_job_init(fsu_job_num_pcfax);
            fsu_update_dialno(fsu_job_num_pcfax,&m_pcfax_header.Recipient[IndexToRecipient].recipient_phone_num[0]);
            fsu_update_retry(fsu_job_num_pcfax, retry);
            fsu_update_retry_tmr(fsu_job_num_pcfax, retry_tmr);
            fsu_update_resolution(fsu_job_num_pcfax, m_pcfax_header.fax_quality);
            fsu_update_doc_id(fsu_job_num_pcfax, DocId);
            fsu_update_original_doc_id(fsu_job_num_pcfax, OriginalDocId);
            fsu_update_photo_mode(fsu_job_num_pcfax, m_pcfax_header.fax_original);
            fsu_update_broadcast(fsu_job_num_pcfax, 1); //set broadcast
            fsu_update_user_id(fsu_job_num_pcfax, gFaxUserId);

            gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient] = fsu_job_num_pcfax;
		}
		fsu_job_num_pcfax = gFaxBroadcastJobInfo[OriginalDocId][0];

        gFax_BC_Num[OriginalDocId].BC_Total_Num = m_pcfax_header.recipient_num;
        gFax_BC_Num[OriginalDocId].BC_Current_Num = 0;
    }

    return OK;
}

// 1. FAX Header 전달
int PCFax_set_pcfax_header(IOReg *Pipe, uint32_t length)
{
	int BytesRead = 0;
	int i = 0, j = 0, temp = 0;
	uint8_t *stream = NULL;
	
	while(1)
	{
		if(isFaxDocInit)
		{
			printf("\n(YHCHO) %s() Fax Doc Init Complete\n", __FUNCTION__);
			break;
		}
		
		usleep(50*1000);
	}
	
	printf("\n(YHCHO) %s() - length: %d\n", __FUNCTION__, length);
	stream = MEM_MALLOC(length);
	if(stream == NULL)
	{
		XASSERT(0,0);
	}
	
	if(PCFax_full_check() == FAIL)
	{
		printf("\n(YHCHO) %s() - PCFax_full!!!!\n", __FUNCTION__);
		MEM_FREE_AND_NULL(stream);
		
		return FAIL;
	}
	
	BytesRead = Pipe->ReadConnect(Pipe, stream, length, PARSEREAD_EXACT);
	if(BytesRead != length)
	{
		printf("\n(YHCHO) %s() - ReadConnect fail: %d\n", __FUNCTION__, BytesRead);
		MEM_FREE_AND_NULL(stream);
		return FAIL;
	}
	printf("\n(YHCHO) %s() - ReadConnect OK: %d\n", __FUNCTION__, BytesRead);
	
	m_pcfax_header.header_length = (stream[1] << 8) | (stream[0] << 0);
	printf("\n(YHCHO) %s() - header_length: %d\n", __FUNCTION__, m_pcfax_header.header_length);
	
	if(stream[3] == 0x11) //Standard
	{
		m_pcfax_header.fax_quality = 0;
	}
	else if(stream[3] == 0x12) //Fine
	{
		m_pcfax_header.fax_quality = 1;
	}
	else if(stream[3] == 0x13) //Super Fine
	{
		m_pcfax_header.fax_quality = 3;
	}
	printf("\n(YHCHO) %s() - fax_quality: %d\n", __FUNCTION__, m_pcfax_header.fax_quality);
	
	if(stream[5] == 0x21) //Text
	{
		m_pcfax_header.fax_original = 0;
	}
	else if(stream[5] == 0x22) //Photo
	{
		m_pcfax_header.fax_original = 1;
	}
	printf("\n(YHCHO) %s() - fax_original: %d\n", __FUNCTION__, m_pcfax_header.fax_original);

	m_pcfax_header.recipient_num = stream[9];
	printf("\n(YHCHO) %s() - recipient_num: %d\n", __FUNCTION__, m_pcfax_header.recipient_num);
	if(m_pcfax_header.recipient_num < 1 || m_pcfax_header.recipient_num > MAX_NUM_BC)
	{
		printf("\n(YHCHO) %s() - invalid recipient_num : %d\n", __FUNCTION__, m_pcfax_header.recipient_num);
		MEM_FREE_AND_NULL(stream);
		return FAIL;
	}
	
	for( i=0; i<m_pcfax_header.recipient_num; i++ )
	{
		temp = 0;
		for( j=0; j<LEN_PHONE_NO; j++ )
		{
			if( stream[10+LEN_PHONE_NO+(i*80)+j] == 0x00 ) // NULL
			{
				if(j != 0)
				{
					break;
				}
			}
			else if ( stream[10+LEN_PHONE_NO+(i*80)+j] == 0x20 ) //Space key
			{
				; //do nothing
			}
           else if ( stream[10+LEN_PHONE_NO+(i*80)+j] == 0x2A 
                   || stream[10+LEN_PHONE_NO+(i*80)+j] == 0x2D ) // '*' or '-' key
			{
				m_pcfax_header.Recipient[i].recipient_phone_num[temp++] = 0x2C; // Pause Key
			}
			else
			{
				m_pcfax_header.Recipient[i].recipient_phone_num[temp++] = stream[10+LEN_PHONE_NO+(i*80)+j];
			}
		}
		m_pcfax_header.Recipient[i].recipient_phone_num[temp] = '\0';
        
		printf("\n(YHCHO) %s() - recipient: %d, %d, phone: %s\n", __FUNCTION__, i, j, m_pcfax_header.Recipient[i].recipient_phone_num);
    }

	MEM_FREE_AND_NULL(stream);
	
	PCFax_job_setting();
	
	return OK;
}

//2. FAX Image Header 전달
int PCFax_set_pcfax_imageheader(IOReg *Pipe, uint32_t length)
{
	int BytesRead = 0;
	int x_size = 0, y_size = 0;
	uint8_t *stream = NULL;
	UINT8 IndexToRecipient = 0;
    UINT16 OriginalDocId = 0;

	if(fsu_get_status(fsu_job_num_pcfax) != FSU_STATE_SCANNING)
	{
		printf("\n(YHCHO) %s() - omit pcfax setting\n", __FUNCTION__);
		return FAIL;
	}

	printf("\n(YHCHO) %s() - length: %d\n", __FUNCTION__, length);
	stream = MEM_MALLOC(length);
	if(stream == NULL)
	{
		XASSERT(0,0);
	}
		
	BytesRead = Pipe->ReadConnect(Pipe, stream, length, PARSEREAD_EXACT);
	if(BytesRead != length)
	{
		printf("\n(YHCHO) %s() - ReadConnect fail: %d\n", __FUNCTION__, BytesRead);
		MEM_FREE_AND_NULL(stream);
		PCFax_job_cancel();
		return FAIL;
	}
	printf("\n(YHCHO) %s() - ReadConnect OK: %d\n", __FUNCTION__, BytesRead);
	
	//m_pcfax_image_header.magic_number;
	//printf("\n(YHCHO) %s() - magic_number: %x\n", __FUNCTION__, m_pcfax_image_header.magic_number);
	
	//m_pcfax_image_header.header_size;
	//printf("\n(YHCHO) %s() - header_size: %d\n", __FUNCTION__, m_pcfax_image_header.header_size);
	
	m_pcfax_image_header.image_width = (stream[9] << 8) + stream[8];
	printf("\n(YHCHO) %s() - image_width: %d\n", __FUNCTION__, m_pcfax_image_header.image_width);	

	m_pcfax_image_header.image_length = (stream[13] << 8) + stream[12];
	printf("\n(YHCHO) %s() - image_length: %d\n", __FUNCTION__, m_pcfax_image_header.image_length);
	
	m_pcfax_image_header.bits_per_pixel = stream[16];
	printf("\n(YHCHO) %s() - bits_per_pixel: %d\n", __FUNCTION__, m_pcfax_image_header.bits_per_pixel);	
	
	m_pcfax_image_header.page_number = stream[20];
	printf("\n(YHCHO) %s() - page_number: %d\n", __FUNCTION__, m_pcfax_image_header.page_number);

	faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_FROM_PC, m_pcfax_image_header.page_number, 0, 0, 0, fsu_job_num_pcfax);

	//m_pcfax_image_header.fax_image_quality;
	//printf("\n(YHCHO) %s() - fax_image_quality: %d\n", __FUNCTION__, m_pcfax_image_header.fax_image_quality);

	m_pcfax_image_header.total_pages = stream[28];
	printf("\n(YHCHO) %s() - total_pages: %d\n", __FUNCTION__, m_pcfax_image_header.total_pages);

	//m_pcfax_image_header.reserve_1;
	//printf("\n(YHCHO) %s() - reserve_1: %d\n", __FUNCTION__, m_pcfax_image_header.reserve_1);
	
	//m_pcfax_image_header.reserve_2;
	//printf("\n(YHCHO) %s() - reserve_2: %d\n", __FUNCTION__, m_pcfax_image_header.reserve_2);

	if(image_buffer != NULL)
	{
		printf("\n(YHCHO) %s() image_buffer NOT NULL!!\n", __FUNCTION__);
		MEM_FREE_AND_NULL(image_buffer);
	}

	need_rotation = 0;
	pcfax_image_width_type = 0;
	x_size = A4_WIDTH_VALUE;
	y_size = m_pcfax_image_header.image_length;
	if(m_pcfax_image_header.image_width < m_pcfax_image_header.image_length) // check rotation
	{
		if(m_pcfax_image_header.image_length <= A4_WIDTH_VALUE) //Rotation, A4 Width
		{
			need_rotation = 1;
			pcfax_image_width_type = 0;
			x_size = A4_WIDTH_VALUE;
			y_size = m_pcfax_image_header.image_width;
			sub_image_buffer = MEM_MALLOC(m_pcfax_image_header.image_width * m_pcfax_image_header.image_length);
			if(sub_image_buffer == NULL)
			{
				XASSERT(0,0);
			}
		}
		else if(m_pcfax_image_header.image_length <= B4_WIDTH_VALUE) //Rotation, B4 Width
		{
			need_rotation = 1;
			pcfax_image_width_type = 1;
			x_size = B4_WIDTH_VALUE;
			y_size = m_pcfax_image_header.image_width;
			sub_image_buffer = MEM_MALLOC(m_pcfax_image_header.image_width * m_pcfax_image_header.image_length);
			if(sub_image_buffer == NULL)
			{
				XASSERT(0,0);
			}
			
			if(m_pcfax_header.fax_original == 1)
			{
				m_pcfax_header.fax_original = 0;
				
				if ( m_pcfax_header.recipient_num == 1 ) // 1 recipient
				{
					fsu_update_photo_mode(fsu_job_num_pcfax, m_pcfax_header.fax_original);
				}
				else if(m_pcfax_header.recipient_num > 1) // broadcast
				{
                    OriginalDocId = fsu_get_original_doc_id(fsu_job_num_pcfax);

					for( IndexToRecipient = 0 ; IndexToRecipient < m_pcfax_header.recipient_num; IndexToRecipient++ )
					{	
                        fsu_update_photo_mode(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], m_pcfax_header.fax_original);
					}
				}				
			}
		}
	}
	printf("\n(YHCHO) %s() rotation:%d, width_type:%d\n", __FUNCTION__,need_rotation,pcfax_image_width_type);
	
	image_buffer = MEM_MALLOC(x_size * y_size);
	if(image_buffer == NULL)
	{
		XASSERT(0,0);
	}
	memset(image_buffer,0,x_size * y_size);
	image_data_count = 0;
	
	MEM_FREE_AND_NULL(stream);
	
	return OK;
}

//3. FAX Image 전달
int PCFax_set_pcfax_image(IOReg *Pipe, uint32_t length)
{
	int BytesRead = 0;
	int i = 0, j = 0;
	int margin = (A4_WIDTH_VALUE - m_pcfax_image_header.image_width)/2;
	int strip = READ_DATA_STRIP*m_pcfax_image_header.image_width;
	int total_strip_count=(length/strip);
	int last_strip = length % strip;

	uint8_t *stream = NULL;

	printf("\n(YHCHO) %s() - length: %d\n", __FUNCTION__, length);
	stream = MEM_MALLOC(strip);
	if(stream == NULL)
	{
		XASSERT(0,0);
	}
	
	printf("\n(YHCHO) %s() - START: %d\n", __FUNCTION__,total_strip_count);
	for(i=0;i<total_strip_count;i++)
	{	
		BytesRead = Pipe->ReadConnect(Pipe, stream, strip, PARSEREAD_EXACT);
		if(BytesRead != strip)
		{
			printf("\n(YHCHO) %s() - ReadConnect fail1: %d\n", __FUNCTION__, BytesRead);
			MEM_FREE_AND_NULL(stream);
			MEM_FREE_AND_NULL(image_buffer);
			if(need_rotation == 1)
			{
				MEM_FREE_AND_NULL(sub_image_buffer);
			}
			PCFax_job_cancel();
			
			return FAIL;			
		}
		else if(BytesRead < 0)
		{
			printf("\n(YHCHO) %s() - ReadConnect fail2: %d\n", __FUNCTION__, BytesRead);
			MEM_FREE_AND_NULL(stream);
			MEM_FREE_AND_NULL(image_buffer);
			if(need_rotation == 1)
			{
				MEM_FREE_AND_NULL(sub_image_buffer);
			}
			PCFax_job_cancel();
			
			return FAIL;
		}

		//printf("\n(YHCHO) %s() - ReadConnect ok: %d\n", __FUNCTION__, BytesRead/m_pcfax_image_header.image_width);
		if(need_rotation == 0)
		{
			for(j=0; j< BytesRead/m_pcfax_image_header.image_width; j++)
			{
				memcpy(&image_buffer[image_data_count+margin],&stream[j*m_pcfax_image_header.image_width],m_pcfax_image_header.image_width);
				image_data_count += A4_WIDTH_VALUE;
			}
		}
		else
		{
			memcpy(&sub_image_buffer[image_data_count],&stream[0],BytesRead);
			image_data_count += BytesRead;
		}
	}
	
	printf("\n(YHCHO) %s() - read OK : %d, last_strip : %d\n", __FUNCTION__,image_data_count,last_strip);
	if(last_strip)
	{
		BytesRead = Pipe->ReadConnect(Pipe, stream, last_strip, PARSEREAD_EXACT);
		if(BytesRead != last_strip)
		{
			printf("\n(YHCHO) %s() - ReadConnect fail3: %d\n", __FUNCTION__, BytesRead);
			MEM_FREE_AND_NULL(stream);
			MEM_FREE_AND_NULL(image_buffer);
			if(need_rotation == 1)
			{
				MEM_FREE_AND_NULL(sub_image_buffer);
			}
			PCFax_job_cancel();
			
			return FAIL;			
		}
		else if(BytesRead < 0)
		{
			printf("\n(YHCHO) %s() - ReadConnect fail4: %d\n", __FUNCTION__, BytesRead);
			MEM_FREE_AND_NULL(stream);
			MEM_FREE_AND_NULL(image_buffer);
			if(need_rotation == 1)
			{
				MEM_FREE_AND_NULL(sub_image_buffer);
			}
			PCFax_job_cancel();
			
			return FAIL;
		}

		//printf("\n(YHCHO) %s() - ReadConnect ok: %d\n", __FUNCTION__, BytesRead/m_pcfax_image_header.image_width);
		if(need_rotation == 0)
		{
			for(j=0; j< BytesRead/m_pcfax_image_header.image_width; j++)
			{
				memcpy(&image_buffer[image_data_count+margin],&stream[j*m_pcfax_image_header.image_width],m_pcfax_image_header.image_width);
				image_data_count += 2592;
			}
		}
		else
		{
			memcpy(&sub_image_buffer[image_data_count],&stream[0],BytesRead);
			image_data_count += BytesRead;
		}			
	}
	
	printf("\n(YHCHO) %s() - read OK : %d\n", __FUNCTION__,image_data_count);
	
	MEM_FREE_AND_NULL(stream);
	
	return OK;
}

//4. Page End
int PCFax_set_pcfax_pageend(void)
{
	int margin = 0;
	int width = A4_WIDTH_VALUE;
	int height = m_pcfax_image_header.image_length;
    int OriginalDocId = 0;
	int index = 0;

	printf("\n(YHCHO) %s()\n", __FUNCTION__);
	
	if(need_rotation)
	{
		if(pcfax_image_width_type == 0)
		{
			width = A4_WIDTH_VALUE;
			height = m_pcfax_image_header.image_width;
			margin = (A4_WIDTH_VALUE - m_pcfax_image_header.image_length)/2;
			fax_rotate(sub_image_buffer, image_buffer, A4_WIDTH_VALUE, m_pcfax_image_header.image_width, m_pcfax_image_header.image_length, margin);
		}
		else if(pcfax_image_width_type == 1)
		{
			width = B4_WIDTH_VALUE;
			height = m_pcfax_image_header.image_width;
			margin = (B4_WIDTH_VALUE - m_pcfax_image_header.image_length)/2;
			fax_rotate(sub_image_buffer, image_buffer, B4_WIDTH_VALUE, m_pcfax_image_header.image_width, m_pcfax_image_header.image_length, margin);
		}
		
		MEM_FREE_AND_NULL(sub_image_buffer);
	}


	for(index = 0; index < width*height; index++)
	{
		image_buffer[index] = 255 - image_buffer[index];
	}

#ifdef HAVE_IMAGELOG
    // Save the remote no. once and make .pdf file for image log
    if (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE)
    {
        if (m_pcfax_image_header.page_number == 1)
        {
			UINT32 jobId = create_unique_job_id();
			fsu_update_unique_job_id(fsu_job_num_pcfax, jobId);

            fsu_update_remote_no_for_image_log(IMAGE_LOG_JOB_TYPE_TX, fsu_job_num_pcfax);

            set_metafile_start_info('F', IMAGE_LOG_JOB_TYPE_TX, fsu_get_unique_job_id(fsu_job_num_pcfax), gFaxUserId);
        }

		uint8_t *imagelog_page_data;
		unsigned long malloced_pageBuf_size = 0;
		unsigned int malloced_pageBuf_WidPixel = 0;
		unsigned int malloced_pageBuf_HeiPixel = 0;
		unsigned int pdf_width = 0;
		unsigned int pdf_height = 0;

		malloced_pageBuf_size = width * height;	//bpp:8 (if depended user setting, have to change)
		malloced_pageBuf_WidPixel = width;
		malloced_pageBuf_HeiPixel = height;

		imagelog_page_data = (uint8_t *)MEM_MALLOC(malloced_pageBuf_size);
		memset(imagelog_page_data, 0xFF, malloced_pageBuf_size);
		memcpy(imagelog_page_data, image_buffer, malloced_pageBuf_size);

		pdf_width = malloced_pageBuf_WidPixel/CONVERT_400DPI;
		pdf_height = malloced_pageBuf_HeiPixel/CONVERT_400DPI;

		fsu_job_num_scan_image_log = fsu_job_num_pcfax;

		if(imagelog_page_data != NULL) {
			SCAN_IL_PDF_Start( imagelog_page_data, malloced_pageBuf_size,
									malloced_pageBuf_WidPixel, malloced_pageBuf_HeiPixel,
									pdf_width, pdf_height,
									(int)(m_pcfax_image_header.page_number - 1),
									IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM,
									IMAGELOG_APPLICATION_TYPE_OF_SCAN_FAX_SCAN
								);
		}
		else {
			printf("\n(PC Fax Scan) ImageLog Turn-On. But PageMem is NULL\n");
		}

		MEM_FREE_AND_NULL(imagelog_page_data);
    }
#endif
	
    if (fsu_get_broadcast(fsu_job_num_pcfax) == 1)
    {
        OriginalDocId = fsu_get_original_doc_id(fsu_job_num_pcfax);

        MakeRawToPbm (m_pcfax_image_header.page_number, image_buffer, width, height, gFaxBroadcastJobInfo[OriginalDocId][0], REQUEST_SOURCE_PC);
    }
    else
    {
        MakeRawToPbm (m_pcfax_image_header.page_number, image_buffer, width, height, fsu_job_num_pcfax, REQUEST_SOURCE_PC);
    }

	MEM_FREE_AND_NULL(image_buffer);

    fsu_get_memory_full_status_nand();

	return OK;
}

//5. Job End
int PCFax_set_pcfax_jobend(void)
{
    UINT8 width = (UINT8)pcfax_image_width_type;
    UINT8 TotalRecipient = m_pcfax_header.recipient_num;
    UINT8 IndexToRecipient = 0;
    UINT16 jnum = fsu_job_num_pcfax;
    UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);
    UINT32 SharedMemID = 0;
    
	printf("\n(YHCHO) %s()\n", __FUNCTION__);

    fsu_get_memory_full_status_nand();

	SharedMemID = faxapp_create_shared_mem_for_pcfax(&m_pcfax_header.Recipient[0], TotalRecipient);

    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PCFAX_DIAL_NO, 0, TotalRecipient, 0, SharedMemID, jnum);

	if (TotalRecipient == 1)
	{
        fsu_update_width (jnum, width);

        fsu_update_total_pages(jnum, m_pcfax_image_header.total_pages);

        fsu_update_tx_total_page_no(jnum, m_pcfax_image_header.total_pages);

        Create_Job_Queue_File(jnum, FAX_APP_JOB_TYPE_TX_IMMEDIATE, fsu_get_doc_id(jnum));
	}
    else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
	{
		for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
		{
            if (IndexToRecipient != 0)
            {
                fsu_make_image_file_for_broadcast(gFaxBroadcastJobInfo[OriginalDocId][0], gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], m_pcfax_image_header.total_pages);
            }

            fsu_update_width (gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], width);

            fsu_update_total_pages(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], m_pcfax_image_header.total_pages);

            fsu_update_tx_total_page_no(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], m_pcfax_image_header.total_pages);

            Create_Job_Queue_File(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], FAX_APP_JOB_TYPE_BROADCAST, fsu_get_doc_id(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]));

            if (IndexToRecipient != 0)
            {
                faxapp_notify_SendPCFaxJobID(gFaxBroadcastJobInfo[OriginalDocId][0], IndexToRecipient, gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
            }
		}
	}

    usleep(500*1000); // To give UI enough time to process..

    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_COMPLETE, 0, 0, 0, 0, jnum);

    if (TotalRecipient == 1)
    {
        set_tx_job_ready(jnum);
    }
    else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
    {
        for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
        {
            set_tx_job_ready(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
        }
    }
	
	return OK;
}

int PCFax_job_cancel(void)
{
	UINT8 TotalRecipient = m_pcfax_header.recipient_num;
	UINT8 IndexToRecipient = 0;
    UINT16 jnum = fsu_job_num_pcfax;
    UINT16 OriginalDocId = 0;
	
	printf("\n(YHCHO) %s()\n", __FUNCTION__);
	
    if ( TotalRecipient == 1 )
	{
        fsu_purge_doc_dir(jnum);
        fsu_remove_tx_job(jnum);
    }
    else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
	{
        OriginalDocId = fsu_get_original_doc_id(jnum);

		for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
		{
            fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
            fsu_remove_tx_job(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
		}
	}
	
    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_FAIL, 0, 0, 0, 0, jnum);
	
	return OK;
}


