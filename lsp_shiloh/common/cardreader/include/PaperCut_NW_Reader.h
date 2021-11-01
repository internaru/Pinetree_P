#ifndef __SYS_PAPERCUT_MGR_H__
#define __SYS_PAPERCUT_MGR_H__

#include "posix_ostools.h"
#define PAPERCUT_MAX_CODE_LENGTH 201

void SysServicePaperCutInit(void);

#ifdef SYSCARD_MAIN_C
pthread_mutex_t papercut_mutex;
//char papercut_code[PAPERCUT_MAX_CODE_LENGTH];
#else
extern pthread_mutex_t papercut_mutex;
//extern char papercut_code[PAPERCUT_MAX_CODE_LENGTH];
#endif

#endif //__SYS_CARD_READER_MGR_H__
