/*
 ***********************************************************
 *(c) Copyright 2014 - Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 *==========================================================
 **/
/**
 *\file fsu_pcfax.h
 *
 *\brief Common structure for interface between passer and fax.
 *
 **/
 
#ifndef FSU_PCFAX_H_
#define FSU_PCFAX_H_ 
 
#include "agConnectMgr.h" 
#include "error_types.h" // OK : 0, FAIL : -1

#define MAX_NUM_BC     200     // Maximum number of jobs that one broadcast job can have

#define LEN_PHONE_NO     40     // Maximum number of dial

typedef struct _PCFAX_Imgae_Header
{
	int image_width;
	int image_length;
	int bits_per_pixel;
	int page_number;
	int fax_image_quality;
	UINT8 total_pages;
}PCFAX_Image_Header;

typedef struct _PCFAX_Recipient_info
{
    UINT8 recipient_phone_num[LEN_PHONE_NO + 1];
}PCFAX_Recipient_info;

typedef struct _PCFAX_Header
{
	UINT16 header_length;
	UINT8 fax_quality;
	UINT8 fax_original;
	UINT8 recipient_num;
    PCFAX_Recipient_info Recipient[MAX_NUM_BC];
}PCFAX_Header;

// 1. FAX Header 전달
int PCFax_set_pcfax_header(IOReg *Pipe, uint32_t length);

//2. FAX Image Header 전달
int PCFax_set_pcfax_imageheader(IOReg *Pipe, uint32_t length);

//3. FAX Image 전달
int PCFax_set_pcfax_image(IOReg *Pipe, uint32_t length);

//4. Page End
int PCFax_set_pcfax_pageend(void);

//5. Job End
int PCFax_set_pcfax_jobend(void);

// Job Cancel
int PCFax_job_cancel(void);

#endif //FSU_PCFAX_H_


