/*
 *
 * ============================================================================
 * Copyright (c) 2008-2013  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief 
 *
 */

#ifndef ZPJLPARSER_H
#define ZPJLPARSER_H

#include "pjl_api.h"

#ifdef __cplusplus
extern "C" {
#endif


/// in parser thread get function from Pipe
PrintJobInfo_t *pji();

/// in parser register 
void RegisterGramString(char *String);

/// in parser register word match for enumeration value fields like:  ON OFF 
int RegisterVarEnum(char *String, WORD_MATCH **Match, UINT16*);

/// for sending AuthResult to SJM
int AuthRequestSend(char *auth_id, char *auth_pw, int auth_mode);


#ifdef __cplusplus
}
#endif

#endif
