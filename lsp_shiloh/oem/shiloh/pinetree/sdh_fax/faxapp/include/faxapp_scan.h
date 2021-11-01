#ifndef __FAXAPP_SCAN__
#define __FAXAPP_SCAN__

#include "posix_ostools.h"


#define FAXSCANTASK_QUEUE_SIZE     50

#define FAXAPP_SCAN_STACK_SIZE (MAX((1024 * 10 * 3), POSIX_MIN_STACK_SIZE))


#define FAX_SCAN_SOURCE_FB     (0)
#define FAX_SCAN_SOURCE_ADF     (1)

#define END_JOB(resource, module)   do{\
                     job_msg.msgType = MSG_FREERECIPE;\
                     job_msg.param1 = 0;\
                     job_msg.param3 = (resource);\
                     SYMsgSend(SJMID, &job_msg);\
                     resource = NULL; }while(0)

#define ACK_CANCEL(resource, module) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_ACK;\
                     job_msg.param2 = module;\
                     job_msg.param3 = resource;\
                     SYMsgSend( SJMID, &job_msg);}while(0)


void FaxScanJobMgrInit(void);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern UINT32 faxapp_get_originalSizeCode(void);
extern UINT32 faxapp_get_DualScan(void);
extern int fsu_get_memory_full_status_nand(void);
extern int fsu_get_memory_full_status_ram(void);

#endif // #ifndef __FAXAPP_SCAN__
