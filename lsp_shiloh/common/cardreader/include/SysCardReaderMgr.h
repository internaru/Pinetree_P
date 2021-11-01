#ifndef __SYS_CARD_READER_MGR_H__
#define __SYS_CARD_READER_MGR_H__

#include "SysCardReaderMsg.h"

void SysServiceCardReaderInit(void);
int AuthVerify(char *id, char *passwd, int authMod);
// 0: NEST / PINETREE-S - ID?PASSWORD
// 1: Lexmark - ID only

#endif //__SYS_CARD_READER_MGR_H__
