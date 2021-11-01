/*
 *
 * ============================================================================
 * Copyright (c) 1998-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file zPJL_put.c
 *
 * \brief lexer / parser module -- semantics handle via callbacks
 *
 *  Some syntax differences:
 *    (1) we allow mixed case in the @PJL, so '@Pjl echo foo' is valid
 *    (2) we allow @PJL by itself, so '@PJL<CR><LF>' is a comment line
 */

#include <stdbool.h>
#include <stdlib.h> /* for atoi */

#include "zPJL.h"
#include "zPJL_xxx.h"

#include "logger.h"		// for debug 		2013.08.22		by Juny
/* user-defined symbol table */
#include "zPJL_sym.h"
#include "zpjlparser.h"
#include "SysCardReaderMgr.h"
#include "db_api.h"

#include <string.h>

// for debug 		2013.08.22		by Juny
#define DBG_PRFX "pjl: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD


#define MSG_BUF_SIZE  80

extern int uPrint_available;
extern int auth_available;

static const STATE_RANGE States[] = { 

	{ S_FIRST, R_END, '@', '@' },
	{ S_FIRST, R_IGNORE, ' ', ' ' },
	{ S_FIRST, R_IGNORE, TAB, TAB },
	/* without any REJECT, all else is rejected! */

	{ S_START_WORDMATCH, R_ACCEPT_and_END, 'A', 'Z' },
	{ S_START_WORDMATCH, R_ACCEPT_and_END, 'a', 'z' },
/**/{ S_START_WORDMATCH, R_ACCEPT_and_END, '0', '9' },
	{ S_START_WORDMATCH, R_IGNORE, ' ', ' ' },
	{ S_START_WORDMATCH, R_IGNORE, TAB, TAB },

	{ S_WORDMATCH, R_ACCEPT, 'A', 'Z' },
	{ S_WORDMATCH, R_ACCEPT, 'a', 'z' },
	{ S_WORDMATCH, R_ACCEPT, '0', '9' },
/**/{ S_WORDMATCH, R_ACCEPT, '-', '-' },
/**/{ S_WORDMATCH, R_ACCEPT, '_', '_' },
	{ S_WORDMATCH, R_END, ':', ':' },
	{ S_WORDMATCH, R_END, '=', '=' },
	{ S_WORDMATCH, R_END, ' ', ' ' },
	{ S_WORDMATCH, R_END, TAB, TAB },
	{ S_WORDMATCH, R_END, LF, LF },

	{ S_START_WORD, R_ACCEPT_and_END, 'A', 'Z' },
	{ S_START_WORD, R_ACCEPT_and_END, 'a', 'z' },
	{ S_START_WORD, R_IGNORE, ' ', ' ' },
	{ S_START_WORD, R_IGNORE, TAB, TAB },

	{ S_WORD, R_ACCEPT, '!', '~' },
	{ S_WORD, R_END, ' ', ' ' },
	{ S_WORD, R_END, TAB, TAB },
	{ S_WORD, R_END, LF, LF },

	{ S_START_NUMBER, R_ACCEPT_and_END, '0', '9' },
	{ S_START_NUMBER, R_ACCEPT_and_END, '-', '-' },
	{ S_START_NUMBER, R_IGNORE, ' ', ' ' },
	{ S_START_NUMBER, R_IGNORE, TAB, TAB },

	{ S_NUMBER, R_ACCEPT, '0', '9' },
	{ S_NUMBER, R_END, ' ', ' ' },
	{ S_NUMBER, R_END, TAB, TAB },
	{ S_NUMBER, R_END, LF, LF },

	{ S_START_QUOTE, R_END, '"', '"' },
	{ S_START_QUOTE, R_IGNORE, ' ', ' ' },
	{ S_START_QUOTE, R_IGNORE, TAB, TAB },

    /* put the R_END condition ahead of the R_ACCEPT if it's in the range */
	{ S_QUOTED, R_END, '"', '"' },
	{ S_QUOTED, R_ACCEPT, TAB, TAB },
	{ S_QUOTED, R_ACCEPT, 0x20, 0xFC },

	{ S_COMMENT, R_ACCEPT, 0x20, 0xFC },
	{ S_COMMENT, R_ACCEPT, TAB, TAB },
	{ S_COMMENT, R_END, LF, LF },

	{ S_ERROR, R_ACCEPT, ' ', '~' },
	{ S_ERROR, R_ACCEPT, TAB, TAB },
	{ S_ERROR, R_END, LF, LF },

	{ S_LAST } };
	
/* -------------------------------------------------------------------------- */

static int strcmpi_( const char *pStrOne, const char *pStrTwo )
{
	char one, two;
	while (1)
	{
		one = *pStrOne++,  two = *pStrTwo++;
		
		/*	make upper-case */
		if (one >= 'a' && one <= 'z')
			one -= ('a' - 'A');
		if (two >= 'a' && two <= 'z')
			two -= ('a' - 'A');
		
		/* compare */
		if (one < two)
			return (-1);
		if (one > two)
			return (1);
		if (!one && !two)
			return (0);
	}
}

/* -------------------------------------------------------------------------- */
#define	XOR_KEYWORD		":@#$%^&*():@#$%^&*():@#$%^&*():@#$%^&*():@#$%^&*():@#$%^&*()"

const char __base64_pad = '='; 

const char reverse_table[] = {
	 64, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255,  62, 255, 255, 255,  63,  52,  53,
	 54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 255, 255, 255, 255,   0,   1,   2,   3,   4,
	  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
	 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
	 25, 255, 255, 255, 255, 255, 255,  26,  27,  28,
	 29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
	 39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255
};

void calculate_XOR(char* szValue, char* szKey, int size)
{
	char* szDefKey = szKey;

	while(size--) {
		*szValue ^= *szKey;
		DBG_PRINTF_INFO("dec=%c key=%c size=%d\n", *szValue, *szKey, size);
		szValue++;
		szKey++;

		if(szKey == NULL) {
			szKey = szDefKey;
		}
	}
}

char *__base64_decode( char *str, int length, int *ret_length) { 
   char *current = str; 
   int ch, i = 0, j = 0, k; 
   /* this sucks for threaded environments */ 
   char *result;
   
   result = (char *)malloc(length + 1); 
   if (result == NULL) { 
      return NULL; 
   } 
 
   /* run through the whole string, converting as we go */ 
   while ((ch = *current++) != '\0') { 
      if (ch == __base64_pad) break; 
 
      /* When Base64 gets POSTed, all pluses are interpreted as spaces. 
         This line changes them back.  It's not exactly the Base64 spec, 
         but it is completely compatible with it (the spec says that 
         spaces are invalid).  This will also save many people considerable 
         headache.  - Turadg Aleahmad <turadg@wise.berkeley.edu> 
      */ 
 
      if (ch == ' ') ch = '+'; 
 
      ch = reverse_table[ch]; 
      if (ch < 0) continue; 
 
      switch(i % 4) { 
      case 0: 
         result[j] = ch << 2; 
         break; 
      case 1: 
         result[j++] |= ch >> 4; 
         result[j] = (ch & 0x0f) << 4; 
         break; 
      case 2: 
         result[j++] |= ch >>2; 
         result[j] = (ch & 0x03) << 6; 
         break; 
      case 3: 
         result[j++] |= ch; 
         break; 
      } 
      i++; 
   } 
 
   k = j; 
   /* mop things up if we ended on a boundary */ 
   if (ch == __base64_pad) { 
      switch(i % 4) { 
      case 0: 
      case 1: 
         free(result); 
         return NULL; 
      case 2: 
         k++; 
         /* no break	*/
      case 3: 
         result[k++] = 0; 
      } 
   } 
   if(ret_length) { 
      *ret_length = j; 
   } 
   result[k] = '\0'; 
   return result; 
}

UINT8	decrypt_password( char *str, UINT8 maxlength, char *key ) {
	int	size;
	char	*str_temp = NULL;
	
	if ( strlen(str) > maxlength ) {
		return 1;
	}

	DBG_PRINTF_INFO("str: %02x %02x %02x %02x %02x %02x %02x\n", str[0], str[1], str[2], str[3], str[4], str[5], str[6] );
	DBG_PRINTF_INFO("key: %02x %02x %02x %02x %02x %02x %02x\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6] );

	str_temp = __base64_decode( str, strlen(str), &size );
	DBG_PRINTF_ERR("inhwan5: after __base64_decode: str_temp = %s size = %d\n", str_temp, size);

	if ( str_temp == NULL ) {
		return 1;
	}
	memcpy( str, str_temp, size );
	str[size] = 0;

	free( str_temp);
	
	DBG_PRINTF_INFO("dec: str(%d) = %s\n", strlen(str), str );
	DBG_PRINTF_INFO("dec: %02x %02x %02x %02x %02x %02x %02x\n", str[0], str[1], str[2], str[3], str[4], str[5], str[6] );

	calculate_XOR( str, key, size );
	str[size] = 0;

	DBG_PRINTF_INFO("XOR: %s, %d\n", str, strlen(str));

	return 0;
}

static Int32 uel_test = 0;
static char Uel[]="\x1b%-12345X";
static char LXKID[]="USERID = ";
Int32 state_auth = 0;
static char str_authID[33]="";
static char str_authPW[64]="";
static char str_authLXKID[64]="";
static char str_authSDHID[64]="";
static Int32 count_authID = 0;
static Int32 count_authPW = 0;
static Int32 count_LXKID = 0;
static Int32 count_SDHID = 0;
Int32 authCheck = 0;
static Int32 NoUseNumber = 0;

static int auth_fill(int c)
{
	int result = 0, ret = 0;
	DBG_PRINTF_INFO( "%s [%c](0x%x) in state_auth(%d)\n", __FUNCTION__ , c, c, state_auth);	// 
	switch ( state_auth  )
	{
		case 1:
			if ( c == 0x22 )	// "
				state_auth = 2;
		break;
		case 2:
			if ( c == 0x0D )	// CR
			{
				str_authID[count_authID-1] = 0;
				if ( count_authID == 1 )	/// Fail: No ID
				{
					DBG_PRINTF_ERR( "str_authID is empty \n");
					//AuthResultSend(str_authID, 0);
					//ret = 1;
				}
				else
				{
					DBG_PRINTF_ERR( "str_authID(%d) = %32s \n", strlen(str_authID), str_authID);
				}

				state_auth = 0;
				count_authID = 0;
			}
			else
			{
				str_authID[count_authID] = c;
				count_authID++;
			}
		break;
		case 11:
			if ( c == 0x22 )	// "
				state_auth = 12;
		break;
		case 12:
			if ( c == 0x0D )	// CR
			{
				str_authPW[count_authPW-1] = 0;
				if ( count_authPW == 1 )	/// Fail: No Password
				{
					DBG_PRINTF_ERR( "str_authPW is empty \n");
					if ( strlen(str_authID) > 1 )
					{
						//AuthResultSend(str_authID, 0);
						//ret = 1;
					}
				}
				else
				{
					DBG_PRINTF_ERR( "str_authPW(%d) = %64s \n", strlen(str_authPW), str_authPW);
					
					/// str_authPW change
					result = decrypt_password ( str_authPW, 64, XOR_KEYWORD );
					if ( result )	/// 1: NG
						DBG_PRINTF_ERR( "ERROR: descryption is failed \n");
					else	/// 0: OK
						DBG_PRINTF_ERR( "descrypted str_authPW(%d) = %32s \n", strlen(str_authPW), str_authPW);

					authCheck = 1;
					DBG_PRINTF_ERR( "ID & PW were found\n");
				}

				state_auth = 0;
				count_authPW = 0;
			}
			else
			{
				str_authPW[count_authPW] = c;
				count_authPW++;
			}
		break;
		
		case 21:	//"USERID = "
			if ( c == LXKID[count_LXKID] )
				count_LXKID++;
			else
				count_LXKID = 0;
			if ( count_LXKID == 9 )
			{
				state_auth = 22;
				count_LXKID = 0;
			}
		break;
		
		case 22:
			if ( c == 0x22 )	// "
				state_auth = 23;
		break;
		
		case 23:
			if ( c == 0x22 )	// "
			{
				str_authLXKID[count_LXKID] = 0;
				if ( count_LXKID == 0 )	/// Fail: No ID
				{
					DBG_PRINTF_ERR( "str_authLXKID is empty \n");
					//AuthResultSend(str_authLXKID, 0);
					//ret = 1;
				}
				else
				{
					DBG_PRINTF_ERR( "str_authLXKID(%d) = %32s \n", strlen(str_authLXKID), str_authLXKID);
					
					/// authentication verify
					
					authCheck = 11;
					DBG_PRINTF_ERR( "only ID was found\n");
				}

				state_auth = 0;
				count_LXKID = 0;
			}
			else
			{
				str_authLXKID[count_LXKID] = c;
				count_LXKID++;
			}
		break;
		
		case 31:
			if ( c == 0x22 )	// "
				state_auth = 32;
		break;
		case 32:
			if ( c == 0x0D )	// CR
			{
				str_authSDHID[count_SDHID-1] = 0;
				if ( count_SDHID == 1 )	/// Fail: No ID
				{
					DBG_PRINTF_ERR( "str_authSDHID is empty \n");
				}
				else
				{
					DBG_PRINTF_ERR( "str_authSDHID(%d) = %32s \n", strlen(str_authSDHID), str_authSDHID);
					authCheck = 21;
				}

				state_auth = 0;
				count_SDHID = 0;
			}
			else
			{
				str_authSDHID[count_SDHID] = c;
				count_SDHID++;
			}
		break;
		
		default:
			DBG_PRINTF_ERR( "ERR: Unkwon state_auth (%d) \n", state_auth);
		break;
	}
	return ret;
}

Int32  zpjlPutc(ZPJL zpjl, int c)
{
	int result = 0, ret = 0;
	
	if ( authCheck == 55 )
	{
		zpjl->pMatchList = NULL;
		authCheck = 0;
		state_auth = 0;
		count_authID = 0;
		count_authPW = 0;
		count_LXKID = 0;
		return ZPJL_CANCEL;
	}
	
	if ( state_auth && uPrint_available && auth_available )
	{
		ret = auth_fill(c);

		if ( ret )
		{
			DBG_PRINTF_ERR( "return CANCEL state!!! \n");
			zpjl->pMatchList = NULL;
			authCheck = 0;
			state_auth = 0;
			count_authID = 0;
			count_authPW = 0;
			count_LXKID = 0;
			return ZPJL_CANCEL;
		}
	}
	
    Int32  rc = -1;
    void*  args[4]; /* for callbacks -- watch the size! */

    if(c == Uel[uel_test])
    {
        if(!uel_test)
            zpjl->cPrev = -1;
        uel_test++;
        if(uel_test >= strlen(Uel))
        {
            DBG_PRINTF_INFO( "UEL %d >= %d\n", uel_test , strlen(Uel) );	// 
            zpjl->cPrev = LF; // UEL not followed by @PJL is exit condition.
            uel_test = 0;
	    zpjl->State = S_FIRST; 
            return ZPJL_MORE; // UEL followed by @ is a comment don't exit we keep parsing and return what we don't grok.
        }
    } else
        uel_test = 0;



    if (zpjl->First)
    {      /* first time?  create the state LUT */
        const STATE_RANGE *psr = States;
        int prev = -1;

        for ( ; S_LAST != psr->state; psr++ )
        {
            if (psr->state != prev)
                zpjl->pStateStart[ (int)psr->state ] = (STATE_RANGE*) psr;/*cast for non-const*/
            prev = psr->state;
        }
        zpjl->First = ! zpjl->First;
        zpjl->State = S_FIRST;
    }

    rc = (zpjl->cPrev != LF || c == '@');  /* still in PJL mode? */

    if (rc)
    {
        STATE_RANGE *psr = zpjl->pStateStart[ zpjl->State ];
        int prevState = zpjl->State;
        UINT num = 0;

        for ( ; rc && psr && psr->state == zpjl->State; psr++ )
        {

            if (c >= psr->first && c <= psr->last)
            {
                switch (psr->aire)
                {
                    
                    case R_ACCEPT_and_END:
                    case R_ACCEPT:
            DBG_PRINTF_DEBUG_M( "%c\n", c );	// level fix 11.28	@@ 2013.08.22 by Juny
                        /*check for buffer overflow*/
                        if(zpjl->inWord < COMMENTBUFSIZE)
                        {
                        zpjl->szWord[ zpjl->inWord++ ] = (uint8_t) c;
                        }
                        rc = 0;
                        if (psr->aire == R_ACCEPT)
                            break;

                    case R_END: {
//            DBG_PRINTF_DEBUG( "%s @@ END %c \t", __FUNCTION__, c );	// @@ 2013.08.22 by Juny
                            zpjl->szWord[ zpjl->inWord ] = 0;
                            rc = 0;

                            switch (zpjl->State)
                            {
                                /* state transitions ... */
                                
                                case S_FIRST:
                                    zpjl->pMatchList = (WORD_MATCH*) zpjl->wm_FIRST;/*cast for non-const*/
                                    zpjl->State++;
                                    zpjl->inWord = 0;
                                    break;

                                case S_WORDMATCH:
									NoUseNumber = 0;
                                    if (zpjl->pMatchList)
                                    {
                                        for ( ; zpjl->inWord && NULL_SYMBOL != zpjl->pMatchList->key; zpjl->pMatchList++)
                                        {
            DBG_PRINTF_DEBUG_H( "\t %s is listed \n", SYM(zpjl->pMatchList->key) );	// @@ 2013.11.30 by Juny
                                            if (! (rc = strcmpi_( zpjl->szWord, SYM(zpjl->pMatchList->key) )))
                                                break;
                                        }
                                        if (rc == 0)
                                        {
                                            /*match!*/
                                            if (A_UNKNOWN != zpjl->pMatchList->action[0])
                                            {
            DBG_PRINTF_DEBUG( " %s @@ WORD MATCH \n", SYM(zpjl->pMatchList->key) );	// @@ 2013.08.22 by Juny
												//NoUseNumber = 0;
												
												// Auth Priority: SINDOHUSERNAME > AUTHENTICATIONUSERID > LJOBINFO
												if ( str_authSDHID[0]==0 && !strcmp(SYM(zpjl->pMatchList->key), "AUTHENTICATIONUSERID") )
													state_auth = 1;
												else if ( str_authSDHID[0]==0 &&  !strcmp(SYM(zpjl->pMatchList->key), "AUTHENTICATIONPASSWORD") )
													state_auth = 11;
												else if ( (str_authID[0]==0 && str_authSDHID[0]==0) && !strcmp(SYM(zpjl->pMatchList->key), "LJOBINFO") )
													state_auth = 21;
												else if ( !strcmp(SYM(zpjl->pMatchList->key), "SINDOHUSERNAME") )
													state_auth = 31;
												else if ( !strcmp(SYM(zpjl->pMatchList->key), "JOBNAME") )
													NoUseNumber = 1;
												else if ( !strcmp(SYM(zpjl->pMatchList->key), "ENTER") && uPrint_available && auth_available )
												{
													DBG_PRINTF_ERR("authCheck = %d\n", authCheck);
													if ( authCheck )
													{
														/// authentication verify
														if ( authCheck / 20 )
														{
															result = AuthRequestSend(str_authSDHID, "", 1);
															DBG_PRINTF_ERR("AuthVerify return %d\n", result);
															str_authSDHID[0] = 0;
															if ( result )	/// 1: NG = cancel
															{
																authCheck = 55;
																args[0] = (void*) zpjl->szWord;
																zPJL_DefaultParserNotify(zpjl, A_STRING, 1, args);
																zpjl->inWord = 0;
															}
															else	/// 0: OK = go ahead
																authCheck = 0;
														}
														else if ( authCheck / 10 )
														{
															result = AuthRequestSend(str_authLXKID, "", 1);
															//result = AuthVerify(str_authLXKID, str_authPW, 1);
															DBG_PRINTF_ERR("AuthVerify return %d\n", result);
															str_authLXKID[0] = 0;
															if ( result )	/// 1: NG = cancel
															{
																authCheck = 55;
																args[0] = (void*) zpjl->szWord;
																zPJL_DefaultParserNotify(zpjl, A_STRING, 1, args);
																zpjl->inWord = 0;
															}
															else	/// 0: OK = go ahead
																authCheck = 0;
														}
														else
														{
															result = AuthRequestSend(str_authID, str_authPW, 0);
															//result = AuthVerify(str_authID, str_authPW, 0);
															DBG_PRINTF_ERR("AuthVerify return %d\n", result);
															str_authID[0] = 0;
															if ( result )	/// 1: NG = cancel
															{
																authCheck = 55;
																args[0] = (void*) zpjl->szWord;
																zPJL_DefaultParserNotify(zpjl, A_STRING, 1, args);
																zpjl->inWord = 0;
															}
															else	/// 0: OK = go ahead
																authCheck = 0;
														}
													}
													else
													{
														authCheck = 55;
														args[0] = (void*) zpjl->szWord;
														zPJL_DefaultParserNotify(zpjl, A_STRING, 1, args);
														zpjl->inWord = 0;
													}
													args[0] = (void*)(uintptr_t) zpjl->pMatchList->key;
													args[1] = (void*)(uintptr_t) zpjl->pMatchList->action[1];
													zPJL_DefaultParserNotify(zpjl, zpjl->pMatchList->action[0], 2, args);
												}
												else
												{
													/* arg is a symbol, command comes from grammar table */
													args[0] = (void*)(uintptr_t) zpjl->pMatchList->key;
													args[1] = (void*)(uintptr_t) zpjl->pMatchList->action[1];
													zPJL_DefaultParserNotify(zpjl, zpjl->pMatchList->action[0], 2, args);
												}
                                            }
                                            zpjl->State = zpjl->pMatchList->NextState;
                                            if (zpjl->pMatchList->wm_Next != NULL)
                                                zpjl->pMatchList = *((WORD_MATCH**) zpjl->pMatchList->wm_Next);/*cast for non-const*/
                                            else
                                                zpjl->pMatchList = NULL;
                                        }
                                        else
                                        {
            DBG_PRINTF_DEBUG( " @@ WORD MATCH ERROR \n" );	// @@ 2013.08.22 by Juny
                                            zpjl->pMatchList = NULL;
                                            zpjl->State = S_ERROR;
                                            /* report syntax error but stay in PJL */
                                            zPJL_DefaultParserNotify(zpjl, A_COMPLAIN, 0, NULL);
                                        }
                                    }
                                    zpjl->inWord = 0;
                                    break;

                                case S_WORD:
            DBG_PRINTF_DEBUG( " @@ WORD \n" );	// @@ 2013.08.22 by Juny
                                    zpjl->inWord = 0;
                                    zpjl->State = S_START_WORD; /* get the next word */
                                    break;

                                case S_NUMBER:
            DBG_PRINTF_DEBUG( " @@ NUMBER \n" );	// @@ 2013.08.22 by Juny
                                    if ( NoUseNumber != 1 )
                                    {
										zpjl->State = S_START_WORD; /* get the next word */
										/* zpjl->State = S_START_WORDMATCH;  ### get the next word */

										if (zpjl->inWord)
										{
											num = atoi( zpjl->szWord );
											args[0] = (void*)(uintptr_t) num;
											zPJL_DefaultParserNotify(zpjl, A_NUMBER, 1, args);
										}

										zpjl->inWord = 0;
									}
                                    NoUseNumber = 0;
                                    break;

                                case S_QUOTED:
                                case S_COMMENT:
            DBG_PRINTF_DEBUG( " @@ QUOTED or COMMENT \n" );	// @@ 2013.08.22 by Juny
                                    args[0] = (void*) zpjl->szWord;
                                    zPJL_DefaultParserNotify(zpjl, A_STRING, 1, args);
                                    zpjl->inWord = 0;
                                    break;

                                default:
                                    zpjl->State++;
                            }
                        }
                        break;

                        /* S_ERROR means we gobble everthing up to the
                        ** next line feed (so keep returning ZPJL_MORE)
                        ** After that (on the next line, essentially),
                        ** we return ZPJL_CHOKE if we don't like it
                        **
                        ** If it's really not PJL (e.g. binary garbage) it will
                        ** bomb out after the next LF.  If it is PJL (just bad PJL)
                        ** It will act on the properly formed lines it receives.
                        */
                    case R_REJECT:
            DBG_PRINTF_DEBUG( "%s @@ REJECT is error %c \n", __FUNCTION__, c );	// @@ 2013.08.22 by Juny
                        zpjl->State = S_ERROR;
                        break;
                }
            }
        }

        zpjl->cPrev = c;

        if (prevState != zpjl->State && prevState != S_START_WORDMATCH)
        {
// @@ continue condition... might be for status??? 2013.08.22 by Juny
/*      char  msg[MSG_BUF_SIZE];
        sprintf( msg, "old=%u new=%u '%s' %u\n", prevState, zpjl->State, zpjl->szWord, num );
        args[0] = (void*) msg;
        zPJL_DefaultErrorNotify( zpjl, D_DEBUG_MESSAGE, 1, args); */
        }

        if (c == LF)
        {      /* end of line; save result */
            /*     char  msg[MSG_BUF_SIZE];
                   sprintf( msg, "end of line, rc=%d \n", rc );
                   args[0] = (void*) msg;
                   zPJL_DefaultErrorNotify( zpjl, D_DEBUG_MESSAGE, 1, args); */

            zpjl->szWord[ zpjl->inWord = 0 ] = 0;

            if (zpjl->State > S_ERROR && zpjl->State < S_LAST)
                /* execute and clear buffered command */
                zPJL_DefaultCommandNotify(zpjl, A_EXECUTE, zpjl->argCount, zpjl->args);

            zPJL_DefaultParserNotify(zpjl, A_UNBUFFER, 0, NULL);
            zpjl->State = S_FIRST;
        }
    }
    else
    {
        zpjl->cPrev = -1;
        return ZPJL_CHOKE;
    }

/*TODO: check for parse errors*/
/*when should we return ZPJL_CHOKE, ever?*/
    rc = zpjl->enterLanguage ? ZPJL_PDL : ZPJL_MORE;

/*always just clear it so it's ready the next time*/
    zpjl->enterLanguage = 0;

    return(rc);
}
