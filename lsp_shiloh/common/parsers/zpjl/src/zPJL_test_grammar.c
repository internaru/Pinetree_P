/***********************************************************
* (c) Copyright 1998-2008 Marvell International, Ltd. 
*
*                 Marvell Confidential
* ==========================================================
*/

/****************************************************************************\
**                                                                          **
**  zPJL_test_grammar.c                                                     **
**                                                                          **
**  These tables specify the grammar for a given PJL implementation.        **
**  These tables depend on keywords being defined by zPJL_SYM.h using       **
**  the SYMGEN utility and a .SMG file.                                     **
**                                                                          **
**  To add any new keyword (command or environment or other), add it to     **
**  the .SMG file for this project and re-run the SYMGEN utility.           **
**                                                                          **
\****************************************************************************/

#include <stdbool.h>

#include "zPJL.h"
#include "zPJL_sym.h"
#include "zPJL_test.h"


/* grammar */
/*         *--------------------------------------------------------------- */

  WORD_MATCH  wm_OffOn[] =
    {
	{ IDS_OFF,          {A_VALUE,0}, S_EOL },
	{ IDS_ON,           {A_VALUE,0}, S_EOL },
    END_WORD_MATCH
    };

WORD_MATCH *pwm_OnOff=wm_OffOn;

WORD_MATCH *pwm_OffOn=wm_OffOn;

  WORD_MATCH  wm_OnOffAuto[] =
    {
	{ IDS_OFF,          {A_VALUE,0}, S_EOL },
    { IDS_ON,           {A_VALUE,0}, S_EOL },
	{ IDS_AUTO,         {A_VALUE,0}, S_EOL },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_OnOffAuto = wm_OnOffAuto;
WORD_MATCH *pwm_OffOnAuto = wm_OnOffAuto;



  WORD_MATCH  wm_OnOffVerbose[] =
    {
	{ IDS_OFF,          {A_VALUE,0}, S_EOL },
	{ IDS_ON,           {A_VALUE,0}, S_EOL },
	{ IDS_VERBOSE,      {A_VALUE,0}, S_EOL },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_OnOffVerbose = wm_OnOffVerbose;
WORD_MATCH *pwm_OffOnVerbose = wm_OnOffVerbose;
  WORD_MATCH  wm_Trays[] =
    {
	{ IDS_TRAY1,        {A_VALUE,0}, S_EOL },
	{ IDS_TRAY2,        {A_VALUE,0}, S_EOL },
	{ IDS_TRAY3,        {A_VALUE,0}, S_EOL },
	{ IDS_TRAY4,        {A_VALUE,0}, S_EOL },
	{ IDS_MPTRAY,       {A_VALUE,0}, S_EOL },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_Trays = wm_Trays;

WORD_MATCH  wm_MediaType[] =
{
	{ IDS_AUTOSELECT,    {A_VALUE,0}, S_EOL },
	{ IDS_PLAIN,         {A_VALUE,0}, S_EOL },
	{ IDS_THICK,         {A_VALUE,0}, S_EOL },
	{ IDS_VERYTHICK,     {A_VALUE,0}, S_EOL },
	{ IDS_THIN,          {A_VALUE,0}, S_EOL },
	{ IDS_TRANSPARENCY,  {A_VALUE,0}, S_EOL },
	{ IDS_PAPERLABEL,    {A_VALUE,0}, S_EOL },
	{ IDS_VINYLLABEL,    {A_VALUE,0}, S_EOL },
	{ IDS_ENVELOPE,      {A_VALUE,0}, S_EOL },
	{ IDS_CARDSTOCK,     {A_VALUE,0}, S_EOL },
	{ IDS_POSTCARD,      {A_VALUE,0}, S_EOL },
	{ IDS_RECYCLED,      {A_VALUE,0}, S_EOL },
	{ IDS_LETTERHEAD,    {A_VALUE,0}, S_EOL },
	END_WORD_MATCH
};
WORD_MATCH *pwm_MediaType=wm_MediaType;


WORD_MATCH *pwm_Paper = NULL;
/* these are for @PJL SET or @PJL DEFAULT
** -- if an environment variable is READ-ONLY, do not put it in this table!
** -- some variables are for exlusively for CE or DE; such logic must be
**    built in to the commandNotify routine
*/
WORD_MATCH *pwm_SetEnv = NULL;
/* these are for @PJL INQUIRE and @PJL DINQUIRE
** -- some variables are for exlusively for CE or DE; such logic must be
**    built in to the commandNotify routine
*/
WORD_MATCH *pwm_GetEnv = NULL;
  WORD_MATCH  wm_Enter[] =
    {
    END_WORD_MATCH
    };
WORD_MATCH *pwm_Enter = wm_Enter;

  WORD_MATCH wm_Lang[] =
{
    { IDS_LANGUAGE,     {A_KEY,0},S_COMMENT},
    END_WORD_MATCH
};
WORD_MATCH *pwm_Lang = wm_Lang;

  WORD_MATCH  wm_Info[] =
    {
	{ IDS_ID,           {A_KEY,0}, S_EOL },
	{ IDS_CONFIG,       {A_KEY,0}, S_EOL },
	{ IDS_MEMORY,       {A_KEY,0}, S_EOL },
	{ IDS_PAGECOUNT,    {A_KEY,0}, S_EOL },
	{ IDS_STATUS,       {A_KEY,0}, S_EOL },
	{ IDS_VARIABLES,    {A_KEY,0}, S_EOL },
	{ IDS_USTATUS,      {A_KEY,0}, S_EOL },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_Info = wm_Info;
  WORD_MATCH  wm_UStatus[] =
    {
	{ IDS_DEVICE,       {A_KEY,0}, S_START_WORDMATCH, &pwm_OnOffVerbose },
	{ IDS_JOB,          {A_KEY,0}, S_START_WORDMATCH, &pwm_OnOff },
	{ IDS_PAGE,         {A_KEY,0}, S_START_WORDMATCH, &pwm_OnOff },
	{ IDS_TIMED,        {A_KEY,0}, S_START_NUMBER },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_UStatus = wm_UStatus;
  WORD_MATCH  wm_Job[] =
    {
	{ IDS_NAME,         {A_KEY,0}, S_START_QUOTE, 0 },
	{ IDS_START,        {A_KEY,0}, S_START_NUMBER, 0 },
	{ IDS_END,          {A_KEY,0}, S_START_NUMBER, 0 },
	{ IDS_PASSWORD,     {A_KEY,0}, S_START_NUMBER, 0 },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_Job = wm_Job;
  WORD_MATCH  wm_Eoj[] =
    {
	{ IDS_NAME,         {A_KEY,0}, S_START_QUOTE },
    END_WORD_MATCH
    };
WORD_MATCH *pwm_Eoj = wm_Eoj;
/*{ KEY_SYMBOL, ACTION,USERCODE, NEXTSTATE, SUBGRAMMAR }*/
static WORD_MATCH  wm_Commands[] =
    {
	{ IDS_COMMENT,      {A_COMMAND,0}, S_COMMENT },
	{ IDS_DEFAULT,      {A_COMMAND,0}, S_START_WORDMATCH, &pwm_SetEnv },
	{ IDS_DINQUIRE,     {A_COMMAND,0}, S_START_WORDMATCH, &pwm_GetEnv },
	{ IDS_ECHO,         {A_COMMAND,0}, S_COMMENT },
	{ IDS_ENTER,        {A_COMMAND,0}, S_START_WORDMATCH, &pwm_Lang },
	{ IDS_EOJ,          {A_COMMAND,0}, S_START_WORDMATCH, &pwm_Eoj },
	{ IDS_INFO,         {A_COMMAND,0}, S_START_WORDMATCH, &pwm_Info },
	{ IDS_LJOBINFO,     {A_COMMAND,0}, S_START_WORDMATCH, &pwm_Info },
	{ IDS_SINDOHUSERNAME,{A_COMMAND,0}, S_START_WORDMATCH, &pwm_Info },
	{ IDS_INITIALIZE,   {A_COMMAND,0}, S_EOL },
	{ IDS_INQUIRE,      {A_COMMAND,0}, S_START_WORDMATCH, &pwm_GetEnv },
	{ IDS_JOB,          {A_COMMAND,0}, S_START_WORDMATCH, &pwm_Job },
	{ IDS_RESET,        {A_COMMAND,0}, S_EOL },
	{ IDS_SET,          {A_COMMAND,0}, S_START_WORDMATCH, &pwm_SetEnv },
	{ IDS_USTATUS,      {A_COMMAND,0}, S_START_WORDMATCH, &pwm_UStatus },
	{ IDS_USTATUSOFF,   {A_COMMAND,0}, S_EOL },
    END_WORD_MATCH
	};

WORD_MATCH *pwm_Commands = wm_Commands;

const WORD_MATCH  wm_PJL[] =
    {
    { IDS_PJL, {A_PJL,0}, S_START_WORDMATCH, (WORD_MATCH **)&pwm_Commands },
    END_WORD_MATCH
    };


/* end listing */
