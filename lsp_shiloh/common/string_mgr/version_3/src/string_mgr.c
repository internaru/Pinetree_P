/*
 * ============================================================================
 * (C) Copyright 2008-2009   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "tx_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "strfmt.h"
#include "string_mgr_api.h"
#include "string_mgr_config_api.h"
#include "string_mgr_config.h"
#include "string_mgr_nvm.h"

#define _STRINGS_FILE_PATH "/data/strings/strings.%s"

static bool _initdone = false;

typedef struct _code_node_st
{
  string_mgr_code_t code;
  char* translation;
  struct _code_node_st* next;
} _code_t;

typedef struct _lang_node_st
{
  string_mgr_language_t lang;
  _code_t* codes;
  unsigned refcount;
  struct _lang_node_st* next;
} _lang_t;

static _lang_t *_languages;    /* A list of open languages */
static _lang_t *_language_set; /* The set language */
static _lang_t *_language_bkp; /* The backup language */
static _lang_t *_language_eng; /* The english language */

/*----------------------------------------------------------------------------*/
static TX_MUTEX _m;
static void _mutex(int get_nRelease)
{
  ASSERT(_initdone == true);
  if (get_nRelease) tx_mutex_get(&_m, TX_WAIT_FOREVER);
  else              tx_mutex_put(&_m);
}

/*----------------------------------------------------------------------------*/
static const string_mgr_code_entry_t* _get_entry_by_str(const char* str,
                                       const string_mgr_code_tbl_t* tbl)
{
  unsigned i;

  if (!str || !*str) return (void*)0;

  for (i=0;i<tbl->size;i++)
    if (!strcmp(str, tbl->tbl[i].identifier))
      return &tbl->tbl[i];
  return (void*)0;
}

/*----------------------------------------------------------------------------*/
static const string_mgr_code_entry_t
             *_get_entry_by_lang(string_mgr_language_t lang)
{
  unsigned i;
  for (i=0;i<string_mgr_config_language_codes->size;i++)
    if (lang == string_mgr_config_language_codes->tbl[i].lang_code)
      return &string_mgr_config_language_codes->tbl[i];
  return (void*)0;
}

/*----------------------------------------------------------------------------*/
static const string_mgr_code_entry_t* _get_entry_by_code(string_mgr_code_t code)
{
  unsigned i;
  for (i=0;i<string_mgr_config_string_codes->size;i++)
    if (code == string_mgr_config_string_codes->tbl[i].str_code)
      return &string_mgr_config_string_codes->tbl[i];
  return (void*)0;
}

/*----------------------------------------------------------------------------*/
static const char* _get_str(_lang_t* lang, string_mgr_code_t code)
{
  const string_mgr_code_entry_t* entry;
  _code_t* c;
  c = lang->codes;
  while (c && (c->code <= code))
  {
    if (c->code == code)
      return c->translation;
    c = c->next;
  }

  /* Provide the OSFA string from the code table */
  entry = _get_entry_by_code(code);
  if (entry) return entry->osfa_str;

  /* All attempts have failed */
  return NULL;
}

/*----------------------------------------------------------------------------*/
_code_t* _insert_code(_code_t* list, _code_t* node)
{
  /* Insert a code into a list of codes in increasing numerical order */
  _code_t *p = NULL, *l = list;

  if (!node) return list;

  while (l && (l->code < node->code)) l=(p=l)->next;
  node->next = l;
  if (p) p->next = node;
  else   return node;
  return list;
}

/*----------------------------------------------------------------------------*/
char* _strdup(char* orig)
{
  char* n;
  int l = strlen(orig);
  if ((n = MEM_MALLOC(l+1)))
    return strcpy(n, orig);
  return NULL;
}

/*----------------------------------------------------------------------------*/
static void _populate_language(char* filepath, _lang_t* lang)
{
  uint32_t filepos = 0;
  unsigned done=0, linelen, linebuflen=1024;
  char *linebuf, *translation;
  FILE* file;
  string_mgr_code_t code;
  _code_t* c;
 
  if (!(file = fopen(filepath, "r"))) return;

  linebuf = MEM_MALLOC(linebuflen);
  if (!linebuf) return;
  while (1)
  {
    filepos = ftell(file);
    while (1)
    {
      if (!fgets(linebuf, linebuflen, file)) { done = 1; break; }
      linelen = strlen(linebuf);
      if (linebuf[linelen-1] == '\n') break;
      if (linebuf[linelen-1] == '\r') break;
      fseek(file, filepos, SEEK_SET);
      MEM_FREE_AND_NULL(linebuf);
      linebuflen += 1024;
      if (!(linebuf = MEM_MALLOC(linebuflen))) { fclose(file); return; }
    }
    if (done) break;
    linebuf[linelen-1] = '\0';
    if (!(translation = strchr(linebuf, ','))) continue;
    *translation++ = 0;
    if (OK == string_mgr_get_code(linebuf, &code))
    {
      c = MEM_MALLOC(sizeof(*c));
      if (c)
      {
        memset(c, 0, sizeof(*c));
        c->code = code;
        c->translation = _strdup(translation);
        lang->codes = _insert_code(lang->codes, c);
      }
    }
  }
  MEM_FREE_AND_NULL(linebuf);
  fclose(file);
}

/*----------------------------------------------------------------------------*/
static void* _open_lang(string_mgr_language_t lang)
{
  _lang_t* l;
  static char _path[100]; /* Can be static because only used within mutex */
  const string_mgr_code_entry_t* entry;

  _mutex(1);

  /* Search to see if language is already open.
     If it is then update the refcount. */
  l = _languages;
  while (l)
  {
    if (l->lang == lang)
    {
      l->refcount += 1;
      _mutex(0);
      return l;
    }
    l = l->next;
  }

  /* Make sure that this language exists in the OEM provided config table */
  if (!(entry = _get_entry_by_lang(lang)))
  {
    _mutex(0);
    return NULL;
  }
  strfmt(_path, _STRINGS_FILE_PATH, entry->identifier /* Language ID */);

  /* Allocate a structure to hold the language */
  if (!(l = MEM_MALLOC(sizeof(*l))))
  {
    _mutex(0);
    return NULL;
  }
  memset(l, 0, sizeof(*l));
  l->lang = lang;

  _populate_language(_path, l);

  /* Link the language into the list, bump up the refcount (from 0 to 1) */
  l->next = _languages;
  _languages = l;
  l->refcount += 1;

  _mutex(0);
  return l;
}

/*----------------------------------------------------------------------------*/
static void _close_lang(void* lang_hdl)
{
  _lang_t *l, *p;
  _code_t *c;

  _mutex(1);

  /* Search to make sure this language is in our list */
  p = NULL;
  l = _languages;
  while (l)
  {
    if (l == lang_hdl)
      break;
    l = (p=l)->next;
  }

  /* If language is not in our list then it is an invalid ptr and we exit */
  if (!l)
  {
    _mutex(0);
    return;
  }

  /* If refcount is not zero (after decrementing) then we are done */
  if ((--l->refcount))
  {
    _mutex(0);
    return;
  }

  /* Unlink the language from the _languages list and unlock that list */
  if (p) p->next    = l->next;
  else   _languages = l->next;
  _mutex(0);

  /* Free everything that was allocated when the language was opened */
  while (l->codes)
  {
    c = l->codes;
    l->codes = c->next;
    MEM_FREE_AND_NULL(c->translation);
    MEM_FREE_AND_NULL(c);
  }
  MEM_FREE_AND_NULL(l);
}

/*----------------------------------------------------------------------------*/
const char * string_mgr_get_string( string_mgr_code_t code,
                                    string_mgr_localization_type_t type )
{
  const char* str;

  _mutex(1);

  if      (type == string_mgr_loc_full)    str = _get_str(_language_set, code);
  else if (type == string_mgr_loc_english) str = _get_str(_language_eng, code);
  else                                     str = NULL;

  _mutex(0);
 
  return str;
}

/*----------------------------------------------------------------------------*/
const char * string_mgr_get_str( string_mgr_code_t code, const char* lang_id )
{
  const char* str = NULL;
  string_mgr_language_t lang;

  const string_mgr_code_entry_t* entry;
  if (!(entry = _get_entry_by_str(lang_id, string_mgr_config_language_codes)))
    lang = _language_set->lang;
  else
    lang = entry->lang_code;

  _mutex(1);

  /* See if any of our currently cached languages match the requested lang */
  if      (_language_eng->lang == lang)
    str = _get_str(_language_eng, code);
  else if (_language_set->lang == lang)
    str = _get_str(_language_set, code);
  else if (_language_bkp && (_language_bkp->lang == lang))
    str = _get_str(_language_bkp, code);

  /* Replace (or initialize) the backup language */
  else
  {
    _lang_t* l = _open_lang(lang);
    if (l)
    {
      if (_language_bkp) _close_lang(_language_bkp);
      str = _get_str(_language_bkp = l, code);
    }
  }

  _mutex(0);

  /* Return the string if it could be found */
  return str;
}

/*----------------------------------------------------------------------------*/
error_type_t string_mgr_set_language( string_mgr_language_t language )
{
  _lang_t* l;

  _mutex(1);
  if (language == _language_set->lang)
  {
    _mutex(0);
    return OK;
  }
  if (!(l = _open_lang(language)))
  {
    _mutex(0);
    return FAIL; /* Cannot open requested language */
  }
  _close_lang(_language_set);
  string_mgr_nvm_set_lang((_language_set = l)->lang);
  _mutex(0);
  return OK;
}

/*----------------------------------------------------------------------------*/
error_type_t string_mgr_get_language( string_mgr_language_t * language )
{
  _mutex(1);
  *language = _language_set->lang;
  _mutex(0);
  return OK;
}

/*----------------------------------------------------------------------------*/
void string_mgr_init( void )
{
  tx_mutex_create(&_m, "string_mgr", TX_INHERIT);
  _initdone     = true;
  _mutex(1);
  string_mgr_nvm_init();
  _language_eng = _open_lang(LANG_ENGLISH);
  ASSERT(_language_eng);
  _language_set = _open_lang(string_mgr_nvm_get_lang());
  if (!_language_set)
  {
    string_mgr_nvm_set_lang(LANG_ENGLISH);
    _language_set = _open_lang(string_mgr_nvm_get_lang());
  }
  ASSERT(_language_set);
  _language_bkp = NULL;
  _mutex(0);
}

/*----------------------------------------------------------------------------*/
error_type_t string_mgr_get_code(const char* key, string_mgr_code_t* code)
{
  const string_mgr_code_entry_t* entry;
  if (!(entry = _get_entry_by_str(key, string_mgr_config_string_codes)))
    return FAIL;
  *code = entry->str_code;
  return OK;
}

/*----------------------------------------------------------------------------*/
bool string_mgr_is_right_to_left_lang( string_mgr_language_t language )
{
  const string_mgr_code_entry_t* entry = _get_entry_by_lang(language);
  if (!entry) return false; /* Assume left to right */
  return entry->right_to_left?true:false;
}
