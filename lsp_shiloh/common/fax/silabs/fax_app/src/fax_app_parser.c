/*
 * ============================================================================
 * Copyright (c) 2008   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * faxer_app_parser.c
 * 
 * This file includes the routines that implements the fax/PC protocol.
 *
 *  Created on: Dec 1, 2008
 *      Author: ericyh
 */
#include <string.h>
#include <dprintf.h>

#include "agRouter.h"
#include "Atypes.h"
#include "agConnectMgr.h"
#include "agMessage.h"
#include "memAPI.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "lassert.h"
#include "rtcAPI.h"
#include "datastore.h"
#include "agStatusMgrAPI.h"

#include "memAPI.h"

#include "FS_wrapper.h"
#include "agiLogger.h"

#include "faxer.h"
#include "faxer_phonebk.h"
#include "faxer_config.h"
#include "app_job.h"
#include "faxer_app_parser.h"
#include "faxer_sys.h"
#include "fax/fax_api.h"

#define FAX_SIG 0x4d465031
#define FAX_BUFFER_SIZE 4096

typedef enum
{
    e_StartFaxJob,
    e_EndFaxJob,
    e_SendJob,
    e_EnumerateFaxes,
    e_GetFax,
    e_GetFaxLogEntry,
    e_FaxReceived,
    e_CheckFaxPasswordSet,
    e_CheckFaxPassword,
    e_GetDefaultFaxSettings,
    e_SetDefaultFaxSettings,
    e_ClearFaxStatus,
    e_RequestFaxStatus,
    e_FaxDataBlock,
    e_GetFaxRecvSettings
} FAX_MESSAGE_ENUM;

typedef enum
{
    e_Success,
    e_Fail,
    e_Busy,
    e_CmdNotRecognized,
    e_InvalidArg,
    e_InvalidPath,
    e_DiskFull
} FAX_RESPONSE_ENUM;

typedef struct
{
    char Prefix[32];
    char PhoneNumber[32];   // TODO -- should this allow as high as 50?
} FAX_RECIPIENT;

typedef struct
{
    UINT32 PhoneNumberCount;
    FAX_RECIPIENT Numbers[1];
} FAX_JOB_SETTINGS;


typedef struct
{
    UINT32 Signature; // Includes version information.
    UINT32 Message; // One of FAX_MESSAGE_ENUM
    SINT32 Param1; // Used for passing message parameters
    SINT32 Param2; // and status.
    FAX_RESPONSE_ENUM Status;
    UINT32 DataLength; // The amount of data following
    // the message block (including the
    // FAX_DATA_HEADER which must precede
    // the data).
    UINT32 Reserved1;
    UINT32 Reserved2;
} FAX_MESSAGE_HEADER;

typedef struct
{
    UINT32 Signature;
    UINT32 Status;
    UINT32 Param1;
    UINT32 Param2;
} FAX_STATUS_TYPE;

typedef struct
{
    char  Password[32]; // Only valid on set
    char  Prefix[32];
    char  OutsideLineAccess[32];
    char  FaxName[128];
    char  FaxNumber[32];
    int RingPattern;
    BOOL  PulseDialing; // 1 if pulse dialing, 0 otherwise
    BOOL  bThumbFirst;
    int PrintFaxReport;
    int  AutoAnswer;
    char  DeviceDateTime[32];
} DEFAULT_FAX_SETTINGS;
/* default settings */


typedef struct faxlog_rtcTime_s
{
    uint8_t	    hour;		/// 0-23 hours
    uint8_t	    min;		/// 0-59 minutes
    uint8_t	    sec;		/// 0-59 seconds
    uint8_t     DOW;		/// day of week 1-7
    uint8_t	    day;		/// 0-30 days since beginning of month
    uint8_t     month;		/// 0-11 months since January
    // Note: This differs from the rtcTime_s definition in the BSP libraries
    // because the documentation for the fax protocol specified a 1-byte year
    uint8_t	    year;		/// 0-99 years since 2000
} faxlog_rtcTime_t; 



/** Note: This is structure mimics the FAX_LOG_ENTRY
 *  structure that is used to store the logs in the file
 *  system.  This one, though, is passed to the host via a
 *  defined API.  It should not be changed without notifying
 *  the software developers of the necessary changes. */
#define LOG_FILENAME_SIZE 35

typedef struct
{
    faxlog_rtcTime_t   SendTime;             ///< The time and day of this fax.
    UINT32      Duration;                    ///< The duration in seconds of this fax.
    UINT32      FaxStatus;                   ///< The status of this fax.
    UINT32      FaxType;                     ///< The type of fax.
    UINT8       Pages;                       ///< Number of pages send or received in this fax.
    UINT8       Thumbnail;                   ///< Set if a Thumbnail of fax is available.
    char        CIS[20];                     ///< CIS storage. (** This should be CIS_SIZE, but the software is out of sync for now **)
    char        FileName[LOG_FILENAME_SIZE]; ///< If a thumbnail, the file name.
} FAX_LOG_MESSAGE;

typedef enum
{
    e_RecvModePrint,    // Faxes will be printed when received
    e_RecvModeForward,  // Received faxes will be forwarded to another number
    e_RecvModeHost,     // Fax should be received to host PC
    e_RecvModePrivate,  // Fax will be stored on printer only
} FAX_RECEIVE_MODE;

static IOReg *Pipe;
static BOOL do_fax_parsing(IOReg *, BOOL );

extern int sscanf(const char *s, const char *format, ...);

/* FUNCTION NAME: faxer_app_parser_report_status */
 
/** 
 * \brief Report the status of the fax to the host.  
 * This calls a function that will report to all fax endpoints and ports the
 * status that is passed in.
 * 
 * \param Message The message to send to the host.
 * 
 * \retval None
 * 
 * \author Dirk Bodily
 * 
 * \date 3/12/2007
 * 
 **/
void faxer_app_parser_report_status(FAXER_APP_STATUS_ENUM status_id)
{
    FAX_STATUS_TYPE *FaxStatus;

    while((FaxStatus = MEM_CALLOC(sizeof(FAX_STATUS_TYPE), 1)) == NULL)
        posix_sleep_ticks(1);
    
    FaxStatus->Status = status_id;
    FaxStatus->Signature = FAX_SIG;
    // Send the status.
    SendIntStatus(e_FaxChan, FaxStatus, sizeof(FAX_STATUS_TYPE));
}

/* FUNCTION NAME: SendFaxStatus */
 
/** 
 * \brief Format and send a status response to a fax message.
 * 
 * \param Pipe
 * 
 * \param ReadHeader
 * 
 * \param Status
 * 
 * \retval None
 * 
 * \author Dirk Bodily
 * 
 * \date 3/12/2007
 * 
 **/
static void send_fax_status(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader, FAX_RESPONSE_ENUM Status)
{
    FAX_MESSAGE_HEADER *pFaxHeader;

    while((pFaxHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
        posix_sleep_ticks(1);
    
    pFaxHeader->Message = pReadHeader->Message;
    pFaxHeader->Signature = pReadHeader->Signature;
    pFaxHeader->Status = Status;

    Pipe->WriteConnect(Pipe, pFaxHeader, sizeof(FAX_MESSAGE_HEADER));

}

static void eat_all()
{
     char *tempDump = 0; 
     
     tempDump= MEM_MALLOC(100);
     if(!tempDump)
     {
    	 dbge("Fail to obtain memory! Abort!\n");
    	 return;
     }
     //eat all data
     while(Pipe->ReadConnect(Pipe, (void *)tempDump, 100, PARSEREAD_DELAY)>0);
     MEM_FREE_AND_NULL(tempDump);
} 


static int process_send_job_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{
    char *DataBuffer;
	char *tmpS1 = NULL;
    char *tmpS2 = NULL;
	PHONEBK_ENTRY *pEntry1, *pEntry2;
	int BytesRead, Size, MyStrLen;
	FAX_JOB_SETTINGS *FaxJobSettings = NULL;
	char TifFile[50];
    FILE *FilePtr;
    // info about the data they sent
    RDR_IMAGE_INFO rdr_image_info;	
    RDR_IMG_TYPE   rdr_image_type;

    // 
    // Now read in the fax settings.
    //
    while((FaxJobSettings = MEM_MALLOC(pReadHeader->DataLength)) == NULL)
        posix_sleep_ticks(1);

    BytesRead = Pipe->ReadConnect(Pipe, (void *)FaxJobSettings, pReadHeader->DataLength,PARSEREAD_EXACT);
    if( (BytesRead != pReadHeader->DataLength) || (FaxJobSettings->PhoneNumberCount< 1))
    {
       if(BytesRead== pReadHeader->DataLength)
           eat_all();

        send_fax_status(Pipe, pReadHeader, e_InvalidArg);
        MEM_FREE_AND_NULL(FaxJobSettings);        // return allocated memory
        dbge("DataLength invalid or no phone number found!\n");
        return -1;       // quit parsing.
    }
    /*
     * Try to obtain fax number(s)
     * 
     */
    tmpS1   = MEM_MALLOC(1024);
    pEntry1 = MEM_MALLOC(sizeof(PHONEBK_ENTRY));
    pEntry2 = MEM_MALLOC(sizeof(PHONEBK_ENTRY));
    
    tmpS1[0]='\0';
    
    dbg("# phone numbers %d\n",FaxJobSettings->PhoneNumberCount);
    for(Size = 0; Size < FaxJobSettings->PhoneNumberCount; Size ++)
    {
        // Now figure out what kind of number this is and build the control file.
        if(strncmp(FaxJobSettings->Numbers[Size].PhoneNumber, PHBKENTRY, 8) == 0)
        {
            sscanf(&(FaxJobSettings->Numbers[Size].PhoneNumber[8]),"%d",&MyStrLen);
            if(faxer_phonebk_get_entry(MyStrLen, pEntry1))
            {
            	dbge("No entry exists for this index (%d)\n", MyStrLen);
            	break;
            }
            if(pEntry1->type==e_SpeedDial)
            {
            	tmpS2 = SPEEDDIAL_GET_NUMBER(pEntry1);
            	if(tmpS1[0]==0)
            		strcpy(tmpS1, tmpS2);
            	else
            	{
            		strcat(tmpS1,";");
            		strcat(tmpS1,tmpS2);
            	}
                tmpS2 = NULL;
            }
            else
            {
            	int i = 0;
            	while((MyStrLen = GROUP_GET_MEMBER_ENTRY(pEntry1,i)))
            	{
            		 if(faxer_phonebk_get_entry(MyStrLen, pEntry2))
            		 {
            			 dbge("No entry exists for index (%d)\n", MyStrLen);
            			 continue;
            		 }
            		 if(pEntry2->type==e_SpeedDial)
            		 {
            		 	tmpS2 = SPEEDDIAL_GET_NUMBER(pEntry1);
            		    if(tmpS1[0]==0)
							strcpy(tmpS1, tmpS2);
						else
						{
								strcat(tmpS1,";");
								strcat(tmpS1,tmpS2);
						}
                        tmpS2 = NULL;
            		 }
            		 i++;
				}
            }
        }
        else
        {
        	 tmpS2 = MEM_MALLOC(100);
        	 tmpS2[0]='\0';
             if(!strncmp(FaxJobSettings->Numbers[Size].Prefix, "DEFAULT", 7))
             {
        	     //Do Nothing //Use system default prefix
        	 }else if(!strncmp(FaxJobSettings->Numbers[Size].Prefix, "NONE", 4))
        	 {
        		 strcpy(tmpS2,":"); //":xxxxxxx" Means no prefix at all
        	 }else
         	 {
        	     strncpy(tmpS2, FaxJobSettings->Numbers[Size].Prefix, 50);
        	     strcat(tmpS2, ":"); //"ppp:"  p for prefix
             }
             
        	 if(tmpS1[0]==0)
        		strcpy(tmpS1, tmpS2);
        	else
        	{ 
        		strcat(tmpS1, ";");
        		strcat(tmpS1, tmpS2);
        	}
     		strcat(tmpS1, FaxJobSettings->Numbers[Size].PhoneNumber);
            db("The send fax is for number %s\n",FaxJobSettings->Numbers[Size].PhoneNumber);

            MEM_FREE_AND_NULL(tmpS2);
        }
    }

    MEM_FREE_AND_NULL(FaxJobSettings);
    MEM_FREE_AND_NULL(pEntry1);
    MEM_FREE_AND_NULL(pEntry2);

    if(!strlen(tmpS1))
    {
    	dbge("No phone number is given. Abort!\n");
        eat_all();
        pReadHeader->Param1 = Size;         //Store the id of invalid number
        send_fax_status(Pipe, pReadHeader, e_InvalidArg);
        MEM_FREE_AND_NULL(tmpS1);
        return -1;       // quit parsing.
    }
    
    /*
     *  Now try to obtain TIF file data
     */

    Size = pReadHeader->DataLength;
    
    faxer_get_free_tif_file_name(TifFile);
    
    //
    // Ready to go, open the file and write away!
    //
    db("PCFAX: Fax file name %s\n",TifFile);
    FilePtr = xt_fopen(TifFile, "w");
    if(FilePtr == NULL)
    {
    	send_fax_status(Pipe, pReadHeader, e_Fail);
        MEM_FREE_AND_NULL(tmpS1);
        return -1;
    }

    while(1)
    {
        BytesRead = Pipe->ReadConnect(Pipe, pReadHeader, sizeof(FAX_MESSAGE_HEADER),PARSEREAD_EXACT);
        if(BytesRead != sizeof(FAX_MESSAGE_HEADER))
        {
            send_fax_status(Pipe, pReadHeader, e_Fail);
            xt_fclose(FilePtr);
            rm(TifFile);   
            return -1;
        }
        db("PCFAX: Data Block Length %d\n",pReadHeader->DataLength);
        if(pReadHeader->Status == e_Fail)
        {
            // We need to abort this, do that here.
            send_fax_status(Pipe, pReadHeader, e_Fail);
            xt_fclose(FilePtr);
            rm(TifFile);   
            return 1;        // return 1 since we did not fail, the host did.
        }
        if(pReadHeader->DataLength == 0)
            {
                pReadHeader->Message = e_SendJob;
                send_fax_status(Pipe, pReadHeader, e_Success);
                break;
            }
        while((DataBuffer = MEM_MALLOC(pReadHeader->DataLength)) == NULL)
            posix_sleep_ticks(1); // wait for memory

        BytesRead = Pipe->ReadConnect(Pipe, DataBuffer, pReadHeader->DataLength, PARSEREAD_EXACT);
        if(BytesRead != pReadHeader->DataLength)
        {
            send_fax_status(Pipe, pReadHeader, e_Fail);
            MEM_FREE_AND_NULL(DataBuffer);
            xt_fclose(FilePtr);
            rm(TifFile);   
            return -1;
        }
        
        BytesRead = xt_fwrite(DataBuffer, 1, pReadHeader->DataLength, FilePtr);

        MEM_FREE_AND_NULL(DataBuffer);     // done with this, return it.

        if(BytesRead != pReadHeader->DataLength)
        {
            dbge("Failed in writing Data Block Length %d\n",BytesRead);
            send_fax_status(Pipe, pReadHeader, e_Fail);
            xt_fclose(FilePtr);
            rm(TifFile);   
            MEM_FREE_AND_NULL(tmpS1);
            return -1;
        }else
        {
            db("Write Data Block Length %d\n",BytesRead);
        }
        posix_sleep_ticks(1);     //Give other threads chance to run
    }
    xt_fclose(FilePtr);

    // get input file attributes, put in rdr_image_info. set input file type in rdr_image_type
    RDR_Get_File_Attributes(TifFile, &rdr_image_info, &rdr_image_type, 0);
    faxer_sys_post_fax_event(FAXER_EVT_MSG_PAGES_SENT_FROM_PC, rdr_image_info.page_count);

    faxer_schedule_fax_send_file(0, tmpS1, TifFile);
    
    MEM_FREE_AND_NULL(tmpS1);
    
    return 0;
}

static int process_fax_enumeration_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{
	FAX_MESSAGE_HEADER *StatusHeader;
	char *DataBuffer, *Ptr;
	int i, MyStrLen,BytesRead,AmtToProcess;
	char TifFile[50];
	unsigned int job_id;
	
	while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER), 1)) == NULL)
        posix_sleep_ticks(1);
        	
	// Initialize the response header.
	StatusHeader->Signature = pReadHeader->Signature;
	StatusHeader->Message = pReadHeader->Message;
	StatusHeader->Status = e_Success;
	BytesRead = 0;
	AmtToProcess = 100;     // the size of the buffer at the beginning.
			
	while((DataBuffer = MEM_CALLOC(AmtToProcess, 1)) == NULL) 
      posix_sleep_ticks(1);
				
	i=0;
	while(!faxer_get_pending_received_fax(i++, &job_id, TifFile))
	{
		 MyStrLen = strlen(TifFile)+1;
		 if(MyStrLen % 2)
			  MyStrLen++;     // make sure we have it in 16 bit chunks.
						
		 // See if we have enough room for the file name.
		 if((AmtToProcess - MyStrLen - BytesRead - (int)sizeof(UINT16)) < 0)
		 {
			// We have run out of memory, get more
			AmtToProcess += 100;    // get another bytes.
			while((Ptr = MEM_REALLOC(DataBuffer, AmtToProcess)) == NULL)
              posix_sleep_ticks(1);
			
			DataBuffer = Ptr;       // update the pointer.
		}
		//
		// Now put the name into the data buffer.
		//
		((UINT16 *)DataBuffer)[BytesRead/2] = MyStrLen; // save the str len
		BytesRead += 2;     // space past the length field
		strcpy(&DataBuffer[BytesRead], TifFile);
		BytesRead += MyStrLen;
		
	}
 
	//
	// Now add a null length field. To terminate the list.
	//
	if((AmtToProcess - 2) < 0)
	{
		//
		// We need more memory for the null field.  Do that here.
		AmtToProcess += 2;
		while((Ptr = MEM_REALLOC(DataBuffer, AmtToProcess)) == NULL)
          posix_sleep_ticks(1);

		DataBuffer = Ptr;
	}
	((UINT16 *)DataBuffer)[BytesRead/2] = 0;    // terminate the list
	// Put in the final list length.
	StatusHeader->DataLength = AmtToProcess;
	// Write out the header.
	Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));
	// Now write the data.
	Pipe->WriteConnect(Pipe, DataBuffer, AmtToProcess);
	
	return 0;
}

static int process_get_fax_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{
	FAX_MESSAGE_HEADER *StatusHeader;
	char *DataBuffer;
	int BytesRead,AmtToProcess, Size;
	FILE *FilePtr;
	
	 // Get memory for the file name
	if(pReadHeader->DataLength == 0)
	{
		send_fax_status(Pipe, pReadHeader, e_Fail);
		return -1;
	}
	 //
	 // Get memory for the file name.
	 //
	while((DataBuffer = MEM_MALLOC(pReadHeader->DataLength + 1)) == NULL)
	  posix_sleep_ticks(1);     // wait for a buffer.
	//
	// Get the filename requested.
	//
	BytesRead = Pipe->ReadConnect(Pipe, DataBuffer, pReadHeader->DataLength,PARSEREAD_EXACT);
	if(BytesRead != pReadHeader->DataLength)
	{
		MEM_FREE_AND_NULL(DataBuffer);
		send_fax_status(Pipe, pReadHeader, e_Fail);
		return -1;
	}
	DataBuffer[pReadHeader->DataLength] = '\0';

    // Open the requested file and return if it fails.
	FilePtr = xt_fopen(DataBuffer, "r");
	if(FilePtr == NULL)
	{
		MEM_FREE_AND_NULL(DataBuffer);
		send_fax_status(Pipe, pReadHeader, e_Fail);
		return 1;  // let the host finish parsing.
	}

    while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
		posix_sleep_ticks(1);

	StatusHeader->DataLength = FileSize(DataBuffer);
	Size = StatusHeader->DataLength;   // the amount to send
	StatusHeader->Signature = pReadHeader->Signature;
	StatusHeader->Message = pReadHeader->Message;
	StatusHeader->Status = e_Success;
    // Done with the file name, return the memory.
	MEM_FREE_AND_NULL(DataBuffer);
	//
	// Send the status structure.
	//
	BytesRead = Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));

    if (Size <= 0)
    {
        // There's no data to send
        // close the file and leave
        xt_fclose(FilePtr);
        return 1;
    }

    // send the data to the host
    //
    while(Size > 0)
    {
        db("Size %d\n", Size);
        // Check the amount of process.
        AmtToProcess = FAX_BUFFER_SIZE;
        if(AmtToProcess > Size)
            AmtToProcess = Size;
        //
        // Get memory for the data
        //
        while((DataBuffer = MEM_MALLOC(AmtToProcess)) == NULL)
            posix_sleep_ticks(1);
        //
        // Get the header for the data header.
        while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
            posix_sleep_ticks(1);

        //
        // Init the status header.
        StatusHeader->Signature = pReadHeader->Signature;
        StatusHeader->Message =  e_FaxDataBlock;
        StatusHeader->Status = e_Success;
        StatusHeader->DataLength = AmtToProcess;

        // Read in the data to xfer
        BytesRead = xt_fread(DataBuffer, 1, AmtToProcess,FilePtr);
        // If we failed, send a fail status and exit.
        if(BytesRead != AmtToProcess)
        {   
            xt_fclose(FilePtr);     // close the file
            StatusHeader->Status = e_Fail;
            MEM_FREE_AND_NULL(DataBuffer);
            Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));
            return 1;
        }
        // Write the data to the host.
        Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));
        BytesRead = Pipe->WriteConnect(Pipe, DataBuffer, AmtToProcess);
        // That is done, decrement size and go on.
        //
        Size -= AmtToProcess;
    }
    //
    // Everything is sent, close the file and send a zero datalength datapacket.
    xt_fclose(FilePtr);     // close the file
    //
    // Now send a zero length datablock.
    //
    while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
        posix_sleep_ticks(1);

    StatusHeader->Signature = pReadHeader->Signature;
    StatusHeader->Message =  e_FaxDataBlock;
    StatusHeader->Status = e_Success;
    Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));

    return 0;
}

static int process_get_faxlog_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{
	 FAX_MESSAGE_HEADER *StatusHeader;
	 faxlog_rtcTime_t t;
	 int i, BytesRead;
	 FAXER_ACTIVITY_ENTRY   LogEntry;
	 char *DataBuffer, *Ptr;
	 FAX_LOG_MESSAGE *LogMessage;
	 
    // Get memory for the file name
	 if(pReadHeader->DataLength == 0)
	 {
		send_fax_status(Pipe, pReadHeader, e_Fail);
		return -1;
	 }
	//
	// Get memory for the file name.
	//
 	 while((DataBuffer = MEM_MALLOC(pReadHeader->DataLength + 1)) == NULL)
	 	 posix_sleep_ticks(1);     // wait for a buffer.
	//
	// Get the filename requested.
	//
	 BytesRead = Pipe->ReadConnect(Pipe, DataBuffer, pReadHeader->DataLength,PARSEREAD_EXACT);
	 if(BytesRead != pReadHeader->DataLength)
	 {
		MEM_FREE_AND_NULL(DataBuffer);
		send_fax_status(Pipe, pReadHeader, e_Fail);
		return FALSE;
	 }
	 DataBuffer[pReadHeader->DataLength] = '\0';
	 Ptr = DataBuffer;
	//
	// Get the memory for the log entries.
	//
	i = 0;      // this is the log entry we are using
	while(faxer_log_get_activity_entry(i++, &LogEntry) == 0)
	{
		//
		// See if the name matches what we expect.  If not try the next one.
		// limit the search to keep things from running away.
		//
		if(strncmp(Ptr, LogEntry.fname, strlen(Ptr)) == 0)
		{
			// found the entry I want.  Send it back.
			//
			while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
				posix_sleep_ticks(1);
			
			while((LogMessage = MEM_MALLOC(sizeof(FAX_LOG_MESSAGE))) == NULL)
				posix_sleep_ticks(1);

			StatusHeader->Signature = pReadHeader->Signature;
			StatusHeader->Status = e_Success;
			StatusHeader->Message = pReadHeader->Message;

			StatusHeader->DataLength = sizeof(FAX_LOG_MESSAGE);  // the amount coming.
			Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER)); // will free status header

			// Fill in the message with data from the log
			// Why have two structures?  Because the internal one keeps getting changed and
			// messing up the host by having the fields out of order.
			LogMessage->Duration  = LogEntry.duration;
			LogMessage->FaxStatus = LogEntry.status;
			LogMessage->FaxType   = LogEntry.type;
			LogMessage->Pages     = LogEntry.page_cnt;
			// switch from internal fax time format to correct log structure format
			t.year  = FAXER_TIME_GET_YEAR(LogEntry.time);
			t.month = FAXER_TIME_GET_MONTH(LogEntry.time) + 1;
			t.day   = FAXER_TIME_GET_DAY(LogEntry.time) + 1;
			t.hour  = FAXER_TIME_GET_HOUR(LogEntry.time);
			t.min   = FAXER_TIME_GET_MIN(LogEntry.time);
			t.sec   = FAXER_TIME_GET_SEC(LogEntry.time);

			LogMessage->SendTime  = t;
			LogMessage->Thumbnail = LogEntry.thumbnail;
			memcpy(LogMessage->CIS, LogEntry.CIS, 20); // FIX this later when the software catches up to our size change.
			LogMessage->CIS[19] = 0;
			memcpy(LogMessage->FileName, LogEntry.fname, LOG_FILENAME_SIZE);

			// Send the data
			Pipe->WriteConnect(Pipe, LogMessage, sizeof(FAX_LOG_MESSAGE)); // will free logmessage
			MEM_FREE_AND_NULL(DataBuffer);
			return 1;
		}
	}
	// We did not find anything to return, return an error
	MEM_FREE_AND_NULL(DataBuffer);
	send_fax_status(Pipe, pReadHeader, e_InvalidPath);
	
	return 0;
}

static int process_get_fax_setting_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{
	 DEFAULT_FAX_SETTINGS *FaxSettings;
	 FAX_MESSAGE_HEADER *StatusHeader;
	 unsigned int tmp;
	 rtcTime_t TOD;
	 int BytesRead;
	 
	 while((FaxSettings = MEM_CALLOC(sizeof(DEFAULT_FAX_SETTINGS),1)) == NULL)
	      posix_sleep_ticks(1);

	  while((StatusHeader = MEM_CALLOC(sizeof(FAX_MESSAGE_HEADER),1)) == NULL)
	      posix_sleep_ticks(1);
	  
	  faxer_config_get_setting(FAXER_SETTING_INT_ANSWER_MODE, &tmp);
	  FaxSettings->AutoAnswer = tmp;

	  faxer_config_get_setting(FAXER_SETTING_INT_RING_PATTERN, &tmp);
	  FaxSettings->RingPattern = tmp;
	  
	  faxer_config_get_setting(FAXER_SETTING_CHR_HEADER_NAME, FaxSettings->FaxName);
	  
	  faxer_config_get_setting(FAXER_SETTING_CHR_SID, FaxSettings->FaxNumber);
	  
	  faxer_config_get_setting(FAXER_SETTING_SW_DIAL_PREFIX, &tmp);
	  if(tmp)
		  faxer_config_get_setting(FAXER_SETTING_CHR_PREFIX, FaxSettings->Prefix);
      else
         FaxSettings->Prefix[0] = 0;
	  
	  // Get the rtc time
	  rtcGetTime(&TOD);   // get the time.
	  // format date/time as YYYYMMDDHHMMSS
	  minSprintf(FaxSettings->DeviceDateTime, "%4d%02d%02d%02d%02d%02d",
				   TOD.year + 2000, 
				   TOD.month+1, 
				   TOD.day+1, 
				   TOD.hour, 
				   TOD.min, 
				   TOD.sec);

	  faxer_config_get_setting(FAXER_SETTING_INT_DIALING_MODE, &tmp);
	  FaxSettings->PulseDialing = tmp;
	  
	  faxer_config_get_setting(FAXER_SETTING_SW_1ST_PAGE_INCLUDED, &tmp);
	  FaxSettings->bThumbFirst = tmp;
	  
	  faxer_config_get_setting(FAXER_SETTING_INT_CONFIRM_AUTO_PRINT, &tmp);
	  FaxSettings->PrintFaxReport = tmp;

	  StatusHeader->DataLength = sizeof(DEFAULT_FAX_SETTINGS);
	            
	  StatusHeader->Signature = pReadHeader->Signature;
	  StatusHeader->Message = pReadHeader->Message;
	  StatusHeader->Status = e_Success;
	  BytesRead = Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));
	  BytesRead = Pipe->WriteConnect(Pipe, FaxSettings, sizeof(DEFAULT_FAX_SETTINGS));
	  
	  return 0;
}

static int process_set_fax_setting_request(IOReg *Pipe, FAX_MESSAGE_HEADER *pReadHeader)
{

	 DEFAULT_FAX_SETTINGS *FaxSettings;
	 unsigned int tmp;
	 int BytesRead;
     int vals[6];
     rtcTime_t TOD;
     ERROR_TYPE err;

      while((FaxSettings = MEM_MALLOC(sizeof(DEFAULT_FAX_SETTINGS))) == NULL)
	      posix_sleep_ticks(1);

	  BytesRead = Pipe->ReadConnect(Pipe, FaxSettings, sizeof(DEFAULT_FAX_SETTINGS), PARSEREAD_EXACT);
	  if(BytesRead != sizeof(DEFAULT_FAX_SETTINGS))
	  {
		  MEM_FREE_AND_NULL(FaxSettings);
		  send_fax_status(Pipe, pReadHeader, e_Fail);
		return FALSE;
	  }

	/*
	 * Skip password checks for now, since we're always reporting that there isn't a password
	*/
	 

    // Get the current date/time settings to ensure that all fields (even those
    // we don't care about, like DOW) are set to a valid value before we begin.
    // We will then overwrite the ones we care about.
    rtcGetTime(&TOD);

      // Handle the date/time (should be in YYYYMMDDHHMMSS format)
	sscanf(FaxSettings->DeviceDateTime + 2, "%2d%2d%2d%2d%2d%2d",
			   vals + 0, vals + 1, vals + 2, vals + 3, vals + 4, vals + 5); 
    // Read in as ints then casted to chars to prevent them from reading the ASCII value
    TOD.year  = (char)vals[0];
	TOD.month = (char)vals[1]-1;
	TOD.day   = (char)vals[2]-1;
	TOD.hour  = (char)vals[3];
	TOD.min   = (char)vals[4];
	TOD.sec   = (char)vals[5];
	err = rtcSetTime(&TOD);

    if (err)
    {
        dbge("Error when setting RTC time: %d", err);
    }

	tmp=FaxSettings->PulseDialing;
	faxer_config_set_setting(FAXER_SETTING_INT_DIALING_MODE, (void *)tmp);
	
	tmp=FaxSettings->bThumbFirst;
	faxer_config_set_setting(FAXER_SETTING_SW_1ST_PAGE_INCLUDED, (void *)tmp);
	
	tmp=FaxSettings->PrintFaxReport;
	faxer_config_set_setting(FAXER_SETTING_INT_CONFIRM_AUTO_PRINT, (void *)tmp);
	
	tmp=FaxSettings->AutoAnswer;
	faxer_config_set_setting(FAXER_SETTING_INT_ANSWER_MODE, (void *)tmp);
	
	tmp=FaxSettings->RingPattern;
	faxer_config_set_setting(FAXER_SETTING_INT_RING_PATTERN, (void *)tmp);
	
	faxer_config_set_setting(FAXER_SETTING_CHR_SID, FaxSettings->FaxNumber);
	
	faxer_config_set_setting(FAXER_SETTING_CHR_HEADER_NAME, FaxSettings->FaxName);
	
	if(strlen( FaxSettings->Prefix))
	{
		tmp=1;
		faxer_config_set_setting(FAXER_SETTING_SW_DIAL_PREFIX, (void *)tmp);
		faxer_config_set_setting(FAXER_SETTING_CHR_PREFIX, FaxSettings->Prefix);
	}
	else
	{
		tmp = 0;
		faxer_config_set_setting(FAXER_SETTING_SW_DIAL_PREFIX, (void *)tmp);
		faxer_config_set_setting(FAXER_SETTING_CHR_PREFIX, "");
			
	}
	
	send_fax_status(Pipe, pReadHeader, e_Success);
		
    MEM_FREE_AND_NULL(FaxSettings);
        
    return 0;
        
}
/* FUNCTION NAME: DoFaxParsing */
 
/** 
 * \brief
 * 
 * \param Pipe
 * 
 * \param Parsing
 * 
 * \retval BOOL
 * 
 * \author Dirk Bodily
 * 
 * \date 3/2/2007
 * 
 **/

static BOOL do_fax_parsing(IOReg *Pipe, BOOL Parsing)
{
    FAX_MESSAGE_HEADER ReadHeader, *StatusHeader;
    int BytesRead;
    BOOL ContinueParsing = Parsing;
    char *DataBuffer;
    FAX_RESPONSE_ENUM Status;
    int ret;
    char tmpS[50];
    unsigned int fax_job_id;

    BytesRead = Pipe->ReadConnect(Pipe, &ReadHeader, sizeof(FAX_MESSAGE_HEADER), PARSEREAD_ANY);
    if(BytesRead != sizeof(FAX_MESSAGE_HEADER))
    {
        // abort the parse
//        dprintf("FAX: abort parse (BytesRead=%d)\n",BytesRead);
        if(BytesRead != 0)
            ContinueParsing = FALSE;
        return ContinueParsing;
    }
    db("Header:\n");
    db("\tCookie: 0x%lx\n", ReadHeader.Signature);
    db("\tMessage: %ld\n", ReadHeader.Message);
    db("\tParam1: 0x%lx\n", ReadHeader.Param1);
    db("\tParam2: 0x%lx\n", ReadHeader.Param2);
    db("\tData Length: %ld\n", ReadHeader.DataLength);
    db("\tStatus: 0x%x\n", ReadHeader.Status);
    if(ReadHeader.Signature != FAX_SIG)
    {
        // Signature failed.
        send_fax_status(Pipe, &ReadHeader, e_CmdNotRecognized);
        return FALSE;
    }

    switch(ReadHeader.Message)
    {
        case e_StartFaxJob:
            {
                // See if we have enough space for a fax job.
                //
                if(faxer_request_free_fs_space(ReadHeader.Param1 + 100000 )>ReadHeader.Param1)
                    Status = e_Success;
                else
                    Status = e_DiskFull;
                send_fax_status(Pipe, &ReadHeader, Status);
            }
            break;

        case e_EndFaxJob:
            send_fax_status(Pipe, &ReadHeader, e_Success);
//            return FALSE;
            break;

        case e_SendJob:
        	ret=process_send_job_request(Pipe, &ReadHeader);
//         	if(ret>0)
//         		return TRUE;
//         	if(ret<0)
//         		return FALSE;
        	break;


        case e_EnumerateFaxes:
        	ret = process_fax_enumeration_request(Pipe, &ReadHeader);
//         	if(ret>0)
// 				return TRUE;
// 			if(ret<0)
// 				return FALSE;
			break;
            
        case e_GetFax:
        	ret= process_get_fax_request(Pipe, &ReadHeader);
//         	if(ret>0)
//         		return TRUE;
//         	if(ret<0)
//         		return FALSE;
        	break;
        	
        case e_GetFaxLogEntry:
        	ret = process_get_faxlog_request(Pipe, &ReadHeader);
//         	if(ret>0)
//                 return TRUE;
//             if(ret<0)
//             	return FALSE;
            break;
          
        case e_FaxReceived:
        	
            //
            // The host has the fax, we can now delete this fax.
            //
            // Get memory for the file name
            while((DataBuffer = MEM_MALLOC(ReadHeader.DataLength + 1)) == NULL)
                posix_sleep_ticks(1);     // wait for a buffer.
            
            BytesRead = Pipe->ReadConnect(Pipe, DataBuffer, ReadHeader.DataLength, PARSEREAD_EXACT);
            if(BytesRead != ReadHeader.DataLength)
            {
                MEM_FREE_AND_NULL(DataBuffer);
                send_fax_status(Pipe, &ReadHeader, e_Fail);
//                return FALSE;
                break;
            }
            DataBuffer[ReadHeader.DataLength] = '\0';
            
            faxer_notify_upload_done(DataBuffer); 
            send_fax_status(Pipe, &ReadHeader, e_Success);
            MEM_FREE_AND_NULL(DataBuffer);
            break;
            
        case e_CheckFaxPasswordSet:
           	faxer_config_get_setting(FAXER_SETTING_CHR_PASSWORD,tmpS);
           	if(tmpS[0] == 0)
                 send_fax_status(Pipe, &ReadHeader, e_Fail);
            else
                 send_fax_status(Pipe, &ReadHeader, e_Fail); 
            break;
            
        case e_CheckFaxPassword:
		   // Get memory for the file name
			while((DataBuffer = MEM_MALLOC(ReadHeader.DataLength + 1)) == NULL)
				posix_sleep_ticks(1);     // wait for a buffer.
			
			BytesRead = Pipe->ReadConnect(Pipe, DataBuffer, ReadHeader.DataLength, PARSEREAD_EXACT);
			if(BytesRead != ReadHeader.DataLength)
			{
				MEM_FREE_AND_NULL(DataBuffer);
				send_fax_status(Pipe, &ReadHeader, e_Fail);
//				return FALSE;
                break;
			}
			DataBuffer[ReadHeader.DataLength] = '\0';
			faxer_config_get_setting(FAXER_SETTING_CHR_PASSWORD,tmpS);
			
			if(strcmp(DataBuffer, tmpS) == 0)
			{
				// It matches.
				send_fax_status(Pipe, &ReadHeader, e_Success);
			}
			else
			{
				send_fax_status(Pipe, &ReadHeader, e_Fail);
			}
			MEM_FREE_AND_NULL(DataBuffer);
            break;
            
        case e_GetDefaultFaxSettings:
        	ret = process_get_fax_setting_request(Pipe, &ReadHeader);
//         	if(ret > 0)
//                  return TRUE;
//             if(ret < 0)
//                   return FALSE;
            break;
           
            
        case e_SetDefaultFaxSettings:
        	ret = process_set_fax_setting_request(Pipe, &ReadHeader);
//         	if(ret > 0)
//         		return TRUE;
//         	if(ret < 0)
//         		return FALSE;
        	break;
        	
        case e_ClearFaxStatus:
            // Nothing to clear so just send success.
            send_fax_status(Pipe, &ReadHeader, e_Success);
            break;
            
        case e_RequestFaxStatus:
        	if(faxer_get_pending_received_fax(0, &fax_job_id, tmpS))
        		tmpS[0]=0;  //Mark it as empty 

            while((StatusHeader = MEM_CALLOC(sizeof(ReadHeader), 1)) == NULL)
                posix_sleep_ticks(1);

            StatusHeader->Signature = ReadHeader.Signature;
            StatusHeader->Message = ReadHeader.Message;
            StatusHeader->Status = e_Success;
            if(tmpS[0]!= '\0')
            {
                StatusHeader->Param1 = e_ReceivedFaxAvailable;
            }
            else
            {
                StatusHeader->Param1 = e_NoFaxes;
            }
            BytesRead = Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));
            break;

        case e_GetFaxRecvSettings:
        {
        	unsigned int tmp;
            //
            // Let the Fax App on the PC know how the printer is handling incomming faxes
            //
            while((StatusHeader = MEM_CALLOC(sizeof(ReadHeader), 1)) == NULL)
                posix_sleep_ticks(1);

            StatusHeader->Signature = ReadHeader.Signature;
            StatusHeader->Message = ReadHeader.Message;
            StatusHeader->Status = e_Success;

            faxer_config_get_setting(FAXER_SETTING_SW_CONFRCV, &tmp);
            if(tmp)
            {
            	 StatusHeader->Param1 = e_RecvModePrivate;
            }else
            {
            	 faxer_config_get_setting(FAXER_SETTING_SW_FORWARD, &tmp);
            	 if(tmp)
            		 StatusHeader->Param1 = e_RecvModeForward ;
            	 else
            	 {
            		 faxer_config_get_setting(FAXER_SETTING_SW_RCV_TO_PC, &tmp);
            		 if(tmp)
            			 StatusHeader->Param1 = e_RecvModeHost;
            		 else
            			 StatusHeader->Param1 = e_RecvModePrint;
            			 
            	 }
            }
            BytesRead = Pipe->WriteConnect(Pipe, StatusHeader, sizeof(FAX_MESSAGE_HEADER));

            break;
        }
        default:
            // unsupported message code -- ignore
            break;
    }
//    return ContinueParsing;
    return FALSE;
}

/*Update the print agent with a new faxer event*/
void parser_job_agent_event_update(unsigned int evt, unsigned int param)
{
	MESSAGE msg;
    msg.msgType = MSG_FAXER_EVT;
    msg.param1 = evt;
    msg.param2 = param;
    SYMsgSend(FAXPARSERID, &msg);
}

/*
 *  Agent thread function for fax parser
 * 
 */
void faxer_app_agent_parser(unsigned long Input)
{
	PARSE_SETUP Parser;
	MESSAGE job_msg;
    MESSAGE msg;
	STARTRECIPE *StartIt;
	QUEUE *pQueue = (QUEUE *)Input;
    BOOL Parsing=FALSE;
    CURRENT_RESOURCE  *OurResource;
	
    SysWaitForInit();

	//Register as a parser
    Parser.ParserID = FAXPARSERID;
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1;
    Parser.ParseString = NULL;
    Parser.ParseType = e_FaxParser;      // parser type.
    Parser.PipeType = e_FaxChan;
    Parser.ShutDownFunction = NULL;
    Parser.EventNotify = NULL;  // no event notification
    ParserRegister(&Parser);        // register the parser.
    
    while(1)
    {        
        if(0 == posix_wait_for_message(pQueue, (char*)&msg,  sizeof(MESSAGE), POSIX_WAIT_FOREVER))
        {       		
    	    switch(msg.msgType)
    	    {
                case MSG_FAXER_EVT:
                    // These messages are expected, but we don't need to do
                    // anything with them.  (Just here to keep you out of default case)
                    break;

			    case MSG_START_PARSE:
//                      dprintf("FAX: MSG_START_PARSE - requesting resources\n");
                    Pipe = msg.param3;      // initialize the pipe
                    START_JOB_PIPE(ejob_FaxParse, FAXPARSERID, 0, e_WaitForever, Pipe);
    				break;
			
    			case MSG_RESOURCES:
     	      		OurResource = (CURRENT_RESOURCE *) msg.param3;
     	      		break;
     	      		
    			case MSG_ACKRECIPE:
//                  dprintf("FAX: MSG_ACKRECIPE - got resources\n");
    				Parsing = TRUE;
    				break;
    				
    			case MSG_NAKRECIPE:
    				dbge("Got NAKRECEIPE! Abort printing!");
    				break;
    				
    			case MSG_CANCELJOB:	
    				break;
    				
                default:
    				db("Unknown msg! type=%d\n", msg.msgType);
    				break;
            }
        }

    	if(Parsing)
		{
			Parsing = do_fax_parsing(Pipe, Parsing);
			if(Parsing == FALSE)
			{
				// we are done parsing, release the job.
				// finished parsing, free the resources.
				msg.msgType = MSG_FREERECIPE;
				msg.param1 = 0;
				msg.param2 = FAXPARSERID;
				msg.param3 = OurResource;
				SYMsgSend(SJMID, &msg); // param3 is untouched and now sent back.
				//
				// Tell the connection manager the results of the parse.
				//
//                dprintf("FAX: done parsing\n");
//				msg.param1 = ParseRet;
				msg.param1 = PARSER_SUCCESS;
				msg.msgType = MSG_END_PARSE;
				msg.param3 = Pipe;          // The pipe that we are parsing.
				SYMsgSend(CMPARSER, &msg);      // send the response.
			}
		}
    	
    }
	return;
}


