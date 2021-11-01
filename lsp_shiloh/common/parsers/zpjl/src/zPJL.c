
/*
 *
 * ============================================================================
 * Copyright (c) 1998-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file zPJL.c
 *
 * \brief zPJL lirary.
 *
 *  This is the main SHARED part of a PJL implementation.  DO NOT PUT
 *  device-specific CODE IN HERE!!
 *
 *  WARNING:
 *  There are still vestages of the environment approach to ustatus and job scoping here.
 */

#include <stdbool.h>
#include <string.h>

#include "agRouter.h"
#include "agConnectMgr.h"
#include "zPJL.h"
#include "zPJL_xxx.h"
#include "zStr.h"
#include "paper_size_api.h"
#include "paper_types_api.h"
#include "platform_api.h"
#include "hwconfig_api.h"

/* user-defined symbol table */
#include "zPJL_sym.h"

#include "zpjlparser.h"
#include "PrintJobInfo.h"

#include "lassert.h"
#include "zPJLapi.h"
#include "dprintf.h"
#include "memAPI.h"
#ifdef HAVE_OID
#include "oid_api.h"
#endif
#include "debug.h" // unused var

#define DBG_PRFX "pjl_var: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD


/*#define VERBOSE * uncomment to get runtime debug messages */

#ifdef VERBOSE
    #define DDUMPFLAGS(x) dumpFlags(x)
#else
    #define DDUMPFLAGS(x) ((void)0)
#endif

char PJLPARSER_VERSION[]={"1.2.32"};

/* 2013.12.02		req. from version	 by Juny */
// first : PJL released version, second : PJL parser version, third : update version
/* history
 * 
 * 1.2.32: 2019.04.01: Stored print 파일명 규칙(80자 제한, UTF-8, rename 방법) 수정 (Juniper 2019.2월 대응 내용 수평전개)
 * 1.2.31: 2019.01.23: add extra Print Auth command(SINDOHUSERNAME) for old Solution Driver (Juniper 2016.12.29 사양 적용)
 * 1.2.30: 2016.09.30: If PRN Dump is enable , save PC print job into /tmp directory
 * 1.2.29: 2015.07.02: 인증 ON 기계에 인증정보 없이 출력되는 문제 대책 (AuthType DB 값 참조 오류 수정)
 * 1.2.28: 2015.06.17: Solution 대책 (Cancel 이후 job 시도하면 912 seg. fault)
 * 1.2.27: 2015.03.27: PJL MEDIASOURCE 사용 가능
 * 1.2.26: 2015.01.16: Solution 사양 추가 (UI 인증 추가)
 * 1.2.25: 2014.08.29: MEDIATYPE_RECYCLED 사용
 * 1.2.24: 2014.08.28: MEDIASIZE_A4_ROTATED 추가
 * 1.2.23: 2014.07.25: PJL JOBNAME 숫자 들어가는 문제 대책
 * 1.2.22: 2014.07.24: USB Status Monitor I/F 계속 수정
 * 1.2.21: 2014.07.23: USB Status Monitor I/F 또 수정 (SDK 바뀔때마다 값이 바뀌는구나)
 * 1.2.20: 2014.06.19: USB Status Monitor I/F 수정
 * 1.2.19: 2014.06.09: ID/PW PJL command 반복 처리
 * 1.2.18: 2014.05.26: Sorry 1.2.17 오류 수정
 * 1.2.17: 2014.05.23: LXK ID vs. PT-S ID/PW 공존할때 LXK 우선 설정
 * 1.2.16: 2014.05.22: uPrint 취소처리 오류 수정
 * 1.2.15: 2014.05.22: LXK ID 인증추가, 타사 Driver 취소처리 추가
 * 1.2.14: 2014.05.12: g_bSystemAuthConfChanged 를 보고 DB read 하는 걸로 수정
 * 1.2.13: 2014.05.09: KT_SOL 삭제, g_bSystemAuthConfChanged 로 대체
 * 1.2.12: 2014.05.09: str_authPW XOR 변환 오류 수정
 * 1.2.11: 2014.05.06: str_authPW XOR 변환 추가
 * 1.2.10: 2014.04.28: ZPJL_CANCEL 추가 (인증 실패시 CM cancel)
 * 1.2.09: 2014.04.28 change size of str_authID and str_authPW
 * 1.2.08: 2014.04.25 separate auth vs. non-auth case (auth_available)
 * 1.2.07: 2014.04.24 add uPrint function (AuthResultSend(), AuthVerify())
 1.2.06		2014.02.27 fix to get printer status for USB status monitor
 1.2.05		2014.02.26 distinguish few paper size 
                        (A5LEF, OFUKU148x200 200x148, POSTCARD100X148)
                        fix some set function (no effected) by GG's suggestion
 1.2.04		2014.01.09 (applied 01.17) get paper size at from Engine to Panda directly
 1.2.03		2014.01.09 no delay for USB status monitor
 1.2.02		2013.12.20 add Media Type for Status Monitor
 1.2.01		2013.12.20 adjust Status Monitor (G_LT, B5, B6, EXECUTIVE)
 1.2.00		2013.12.19 Status Monitor
 1.1.00		2013.12.02 set version
*/

char* get_version_PJLParser()
{
   return (&PJLPARSER_VERSION[0]);
}

void outputVar(ZPJL z, Symbol command, Symbol var, char *Value);
void outputVar2(ZPJL z, Symbol command, Symbol var, char *Value);

void RemoveChar(char *Source, char Char);
extern WORD_MATCH *pwm_Paper;   /// ugly extern.
static const char*  AT_PJL = "@PJL ";
static const char*  INDENT = "\t";

Symbol  zpjlGetCommand(Uint32 argC, Modifier* args)
    {
    /* assume it's in position 0 for now...
    ** - although we should be able to search for it */
    return (argC > 0 && VT_COMMAND == args[0].type) ?
        args[0].value.symbol : NULL_SYMBOL;
    }

Symbol  zpjlGetKey(Uint32 argC, Modifier* args, Uint32 argNum)
    { return (argC >= argNum) ? args[argNum].key : NULL_SYMBOL; }

Symbol  zpjlSymbolArg(Uint32 argC, Modifier* args, Symbol argID)
    {
    Uint32  i;

    for (i = 0; i < argC; i++)
        if (VT_SYMBOL == args[i].type && argID == args[i].key)
            return args[i].value.symbol;

    return NULL_SYMBOL;
    }


/* caution -- potentially xfers ownership! */
char*  zpjlStringArgEx(Uint32 argC, Modifier* args, Symbol argID, bool xfer)
    {
    Uint32  i;

    for (i = 0; i < argC; i++)
        if (VT_STRING == args[i].type && argID == args[i].key)
            {
            char*  s = args[i].value.string;

            if (xfer)
                args[i].value.string = NULL;

            return s;
            }

    return NULL;
    }

Uint32  zpjlIntArg(Uint32 argC, Modifier* args, Symbol argID)
    {
    Uint32  i;

    for (i = 0; i < argC; i++)
        if (VT_NUMBER == args[i].type && argID == args[i].key)
            return args[i].value.number;

    return 0;
    }

/* default stuff */
/*               *--------------------------------------------------------- */

static WORD_MATCH  wm_DEFAULTGRAMMAR[] =
    {
    { IDS_PJL, {0,0}, S_COMMENT },
    END_WORD_MATCH
    };

/* ------------------------------------------------------------------------ */

static void  job_init(Job* job)
{
    job->id = BADJOB;
    job->pageCounter = 0;
    job->job_start = 0;
    job->job_end = 0;
    job->job_password = 0;
    job->start[0] = 0;
    job->end[0] = 0;
    job->refCount = 0;
    job->flags = 0;
}

void  _zpjlDumpFlags(uint8_t f)
{
	DBG_PRINTF_INFO( "not implemented \n");	
	/// job flags are deprecated, so the caller is too.
/*
    DDDPRINTF(("    "));
    DDDPRINTF(("%s", f & JF_JOBSTART ? "JF_JOBSTART " : "            "));
    DDDPRINTF(("%s", f & JF_JOBEND ? "JF_JOBEND " : "          "));
    DDDPRINTF(("%s", f & JF_PRINTSTART ? "JF_PRINTSTART " : "              "));
    DDDPRINTF(("%s", f & JF_PRINTEND ? "JF_PRINTEND " : "            "));
    DDDPRINTF(("%s", f & JF_JOBNAME ? "JF_JOBNAME " : "           "));
    DDDPRINTF(("%s", f & JF_EOJNAME ? "JF_EOJNAME " : "           "));
    DDDPRINTF(("\r\n"));
*/
}

/// cool queue modulo function used in environments...
/// deprecate environment/job
static Uint32  modQ(Uint32 n, Uint32 m)
{
    return(n == m - 1) ? 0 : n + 1;
}

/// deprecate environment/job
static Uint32  subQ(Uint32 n, Uint32 m)
{
    return(0 == n) ? m - 1 : n - 1;
}
/// deprecate environment/job
static void  job_advanceQ(ZPJL z, Job* currentJob)
{
    ZPJL_EnvItem*  ev;
    int  current;

    if (currentJob->flags & JF_JOBSTART)
    {
        ev = zpjlFindEV(z, ES_USTATUS, IDS_JOB, FALSE);
        if (NULL != ev && IDS_ON == (Symbol) ev->data.dword)
        {
            zStr  zstr;
            if (zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
            {
                zStr_putss(&zstr, IDS_USTATUS);
                zStr_putl(&zstr, SYM(IDS_JOB));
                zStr_putl(&zstr, SYM(IDS_START));

                if (currentJob->flags & JF_JOBNAME)
                {
                    zStr_puts(&zstr, IDS_NAME);
                    zStr_putstr(&zstr, "=\"");
                    zStr_putstr(&zstr, currentJob->start);
                    zStr_putl(&zstr, "\"");
                }

                zStr_putc(&zstr, FF);
                zpjlBuffer(z, zStr_xfer(&zstr)); /*xfer ownership*/
            }
        }
    }

    current = z->enQ;
    z->enQ = modQ(z->enQ, z->zpjlContext.maxJobQueue);
    if (z->enQ == z->deQ)
        z->deQ = modQ(z->deQ, z->zpjlContext.maxJobQueue);
    ASSERT(current != z->enQ);
    if (current != z->enQ) /*should never happen*/
        job_init(&z->jobQueue[z->enQ]);
}
/// deprecate environment/job
static bool  job_dequeue(ZPJL z, Job* currentJob)
{
    Uint32  mask;
    ZPJL_EnvItem*  ev;

    mask = JF_PRINTSTART|JF_PRINTEND|JF_JOBEND;
    if (mask != (currentJob->flags & mask))
        return FALSE;

    ev = zpjlFindEV(z, ES_USTATUS, IDS_JOB, FALSE);
    if (NULL != ev && IDS_ON == (Symbol) ev->data.dword)
    {
        zStr  zstr;
        if (zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
        {
            zStr_putss(&zstr, IDS_USTATUS);
            zStr_putl(&zstr, SYM(IDS_JOB));
            zStr_putl(&zstr, SYM(IDS_END));

            if (currentJob->flags & JF_EOJNAME)
            {
                zStr_puts(&zstr, IDS_NAME);
                zStr_putstr(&zstr, "=\"");
                zStr_putstr(&zstr, currentJob->end);
                zStr_putl(&zstr, "\"");
            }

            zStr_putc(&zstr, FF);
            zpjlBuffer(z, zStr_xfer(&zstr)); /*xfer ownership*/
        }
    }

    if (z->enQ != z->deQ)
    {
        job_init(currentJob);
        z->deQ = modQ(z->deQ, z->zpjlContext.maxJobQueue);
    }

    return TRUE;
}
/// deprecate environment/job
static void  safeFree(ZPJL z, char** pp)
{
    if (NULL == *pp)
        return;

    (*(z->zpjlContext.system.memFreeProc))(*pp, z->zpjlContext.system.refcon);
    *pp = NULL;
}

/* default callbacks */
/*                   *----------------------------------------------------- */
/// deprecate environment/job
static Uint32  DefaultStatusProc(Uint32 flags, RefCon refcon)
{
    return 0L;
}
/// deprecate environment/job
static Uint32  DefaultTimerChangeNotify(void* p, RefCon refcon)
{
    return 1L;
}
/// deprecate environment/job
static int  DefaultDiagnosticProc(void* p, RefCon refcon)
{
    return 0;
}

Int32  zPJL_DefaultCommandNotify(ZPJL z, Uint32 command, Uint32 argC, Modifier* argV)
{
    zStr  zstr;
    Modifier*  args = (Modifier*) argV;
    Symbol  var;
    char *Value = 0, *VarValue = 0;
    int i;
    PJL_VARS *pjl_var;
    uint32_t uint_value;
    char * str_value;
    char *g_USBProdStr;

    Int32  doDefault = (z->zpjlContext.commandNotify == NULL) ? TRUE :
                       (*z->zpjlContext.commandNotify)(z, command, argC, argV);

    if (!doDefault)
        return FALSE;

/* no need to call zStr_destruct() if we transfer ownership */
    if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
        return FALSE;

	switch (command)
	{
		/* execute the command */
		case A_EXECUTE:
			{
				// see if we have any custom cmds to execute
				//
				for(i = 0; i < num_new_grammar(); i++)
				{
					// see if the received word matches this key.
					if(zpjlGetCommand(argC,args) == get_grammar_key(i))
					{
                        
						// we found the command, now execute it.
						//
						char *tempstr;
						while((tempstr = MEM_MALLOC(160)) == NULL)
							sleep(1); // wait for memory to come available.
						memset( tempstr, 0, 160 );
						// copy the string
						strncpy( tempstr, zpjlStringArgEx(argC, args, zpjlGetCommand(argC,args), FALSE), 159 );
						// now run the program that goes with this word
						run_grammar_program(i,tempstr);
						// free the memory
						memFree(tempstr);   // free the memory

						break;  // done executing, get out.
					}
				}
				switch (zpjlGetCommand(argC, args))
				{
#ifdef IDS_ECHO
					case IDS_ECHO:
						{
							zStr_putss(&zstr, IDS_ECHO);
							zStr_putl(&zstr, zpjlStringArgEx(argC, args, IDS_ECHO, FALSE));
							zStr_putc(&zstr, FF);
							zpjlBuffer(z, zStr_xfer(&zstr)); /*xfer ownership*/
						}
						break;
#endif
#ifdef IDS_COMMENT
					case IDS_COMMENT:
						{
							zStr_putss(&zstr, IDS_COMMENT);
							zStr_putl(&zstr, zpjlStringArgEx(argC, args, IDS_COMMENT, FALSE));
							zStr_putc(&zstr, FF);
							/* davep 13-Jan-2012 ; don't echo PJL COMMENT (fix
							 * from junwang@marvell.com)
							 */
							//zpjlBuffer(z, zStr_xfer(&zstr)); /*xfer ownership*/
						}
						break;
#endif
#ifdef IDS_ENTER
						/* get ready to start a new PDL (but we only understand ZJS) */
					case IDS_ENTER:
						{
							RemoveChar(args[argC-1].value.string, ' ');
							RemoveChar(args[argC-1].value.string, '=');
							if (FindParser(args[argC-1].value.string) == 1)
								zpjlEnterLanguage(z);
						}
						break;
#endif
#ifdef IDS_DINQUIRE
						/* get an environment variable from the NVRAM (or ROM) */
					case IDS_DINQUIRE:
						{
							var = zpjlGetKey(argC, args, 1);
							Value = get_var_as_str( var, e_DefaultVar );
							outputVar(z, IDS_DINQUIRE, var, Value); 
							if(Value != NULL)
								memFree(Value);
						}
						break;
#endif
#ifdef IDS_SET
                        /* handle the set function */
                    case IDS_SET:
		    {
			    int err = 0; UNUSED_VAR(err);
			    var = zpjlGetKey(argC, args, 1);
			    if ( var != NULL_SYMBOL && (pjl_var = FindVariable(var)) ) {
				    if ( pjl_var->VarUse == e_VarString ) {
					    str_value = zpjlStringArgEx(argC, args, var, false);
					    err = pji_str_field_set_by_index( pji(), 
									      str_value, 
									      pjl_var->pji_index, 
									      false );
					    DBG_PRINTF_DEBUG("%s SET %s %s\n", __FUNCTION__, str_value, !err ? "ok" : "error" );
				    } else if ( pjl_var->VarUse == e_VarEnum ) {
					    str_value = SYM(zpjlSymbolArg(argC, args, var));
					    if ( pjl_set_string_val(pjl_var->enum_object, str_value) )
						    break; 
					    uint_value = pjl_get_val(pjl_var->enum_object);
					    if ( pjl_var->pji_index == 7 ) { 
						    ASSERT( 0 == strcmp("PAPER", pji_get_registered_name( 7 )));
						    /// 0-n map to string map array index to media_size 
						    uint_value = paper_get_entry(uint_value); 
					    } 
					    if ( pjl_var->pji_index == 8 ) { 
						    ASSERT( 0 == strcmp("MEDIATYPE", pji_get_registered_name( 8 )));
						    /// 0-n map to string map array index to media_size 
						    uint_value = papertypes_get_entry(uint_value); 
					    } 
					    if ( pjl_var->pji_index == 26 ) { 
						    ASSERT( 0 == strcmp("MEDIASOURCE", pji_get_registered_name( 26 )));
						    /// 0-n map to string map array index to media_size 
						    uint_value = papertypes_get_entry(uint_value); 
					    } 
					    pji_int_field_set_by_index( pji(), 
									uint_value, 
									pjl_var->pji_index, 
									false );
				    } else {
					    uint_value = zpjlIntArg(argC, args, var);
					    pji_int_field_set_by_index( pji(), 
									uint_value,
									pjl_var->pji_index, 
									false );
				    }
			    }
		    }
		    break;
#endif
#ifdef IDS_DEFAULT
                        /* handle the set function */
					case IDS_DEFAULT:
						{
							var = zpjlGetKey(argC, args, 1);
							if ( var != NULL_SYMBOL && (pjl_var = FindVariable(var)) ) {
								if ( pjl_var->VarUse == e_VarString ) {
									str_value = zpjlStringArgEx(argC, args, var, false);
									pji_str_field_set_by_index( 
																pji(), 
																str_value, 
																pjl_var->pji_index, 
																true );
								} else if ( pjl_var->VarUse == e_VarEnum ) {
									str_value = SYM(zpjlSymbolArg(argC, args, var));
									if ( pjl_set_string_val(pjl_var->enum_object, str_value) )
										break; // enum value invalid

									uint_value = pjl_get_val(pjl_var->enum_object);
									if ( pjl_var->pji_index == 7 ) { 
										/// 0-n map to string map array index to media_size 
										uint_value = paper_get_entry(uint_value);  
									}
									if ( pjl_var->pji_index == 8 ) { 
										uint_value = papertypes_get_entry(uint_value); 
									} 
									DBG_PRINTF_INFO( "inhwan5: 33: value=%d, index=%d \n", uint_value, pjl_var->pji_index);
									pji_int_field_set_by_index( pji(), 
																uint_value, 
																pjl_var->pji_index, 
																true );
								} else {
									uint_value = zpjlIntArg(argC, args, var);
									DBG_PRINTF_INFO( "inhwan5: 44: value=%d, index=%d \n", uint_value, pjl_var->pji_index);
									pji_int_field_set_by_index( pji(), 
																uint_value,
																pjl_var->pji_index, 
																true );
								}
							}
						}
						break;
#endif

					case IDS_INFO:
						{
							zStr  zstr;
							int index;
							Symbol  option = zpjlGetKey(argC, args, 1);
							DBG_PRINTF_INFO( "Notify INFO %d\n", option);	/* 2013.11.27		by Juny */

							/* cast to ignore const warning */
							if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
								return TRUE;
							zStr_putss(&zstr, IDS_INFO);

							switch (option)
							{
								case IDS_ID:
									zStr_putss(&zstr, IDS_ID);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									platvars_get_usb_prodstr(&g_USBProdStr);
									zStr_putl(&zstr, g_USBProdStr); // the product id
									break;
								case IDS_STATUS:
									{
										char string[7];
										DBG_PRINTF_ERR( "%s case %d\n", __FUNCTION__, option );		// 2013.12.18		by Juny
										getCurStatus(string);

										zStr_putss(&zstr, IDS_STATUS);
										zStr_putc(&zstr,CR);
										zStr_putc(&zstr,LF);        // crlf.
										zStr_putstr(&zstr,"CODE=");
										zStr_putstr(&zstr,string);

										zStr_putc(&zstr,CR);
										zStr_putc(&zstr,LF);        // crlf.
										break;
									}
								case IDS_CONFIG:
									zStr_putss(&zstr, IDS_CONFIG);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									//
									// Now put down the paper information
									//
									index = paper_get_num_media_sizes();
									if (index != 0)
									{
										zStr_putss(&zstr, IDS_PAPER);
										zStr_putstr(&zstr, "[ ");
										zStr_putn(&zstr, index);
										zStr_putc(&zstr, ' ');
										zStr_putss(&zstr, IDS_ENUMERATED);
										zStr_putl(&zstr,"]");
										i = 0;
										while (i < index)
										{
											zStr_putc(&zstr,TAB);
											zStr_putl(&zstr, SYM(pwm_Paper[i].key));
											i++;
										}
									}
									//
									// Now do the languages
									//
									zStr_putss(&zstr, IDS_LANGUAGE);
									index = GetNumLanguages();
									zStr_putstr(&zstr, "[ ");
									zStr_putn(&zstr, index);
									zStr_putc(&zstr, ' ');
									zStr_putss(&zstr, IDS_ENUMERATED);
									zStr_putl(&zstr,"]");
									while (index > 0)
									{
										zStr_putc(&zstr,TAB);
										zStr_putl(&zstr, GetPJLLanguageString(--index));
									}
								case IDS_MEMORY:
									// do memory
									zStr_putss(&zstr, IDS_MEMORY);
									zStr_putstr(&zstr, " = ");
									//zStr_putn(&zstr, hwGetRamSize());
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									if (option == IDS_MEMORY)
										break;              // if memory, done here.
									// no break here because info also includes ustatus.
									zStr_putss(&zstr, IDS_USTATUS);
									zStr_putstr(&zstr, "[ 4 ");
									zStr_putss(&zstr, IDS_ENUMERATED);
									zStr_putl(&zstr,"]");
									zStr_putc(&zstr,TAB);
									zStr_putss(&zstr, IDS_DEVICE);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.

									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr,IDS_JOB);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);

									zStr_putss(&zstr, IDS_PAGE);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);

									zStr_putss(&zstr, IDS_TIMED);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									break;


								case IDS_USTATUS:
									zStr_putss(&zstr, IDS_USTATUS);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putss(&zstr, IDS_DEVICE);
									zStr_putstr(&zstr, "= ");
									if (isUstatusDeviceOn())
									{
										zStr_putss(&zstr,IDS_ON);
									}
									else
										zStr_putss(&zstr, IDS_OFF);
									zStr_putc(&zstr, ' ');
									zStr_putstr(&zstr, "[ 3 ");
									zStr_putss(&zstr, IDS_ENUMERATED);
									zStr_putl(&zstr,"]");
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_ON);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_OFF);

									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_VERBOSE);

									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putss(&zstr,IDS_JOB);
									zStr_putstr(&zstr, "= ");
									if (isUstatusJobOn())
									{
										zStr_putss(&zstr,IDS_ON);
									}
									else
										zStr_putss(&zstr, IDS_OFF);

									zStr_putc(&zstr, ' ');
									zStr_putstr(&zstr, "[ 2 ");
									zStr_putss(&zstr, IDS_ENUMERATED);
									zStr_putl(&zstr,"]");
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_ON);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_OFF);

									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putss(&zstr, IDS_PAGE);
									zStr_putstr(&zstr, "= ");
									if (isUstatusPageOn())
									{
										zStr_putss(&zstr,IDS_ON);
									}
									else
										zStr_putss(&zstr, IDS_OFF);
									zStr_putc(&zstr, ' ');
									zStr_putstr(&zstr, "[ 2 ");
									zStr_putss(&zstr, IDS_ENUMERATED);
									zStr_putl(&zstr,"]");
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_ON);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);
									zStr_putss(&zstr, IDS_OFF);

									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.

									zStr_putss(&zstr, IDS_TIMED);
									zStr_putstr(&zstr, "= ");
									if ((index = UstatusTimedState()) != 0)
									{
										char Time[30];
										minSprintf(Time, "%d", index);
										zStr_putl(&zstr, Time);
									}
									else
										zStr_putc(&zstr, '0');
									zStr_putc(&zstr, ' ');
									zStr_putstr(&zstr, "[ 2 ");
									zStr_putss(&zstr, IDS_RANGE);
									zStr_putl(&zstr,"]");
									zStr_putc(&zstr, TAB);
									zStr_putc(&zstr, '5');
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putc(&zstr, TAB);
									zStr_putl(&zstr, "300");
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									break;
								case IDS_PAGECOUNT:
									{
										char tempStr[10];
										zStr_putss(&zstr, IDS_PAGECOUNT);
										zStr_putc(&zstr,CR);
										zStr_putc(&zstr,LF);        // crlf.
#ifdef HAVE_OID
										oid_get_string(OID_PRINT_HEAD_TOTAL_PAGES_PRINTED, 0, tempStr,10);
#else
										tempStr[0] = '0';
										tempStr[1] = '\0';
#endif
										zStr_putstr(&zstr, tempStr);
										zStr_putc(&zstr,CR);
										zStr_putc(&zstr,LF);        // crlf.
									}
									break;
								case IDS_VARIABLES:
									zStr_putss(&zstr, IDS_VARIABLES);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									index = GetNumVars();
									while (index > 0)
									{
										zStr_putstr(&zstr, GetVarName(--index));
										zStr_putstr(&zstr," = ");

										switch (FindVariableType(GetVarKey(index)))
										{
											case e_VarEnum:
						    
												VarValue = get_var_as_str(GetVarKey(index), e_NormalVar);
												if (VarValue)
													zStr_putstr(&zstr, VarValue); // if a valid value
												else
													zStr_putstr(&zstr,"Bad Value");
												memFree(VarValue);
												//
												// Now find the number of choices.
												//
												i = 0;
												while (NULL != GetVarEnum(GetVarKey(index), i++));

												zStr_putstr(&zstr, " [ ");
												zStr_putn(&zstr, --i);
												zStr_putc(&zstr, ' ');
												zStr_putss(&zstr, IDS_ENUMERATED);
												zStr_putl(&zstr,"]");
												//
												// Now put out all the enumerated choices.
												//
												i = 0;
												while (NULL != (Value = GetVarEnum(GetVarKey(index),i++)))
												{
													zStr_putc(&zstr, TAB);
													zStr_putl(&zstr, Value);
												}
												break;
											case e_VarNumber:

												VarValue = get_var_as_str(GetVarKey(index), e_NormalVar);
												if (VarValue)
													zStr_putstr(&zstr, VarValue); // if a valid value
												else
													zStr_putstr(&zstr,"Bad Value");
												memFree(VarValue);

												zStr_putstr(&zstr, " [ ");
												zStr_putn(&zstr, 2);
												zStr_putc(&zstr, ' ');
												zStr_putss(&zstr, IDS_RANGE);
												zStr_putl(&zstr,"]");
												zStr_putc(&zstr, TAB);
												zStr_putn(&zstr, ReturnNumVarMin(GetVarKey(index)));
												zStr_putc(&zstr,CR);
												zStr_putc(&zstr,LF);        // crlf.
												zStr_putc(&zstr, TAB);
												zStr_putn(&zstr, ReturnNumVarMax(GetVarKey(index)));
												zStr_putc(&zstr,CR);
												zStr_putc(&zstr,LF);        // crlf.
												break;
											default:
												break;
										}
									}

							}
							zStr_putc(&zstr, FF);
							DBG_PRINTF_INFO("Out is \n %s\n", zstr.buffer);	// 2013.11.27 	by Juny */
							doOutput(zstr.buffer);  // put it out.
							break;
						}


#ifdef IDS_EOJ
					case IDS_EOJ:
						{
							char*  s = zpjlStringArg(argC, args, IDS_NAME); /*xfer ownership*/
							Job*  job = NULL;
							int  qPos = z->enQ;
							Uint32  i;

							/* find the first job which has been started but not EOJ'd */
							for (i = 0; i < z->zpjlContext.maxJobQueue && NULL == job; i++)
							{
								if ((z->jobQueue[qPos].flags & JF_JOBSTART) &&
									!(z->jobQueue[qPos].flags & JF_JOBEND))
									job = &z->jobQueue[qPos];
								qPos = subQ(qPos, z->zpjlContext.maxJobQueue);
							}
							DBG_PRINTF_INFO("[IDS_EOJ] e:%d, d:%d, q:%d\r\n", z->enQ, z->deQ, qPos);
							if (NULL != job)
							{
								//DDUMPFLAGS(job->flags);

								if (NULL != s)
								{
									strncpy(job->end, s, JOBNAMESIZE - 1);
									job->end[JOBNAMESIZE - 1] = 0;
									job->flags |= JF_EOJNAME;
								}
								else
									job->end[0] = 0;

								job->flags |= JF_JOBEND;
								if (job_dequeue(z, job))
								{
									/*reset current environment*/
									zPJL_DefaultCommandNotify(z, IDS_RESET, 0, NULL);
									job->pageCounter = 0;
								}

								DBG_PRINTF_INFO("          e:%d, d:%d, q:%d\r\n", z->enQ, z->deQ, qPos);
								//DDUMPFLAGS(job->flags);
							}
							else
							{
								DBG_PRINTF_INFO("couldn't find matching job\r\n");
							}

							safeFree(z, &s);
						}
#endif

#ifdef IDS_INITIALIZE
					case IDS_INITIALIZE:
						{
							uint32_t varCnt;
							varCnt = GetNumVars();
							while (varCnt > 0)
							{
								switch (FindVariableType(GetVarKey(varCnt-1)))
								{
									/* lsptodo: factory reset ????
									case e_VarEnum:
										SetNormalEnumVar(GetVarKey(varCnt-1),0, e_FactDefaultVar);
										break;
									case e_VarNumber:

										SetNormalNumVar(GetVarKey(varCnt-1),0, e_FactDefaultVar);
										break;
									*/

									default:
										break;
								}
								varCnt--;
							}
						}
						break;
#endif

#ifdef IDS_INQUIRE
                        /* get an environment variable from RAM (or NVRAM or ROM) */
					case IDS_INQUIRE:
						{
//		2013.12.17	by Juny
							DBG_PRINTF_ERR("Enter parsing Inquire %d args %s key=%d\n", IDS_INQUIRE, args, args[1].key );	// 2013.12.17 	by Juny */
							var = zpjlGetKey(argC, args, 1);
//							Value = get_var_as_str( var, e_NormalVar );
							Value = get_var_as_str_Sindoh( var, e_NormalVar );
							DBG_PRINTF_ERR("Inquire %s \n", Value );	// 2013.11.29 	by Juny */
							outputVar2(z, IDS_INQUIRE, var, Value); 
							memFree(Value);
						}
						break;
#endif

#ifdef IDS_JOB
                        /* named options real handy here */
					case IDS_JOB:
					{
						char *s               = zpjlStringArg(argC, args, IDS_NAME); /*xfer ownership*/
						uint32_t job_start    = zpjlIntArg(argC, args, IDS_START);
						uint32_t job_end      = zpjlIntArg(argC, args, IDS_END);
						uint32_t job_password = zpjlIntArg(argC, args, IDS_PASSWORD);
						char *display         = zpjlStringArg(argC, args, IDS_DISPLAY);

						// reset variables to defaults.
						pji_local_copy_init( &(pji()->local), &(pji()->local) );
						// existing variables... lsptodo fix these.
						if (s) { 
							pji_str_field_set_by_name( pji(), 
														JOBNAME, 
														s,
														false); // not default
							PrintJobInfo_Set_jobname( pji(), s );
						}
						PrintJobInfo_Set_start_end( pji(), job_start, job_end );

						if (display) {
							// lsptodo: send "job" info to control panel.
						}
						/// lsptodo: password is standard?
						UNUSED_VAR( job_password );
			
/*
			pji_int_field_set_by_name( pji(), 
						   PASSWORD, 
						   job_password,
						   false); // not default
*/
			
					}
					break;
#endif

#ifdef IDS_RESET
					case IDS_RESET: 
						// reset variables to defaults.
						pji_local_copy_init( &pji()->local, &pji()->local );
						break;
#endif

#ifdef IDS_USTATUS
                        /* change USTATUS config */
					case IDS_USTATUS:
						{
							//static const Symbol  enumEV_OnOff[] = { IDS_ON, IDS_OFF};
							//static const Symbol  enumEV_OnOffVerbose[] = { IDS_ON, IDS_OFF, IDS_VERBOSE};
							zStr  zstr;
							Symbol  var = zpjlGetKey(argC, args, 1);

							if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
								return TRUE;
							zStr_putss(&zstr, IDS_USTATUS);

							switch (var)
							{
								case IDS_DEVICE:
									if ((zpjlSymbolArg(argC, args, var) == IDS_ON) ||
										(zpjlSymbolArg(argC, args, var) == IDS_VERBOSE))
									{
										setUstatusDevice(true);
									}
									else if (zpjlSymbolArg(argC, args, var) == IDS_OFF)
									{
										setUstatusDevice(false);
									}
									zStr_putss(&zstr, IDS_DEVICE);
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);        // crlf.
									zStr_putl(&zstr, "CODE=10001"); // the product id
									zStr_putc(&zstr,CR);
									zStr_putc(&zstr,LF);
									zStr_putc(&zstr, FF);
									DBG_PRINTF_INFO("always online is %s\n", zstr.buffer);
									doOutput(zstr.buffer);  // put it out.
									break;

								case IDS_JOB:
									if ((zpjlSymbolArg(argC, args, var) == IDS_ON) ||
										(zpjlSymbolArg(argC, args, var) == IDS_VERBOSE))
									{
										setUstatusJob(true);
									}
									else if (zpjlSymbolArg(argC, args, var) == IDS_OFF)
									{
										setUstatusJob(false);
									}
									break;

								case IDS_PAGE:
									if ((zpjlSymbolArg(argC, args, var) == IDS_ON) ||
										(zpjlSymbolArg(argC, args, var) == IDS_VERBOSE))
									{
										setUstatusPage(true);
									}
									else if (zpjlSymbolArg(argC, args, var) == IDS_OFF)
									{
										setUstatusPage(false);
									}
									break;

								case IDS_TIMED:
									{
										int seconds;

										seconds = zpjlIntArg(argC, args, var);
										setUstatusTimed( seconds );    // get it started.  
									}
									break;
							}

						} /*IDS_USTATUS*/
						break;
#endif

#ifdef IDS_USTATUSOFF
                        /* turn off all USTATUS */
					case IDS_USTATUSOFF:
						{
							setUstatusTimed(0); // turn it off
							setUstatusJob(false);
							setUstatusPage(false);
							setUstatusDevice(false);

						} /*IDS_USTATUS*/
						break;
#endif

					default:
						break;
				}
			}
			break;
/// deprecate environment/job
		case A_STATUSCHANGE: /* USTATUS [DEVICE|TIMED] output */
			{
				Symbol  s = zpjlSymbolArg(argC, args, IDS_USTATUS);
				bool callInfoStatus = FALSE;
				DBG_PRINTF_ERR("Status change \n");	// 2013.12.18 	by Juny */

				if (IDS_DEVICE == s)
				{
					ZPJL_EnvItem*  ev = zpjlFindEV(z, ES_USTATUS, IDS_DEVICE, FALSE);
					callInfoStatus =
					(NULL != ev && IDS_ON == (Symbol) ev->data.dword);
				}
				else if (IDS_TIMED == s)
				{
					ZPJL_EnvItem*  ev = zpjlFindEV(z, ES_USTATUS, IDS_TIMED, FALSE);
					callInfoStatus =
					(NULL != ev && 0 < rangeValue(ev));
				}

				if (callInfoStatus)
				{
					/* emulate "@PJL INFO STATUS" - with a twist */
					Modifier  arg[3];
					/*arg[0].key = IDS_INFO;*/
					arg[0].value.symbol = IDS_INFO;
					arg[0].type = VT_COMMAND;

					arg[1].key = IDS_INFO;
					arg[1].value.symbol = IDS_STATUS;
					arg[1].type = VT_SYMBOL;

					arg[2].key = IDS_USTATUS; /*here's the twist*/
					arg[2].value.symbol = zpjlSymbolArg(argC, args, IDS_USTATUS);
					arg[2].type = VT_SYMBOL;

					zPJL_DefaultCommandNotify(z, A_EXECUTE, 3, arg);
				}
			}
			break;

#if 0
/// deprecate environment/job
		case A_TIMED: /* USTATUS TIMED output */
			{
				Modifier  arg[1];
				arg[0].key = IDS_USTATUS;
				arg[0].value.symbol = IDS_TIMED;
				arg[0].type = VT_SYMBOL;
				zPJL_DefaultCommandNotify(z, A_STATUSCHANGE, 1, (void**) arg);
			}
			break;
#endif
/// deprecate environment/job
		case A_JOBSTARTED: /* USTATUS JOB output */
			{
				Job*  job = &z->jobQueue[z->enQ];
				DBG_PRINTF_INFO("[IDS_JOB] e:%d, d:%d\r\n", z->enQ, z->deQ);
				//DDUMPFLAGS(job->flags);

				job->id = zpjlIntArg(argC, args, IDS_JOB);
				job->flags |= JF_PRINTSTART;
				job->flags &= ~JF_PRINTEND;
				job_advanceQ(z, job);

				DBG_PRINTF_INFO("               e:%d, d:%d\r\n", z->enQ, z->deQ);
				//DDUMPFLAGS(job->flags);
			}
			break;
/// deprecate environment/job
		case A_JOBCOMPLETE: /* USTATUS JOB output */
			{
				Job*  job = &z->jobQueue[z->deQ];
				DBG_PRINTF_INFO("[A_JOBCOMPLETE] e:%d, d:%d", z->enQ, z->deQ);
				DBG_PRINTF_INFO(", id(1):%d, id(2)d:%d\r\n", job->id, zpjlIntArg(argC, args, IDS_JOB));
				//DDUMPFLAGS(job->flags);

				job->flags |= JF_PRINTEND;
				job_dequeue(z, job);

				DBG_PRINTF_INFO("                e:%d, d:%d", z->enQ, z->deQ);
				DBG_PRINTF_INFO(", id(1):%d, id(2)d:%d\r\n", job->id, zpjlIntArg(argC, args, IDS_JOB));
				//DDUMPFLAGS(job->flags);
			}
			break;
/// deprecate environment/job
		case A_PAGE: /* USTATUS PAGE output */
			{
				ZPJL_EnvItem*  ev = zpjlFindEV(z, ES_USTATUS, IDS_PAGE, FALSE);
				Job*  job = &z->jobQueue[z->deQ];

				if (job->flags & JF_PRINTSTART)
					job->pageCounter++;

				if (NULL != ev && IDS_ON == (Symbol) ev->data.dword)
				{
					zStr_putss(&zstr, IDS_USTATUS);
					zStr_putl(&zstr, SYM(IDS_PAGE));
					zStr_putn(&zstr, zpjlIntArg(argC, args, IDS_PAGE));
					zStr_putl(&zstr, NULL);
					zStr_putc(&zstr, FF);
					zpjlBuffer(z, zStr_xfer(&zstr)); /*xfer ownership*/
				}
			}
			break;

		default:
			break;
	}

/* safe even if ownership transferred */
    zStr_destruct(&zstr);

    return TRUE;
}

 
 
/* FUNCTION NAME: RemoveChar */
 
/** 
 * \brief Remove all characters of this type from the string.
 * 
 * \param Source The string from which to remove the characters.
 * 
 * \param Char The character to remove.
 * 
 * \retval None
 * 
 * \author Dirk Bodily
 * 
 * \date 11/20/2007
 * 
 **/
void RemoveChar(char *Source, char Char)
{
    char *TempStr;

    while((TempStr = strchr(Source, Char)) != NULL)
    {
        strcpy(TempStr, TempStr+1);     // get rid of the character.
    }
}

Int32  zPJL_DefaultParserNotify(ZPJL z, Uint32 command, Uint32 argC, void** argV)
    {
    Int32  doDefault = (z->zpjlContext.parserNotify == NULL) ? TRUE :
         (*z->zpjlContext.parserNotify)(z, command, argC, argV);

    if (!doDefault)
        return FALSE;

    /* buffer up the tokens */
    switch (command)
        {
        case A_COMMAND:
            if (z->argCount < z->zpjlContext.maxArgs)
                {
                z->args[z->argCount].type = VT_COMMAND;
                z->args[z->argCount].value.symbol =
                z->args[z->argCount].key =
                    (argC > 0) ? (Symbol)(uintptr_t) argV[0] : NULL_SYMBOL;
                /* default the next arg's key to the command symbol */
                if (z->argCount + 1 < z->zpjlContext.maxArgs)
                    z->args[z->argCount + 1].key = z->args[z->argCount].value.symbol;
                z->argCount++;
                }
            break;

        case A_KEY:
            if (z->argCount < z->zpjlContext.maxArgs)
                z->args[z->argCount].key = (argC > 0) ? (Symbol)(uintptr_t) argV[0] : NULL_SYMBOL;
            break;

        case A_VALUE:
            {
            if (z->argCount < z->zpjlContext.maxArgs)
                {
                z->args[z->argCount].type = VT_SYMBOL;
                z->args[z->argCount].value.symbol =
                    (argC > 0) ? (Symbol)(uintptr_t) argV[0] : NULL_SYMBOL;
                z->argCount++;
                }
            }
            break;

        case A_NUMBER:
            {
            if (z->argCount < z->zpjlContext.maxArgs)
                {
                z->args[z->argCount].type = VT_NUMBER;
                z->args[z->argCount].value.number =
                    (argC > 0) ? (Uint32) argV[0] : 0;
                z->argCount++;
                }
            }
            break;

        case A_STRING:
            {
            if (z->argCount < z->zpjlContext.maxArgs)
                {
                char*  sourceString = (argC > 0) ? (char*) argV[0] : NULL;

                if (NULL != sourceString)
                    {
                    // get rid of quotes in the source string.  This allows the rest
                    // to work correctly.
                    //
                    RemoveChar(sourceString,'"');
                    RemoveChar(sourceString,'\'');
                    
                    z->args[z->argCount].value.string = (char*)
                        (*(z->zpjlContext.system.memAllocProc))(strlen(sourceString) + 1,
                            DEFAULT_ALLOCFLAGS, z->zpjlContext.system.refcon);

                    if (NULL != z->args[z->argCount].value.string)
                        {
                        strcpy(z->args[z->argCount].value.string, sourceString);
                        z->args[z->argCount].type = VT_STRING;
                        z->argCount++;
                        }
                    }
                }
            }
            break;

        /* boohoo! */
        case A_COMPLAIN:
            {
            /* eventually we might have a more detailed response */
            /* depending on the args; for now, the response is always '?' */
            zStr  zstr;
            if (zStr_construct(&zstr, &z->zpjlContext.system, "?\r\n"))
                /* no need to call zStr_destruct() if we transfer ownership */
                zpjlBuffer(z, zstr.buffer); /*xfer ownership*/
            }
            break;

        /* unbuffer up tokens */
        case A_UNBUFFER:
            {
            Uint32  i;
            for (i = 0; i < z->argCount; i++)
                {
                z->args[i].key = NULL_SYMBOL;

                switch (z->args[i].type)
                    {
                    case VT_STRING:
                        if (NULL != z->args[i].value.string)
                            {
                            (*(z->zpjlContext.system.memFreeProc))(
                                z->args[i].value.string,
                                z->zpjlContext.system.refcon);
                            z->args[i].value.string = NULL;
                            }
                        break;

                    default:
                        z->args[i].value.number = 0;
                    }
                }
            z->argCount = 0;
            }
            break;

        default:
            break;
        }

    return TRUE;
    }

Int32  zPJL_DefaultErrorNotify(ZPJL z, Uint32 command, Uint32 argC, void** argV)
    {
    Int32  doDefault = (z->zpjlContext.errorNotify == NULL) ? TRUE :
         (*z->zpjlContext.errorNotify)(z, command, argC, argV);

    if (!doDefault)
        return FALSE;

    switch (command)
        {
        case D_DEBUG_MESSAGE:
            {
#ifdef WINDOWS
            Uint32  i;
            for (i = 0; i < argC; i++)
                OutputDebugStr((char*) argV[i]);
#endif
            }
            break;

        default:
            break;
        }

    return TRUE;
    }


/* class methods */
/*               *--------------------------------------------------------- */

Int32  zpjlConstruct(zpjlContextInfo* zpjlContext, ZPJL* zpjl)
    {
    Int32  returnValue = ZPJL_OK;
    _ZPJL*  z;
    Uint32  i;

    *zpjl = BAD_ZPJL;

    if (NULL == zpjlContext || zpjlContext->system.memAllocProc == NULL
        || zpjlContext->system.memFreeProc == NULL)
        return ZPJL_CONSTRUCT_FAILED;

    z = (_ZPJL*) (*zpjlContext->system.memAllocProc)(
        sizeof(_ZPJL), DEFAULT_ALLOCFLAGS, zpjlContext->system.refcon);
    if (NULL == z)
        return ZPJL_CONSTRUCT_FAILED;

    z->zpjlContext = *zpjlContext;

    /* lexer/parser initialization */
    z->wm_FIRST = z->zpjlContext.rootGrammar;
    z->cPrev = -1;
    z->State = 0;
    z->inWord = 0;
    z->pMatchList = NULL;
    z->First = TRUE;

    if (NULL == z->zpjlContext.rootGrammar)
        z->zpjlContext.rootGrammar = wm_DEFAULTGRAMMAR;

    /* other initialization */
    z->argCount = 0;
    if (0 == z->zpjlContext.maxArgs)
        z->args = NULL;
    else
        {
        z->args = (Modifier*) (*zpjlContext->system.memAllocProc)(
            sizeof(Modifier) * z->zpjlContext.maxArgs,
            DEFAULT_ALLOCFLAGS, zpjlContext->system.refcon);
        if (NULL == z->args)
            {
            z->zpjlContext.maxArgs = 0;
            returnValue = ZPJL_CONSTRUCT_FALTERED;
            }

        for (i = 0; i < z->zpjlContext.maxArgs; i++)
            {
            z->args[i].key = NULL_SYMBOL;
            z->args[i].value.number = 0;
            }
        }

    z->outboundSP = -1;
    z->inProgress = z->inProgressBP = NULL;
    if (0 == z->zpjlContext.maxOutbound)
        z->outbound = NULL;
    else
        {
        z->outbound = (char**) (*zpjlContext->system.memAllocProc)(
            sizeof(char*) * z->zpjlContext.maxOutbound,
            DEFAULT_ALLOCFLAGS, zpjlContext->system.refcon);
        if (NULL == z->outbound)
            {
            z->zpjlContext.maxOutbound = 0;
            returnValue = ZPJL_CONSTRUCT_FALTERED;
            }
        }

    if (z->zpjlContext.maxJobQueue < 2)
        z->zpjlContext.maxJobQueue = 2;
    z->jobQueue = (Job*) (*zpjlContext->system.memAllocProc)(
        sizeof(Job) * z->zpjlContext.maxJobQueue,
        DEFAULT_ALLOCFLAGS, zpjlContext->system.refcon);
    z->enQ = 0;
    z->deQ = 0;
    /* init the jobs */
    for (i = 0; i < z->zpjlContext.maxJobQueue; i++)
        job_init(&z->jobQueue[i]);

    z->enterLanguage = FALSE;

    if (zpjlContext->system.diagnosticProc == NULL)
        z->zpjlContext.system.diagnosticProc = DefaultDiagnosticProc;

    if (zpjlContext->system.getStatus == NULL)
        z->zpjlContext.system.getStatus = DefaultStatusProc;

    if (zpjlContext->system.timerChange == NULL)
        z->zpjlContext.system.timerChange = DefaultTimerChangeNotify;

/* USB Status Monitor 		2013.12.17		by Juny */
/*
    if (NULL == z->zpjlContext.parserNotify)
        z->zpjlContext.parserNotify = zPJL_DefaultParserNotify;
*/

/*
    if (NULL == z->zpjlContext.commandNotify)
        z->zpjlContext.commandNotify = zPJL_DefaultCommandNotify;
    if (NULL == z->zpjlContext.parserNotify)
        z->zpjlContext.parserNotify = zPJL_DefaultParserNotify;
    if (NULL == z->zpjlContext.errorNotify)
        z->zpjlContext.errorNotify = zPJL_DefaultErrorNotify;
*/

    /* client constructor hook? */
    /* ... (*zpjl)->zpjlContext.zpjlCtorProc) */

    *zpjl = z;

    return returnValue ; // ZPJL_OK;
    }

Int32  zpjlDestruct(ZPJL* zpjl)
    {
    MemFreeProcT  memFreeProc;
    void*  refcon;

    if (NULL == zpjl || (*zpjl)->zpjlContext.system.memFreeProc == NULL)
        return ZPJL_DESTRUCT_FAILED;

    /* client destructor hook? */
    /* ... (*zpjl)->zpjlContext.zpjlDtorProc) */

    /* don't really need separate proc ptr, but it's easier to read */
    memFreeProc = (*zpjl)->zpjlContext.system.memFreeProc;
    refcon = (*zpjl)->zpjlContext.system.refcon;

    if (NULL != (*zpjl)->args)
        (*memFreeProc)((*zpjl)->args, refcon);

    if (NULL != (*zpjl)->inProgressBP)
        (*memFreeProc)((*zpjl)->inProgressBP, refcon);

    if (NULL != (*zpjl)->outbound)
        (*memFreeProc)((*zpjl)->outbound, refcon);

/*  if (NULL != (*zpjl)->job_name)
        (*memFreeProc)((*zpjl)->job_name, refcon);

    if (NULL != (*zpjl)->eoj_name)
        (*memFreeProc)((*zpjl)->eoj_name, refcon);*/

    if (NULL != (*zpjl)->jobQueue)
        (*memFreeProc)((*zpjl)->jobQueue, refcon);

    if ((*memFreeProc)(*zpjl, (*zpjl)->zpjlContext.system.refcon))
        *zpjl = BAD_ZPJL;

    return ZPJL_OK;
    }

/* -------------------------------------------------------------------------- */

/* call these to generate outbound status data */
/* (since zPJL_DefaultCommandNotify() is public, it could be called directly,
** these are just helpers)
*/
void  zpjlStatus(ZPJL zpjl)
    {
    Modifier  arg[1];
    arg[0].key = IDS_USTATUS;
    arg[0].value.symbol = IDS_DEVICE;
    arg[0].type = VT_SYMBOL;
    zPJL_DefaultCommandNotify(zpjl, A_STATUSCHANGE, 1, arg);
    }

void  zpjlJobStarted(ZPJL zpjl, Uint32 jobId)
    {
    Modifier  arg[1];
    arg[0].key = IDS_JOB;
    arg[0].value.number = jobId;
    arg[0].type = VT_NUMBER;
    zPJL_DefaultCommandNotify(zpjl, A_JOBSTARTED, 1, arg);
    }

void  zpjlJobComplete(ZPJL zpjl, Uint32 jobId, Uint32 whichCopy)
    {
    Modifier  arg[1];
    arg[0].key = IDS_JOB;
    arg[0].value.number = jobId;
    arg[0].type = VT_NUMBER;
/*  arg[1].key = IDS_COUNTER;
    arg[1].value.number = whichCopy;
    arg[1].type = VT_NUMBER;*/
/*  arg[2].key = IDS_PAGES;
    arg[2].value.number = pagesPrinted;
    arg[2].type = VT_NUMBER;*/
    zPJL_DefaultCommandNotify(zpjl, A_JOBCOMPLETE, 1, arg);
    }

void  zpjlPage(ZPJL zpjl, Uint32 n, Uint32 jobId, Uint32 whichCopy)
    {
    Modifier  arg[1];
    arg[0].key = IDS_PAGE;
    arg[0].value.number = n;
    arg[0].type = VT_NUMBER;
    zPJL_DefaultCommandNotify(zpjl, A_PAGE, 1, arg);
    }

void  zpjlTick(ZPJL zpjl)
    {
    Modifier  arg[1];
    arg[0].key = IDS_USTATUS;
    arg[0].value.symbol = IDS_TIMED;
    arg[0].type = VT_SYMBOL;
    zPJL_DefaultCommandNotify(zpjl, A_STATUSCHANGE, 1, arg);
/*  zPJL_DefaultCommandNotify(zpjl, A_TIMED, 0, NULL); */
    }

/* -------------------------------------------------------------------------- */

int  zpjlBuffer(ZPJL z, char* s)
    {

    if (0 == z->zpjlContext.maxOutbound || NULL == z->outbound)
        return FALSE;
/*
    if (z->outboundSP == (int) z->zpjlContext.maxOutbound)
        {
		(*z->zpjlContext.system.diagnosticProc)( "Outbound queue full, dropping oldest entry.", 
			z->zpjlContext.system.refcon );
        (*z->zpjlContext.system.memFreeProc)(z->outbound[0],
            z->zpjlContext.system.refcon);


        for (i = 0; i < z->outboundSP - 1; i++)
            z->outbound[i] = z->outbound[i + 1];

        z->outbound[z->outboundSP - 1] = s;
        }
    else
        z->outbound[++z->outboundSP] = s;
*/
    doOutput(s);    // write it out.
    zPJL_DefaultCommandNotify(z, A_DATAREADY, 0, NULL);

    return TRUE;
    }


int  zpjlGotData(ZPJL z)
    {
    if (0 == z->zpjlContext.maxOutbound || NULL == z->outbound)
        return FALSE;

    return (z->outboundSP >= 0) || (NULL != z->inProgressBP);
    }


/*dequeue will skip and free empty strings*/
static char*  dequeue(ZPJL z)
    {
    if (0 == z->zpjlContext.maxOutbound || NULL == z->outbound)
        return NULL;

    while (z->outboundSP >= 0)
        {
        int  i;
        char*  result = z->outbound[0]; /*grab from bottom*/

        z->outboundSP--;
        for (i = 0; i <= z->outboundSP; i++)
            z->outbound[i] = z->outbound[i + 1];

        if (NULL == result)
            continue;

        if (0 != result[0])
            return result;

        /*empty string - skip it*/
        (*z->zpjlContext.system.memFreeProc)(result,
            z->zpjlContext.system.refcon);
        }
    return NULL;
    }

Byte  zpjlGetc(ZPJL z, int* more)
    {
    Byte  returnValue = 0;

    if (NULL == z->inProgress)
        {
        /*dequeue will skip and free empty strings*/
        z->inProgress = z->inProgressBP = dequeue(z);
        }

    if (NULL == z->inProgress)
        {
        if (NULL != more)
            *more = (NULL != z->inProgress);
        /*we erroneously thought there was more data*/
        return 0;
        }
        
    returnValue = *z->inProgress++;
    while (0 == returnValue)
        {
        (*z->zpjlContext.system.memFreeProc)(z->inProgressBP,
            z->zpjlContext.system.refcon);
        z->inProgress = z->inProgressBP = dequeue(z);
        if (NULL == z->inProgress)
            break;
        returnValue = *z->inProgress++;
        }

    if (NULL != more)
        *more = (NULL != z->inProgress);

    return returnValue;
    }

/* -------------------------------------------------------------------------- */

void  zpjlEnterLanguage(ZPJL z)
    { z->enterLanguage = TRUE; }

RefCon  zpjlRefcon(ZPJL zpjl)
    {
    return zpjl->zpjlContext.system.refcon;
    }

zpjlSystem *zpjlSys(ZPJL zpjl)
{
	return &zpjl->zpjlContext.system;
}


/* handy output helpers */
/*                      *---------------------------------------------------- */
void outputVar(ZPJL z, Symbol command, Symbol var, char *Value)
{
    zStr zstr;

    if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
        return;

    zStr_putss(&zstr, command);
    zStr_putss(&zstr, var);
    zStr_putl(&zstr, "=");
    zStr_putl(&zstr, Value);    // put in the value.
    zStr_putc(&zstr, FF);
    doOutput(zstr.buffer);  // send the thing on.

}

void outputVar2(ZPJL z, Symbol command, Symbol var, char *Value)
{
    zStr zstr;

    if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
        return;

    zStr_putss(&zstr, command);
    zStr_puts(&zstr, var);
    zStr_putc(&zstr, CR);
    zStr_putc(&zstr, LF);		// test with LF
    zStr_putl(&zstr, Value);    // put in the value.
    zStr_putc(&zstr, FF);
    DBG_PRINTF_ERR("PJL out string2 \n %s \n", zstr.buffer );	// 2013.12.19 	by Juny */
    doOutput(zstr.buffer);  // send the thing on.
}

void  outputEV(ZPJL z, Symbol command, ZPJL_EnvItem* ev)
    {
    zStr  zstr;
    EvaluateProcT  proc;

    if (NULL == ev)
        return;

    /* cast to ignore const warning */
    if (!zStr_construct(&zstr, &z->zpjlContext.system, AT_PJL))
        return;

    zStr_putss(&zstr, command);
    zStr_putl(&zstr, SYM(ev->id));

    switch (ev->type)
        {
        case EIT_ENUM:
            zStr_putl(&zstr, SYM(ev->data.dword));
            break;

        case EIT_NUMBER:
            zStr_putn(&zstr, ev->data.dword);
            zStr_putl(&zstr, NULL);
            break;

        case EIT_RANGEVAL:
            zStr_putn(&zstr, rangeValue(ev));
            zStr_putl(&zstr, NULL);
            break;

        case EIT_ENUM_RO:
            if ((proc = ev->data.proc) != NULL)
                {
                Symbol  s;
                if (NULL_SYMBOL != (s = (*proc)(z)))
                    zStr_putl(&zstr, SYM(s));
                }
            break;

        case EIT_INT_RO:
            if ((proc = ev->data.proc) != NULL)
                { 
                zStr_putn(&zstr, (*proc)(z));
                zStr_putl(&zstr, NULL);
                }
            break;

        case EIT_CSTR:
            zStr_putl(&zstr, ev->data.ptr);
            break;
        }

    zStr_putc(&zstr, FF);

    /* no need to call zStr_destruct() if we transfer ownership */
    zpjlBuffer(z, zstr.buffer); /*xfer ownership*/
    }

/* output config list */
/* <value> sym may be -1 if not used; otherwise, it's and index into the table */
void  outputCL(zStr* zstr, Symbol category,  Symbol* list, Uint32 listSize, int value)
    {
    Uint32  i;

    /*header*/
    if (-1 == value || (Uint32) value >= listSize)
        zStr_putss(zstr, category);
    else
        {
        zStr_puts(zstr, category);
        zStr_putc(zstr, '=');
        zStr_putss(zstr, list[value]);
        }
    zStr_putc(zstr, '[');
    zStr_putn(zstr, listSize);
    zStr_putc(zstr, ' ');
    zStr_puts(zstr, IDS_ENUMERATED);
    zStr_putc(zstr, ']');
    zStr_putl(zstr, NULL);

    for (i = 0; i < listSize; i++)
        {
        zStr_putstr(zstr, INDENT);
        zStr_putl(zstr, SYM(list[i]));
        }
    }

void  outputKeqV(zStr* zstr, Symbol id, Uint32 value)
    {
    zStr_puts(zstr, id);
    zStr_putc(zstr, '=');
    zStr_putn(zstr, value);
    zStr_putl(zstr, NULL);
    }

/* INFO VARIABLE output for a range variable */
void  outputIV_range(ZPJLENV zpe, zStr* zstr, Symbol id)
    {
    ZPJL_EnvItem*  ev = zpjlFindVar(zpe, id);

    zStr_puts(zstr, id);

    if (NULL == ev)
        {
        zStr_putl(zstr, NULL);
        zStr_putc(zstr, '?');
        zStr_putl(zstr, NULL);
        return;
        }

    zStr_putc(zstr, '=');
    zStr_putn(zstr, rangeValue(ev));
    zStr_putstr(zstr, " [2 ");
    zStr_puts(zstr, IDS_RANGE);
    zStr_putc(zstr, ']');
    zStr_putl(zstr, NULL);

    zStr_putstr(zstr, INDENT);
    zStr_putn(zstr, rangeLower(ev));
    zStr_putl(zstr, NULL);

    zStr_putstr(zstr, INDENT);
    zStr_putn(zstr, rangeUpper(ev));
    zStr_putl(zstr, NULL);
    }

/* INFO VARIABLE output for an enum variable */
void  outputIV_enum(ZPJL z, ZPJLENV zpe, zStr* zstr, Symbol id,  Symbol* list, Uint32 listSize, bool readOnly)
    {
    Uint32  i;
    Symbol  currVal = NULL_SYMBOL;
    ZPJL_EnvItem*  ev = zpjlFindVar(zpe, id);

    zStr_puts(zstr, id);

    if (NULL != ev)
        {
        switch (ev->type)
            {
            case EIT_ENUM:
                currVal = (Symbol) ev->data.dword;
                break;

            case EIT_ENUM_RO:
                if (ev->data.proc != NULL)
                    currVal = (*(ev->data.proc))(z);
                break;
            }
        }
    if (NULL_SYMBOL == currVal)
        {
        zStr_putl(zstr, NULL);
        zStr_putc(zstr, '?');
        zStr_putl(zstr, NULL);
        return;
        }

    zStr_putc(zstr, '=');
    zStr_puts(zstr, currVal);
    zStr_putstr(zstr, " [");
    zStr_putn(zstr, listSize);
    zStr_putc(zstr, ' ');
    zStr_puts(zstr, IDS_ENUMERATED);

    if (readOnly)
        {
        zStr_putc(zstr, ' ');
        zStr_puts(zstr, IDS_READONLY);
        }

    zStr_putc(zstr, ']');
    zStr_putl(zstr, NULL);

    for (i = 0; i < listSize; i++)
        {
        zStr_putstr(zstr, INDENT);
        zStr_putl(zstr, SYM(list[i]));
        }
    }

char*  zpjlSym(Symbol s)
    {
    return SYM(s);
    }

/* end listing */
