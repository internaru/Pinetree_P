/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>
#include "lassert.h"
#include "nvram_api.h"
#include "fax_phbook.h"
#include "posix_ostools.h"

/*----------------------------------------------------------------------------*/
typedef struct
{
  char         name[FAX_PHBOOK_NAME_MAX_LEN+1];
  char         number[FAX_PHBOOK_NUMBER_MAX_LEN+1];
  unsigned int blacklist:1;
} _entry_t;

typedef struct
{
  char          name[FAX_PHBOOK_NAME_MAX_LEN+1];
  unsigned char members[(FAX_PHBOOK_NUM_ENTRIES+7)/8];
} _group_t;

typedef struct
{
  _entry_t entries[FAX_PHBOOK_NUM_ENTRIES];
  _group_t groups[FAX_PHBOOK_NUM_GROUPS];
} _data_t;

/*----------------------------------------------------------------------------*/
static nvram_handle_t*     _nvm_hdl;
static _data_t             _data;
static int                 _dirty;
static pthread_mutex_t    _mutex;
static const unsigned char _pow2[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/*----------------------------------------------------------------------------*/
static void _access(int lock_nLock)
{
  if (lock_nLock) pthread_mutex_lock( &_mutex );
  else            pthread_mutex_unlock( &_mutex );
}

#if HAVE_NVRAM
/*----------------------------------------------------------------------------*/
static error_type_t _init(nvram_init_type_t InitType, uint16_t Version,
                          void* InitLocation, uint32_t InitLocationSize, void *unused)
{
    switch( InitType )
    {
        case NVRAM_NEW:
        case NVRAM_ERROR:
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_VERSION_CHANGE: // for now, set defaults, production code should migrate versions
        {
            // set defaults
            memset( &_data, 0, sizeof( _data_t ) );
            ASSERT(sizeof(_data_t) <= InitLocationSize);
            memcpy( InitLocation, &_data, sizeof(_data_t));
            break;
        }
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        {
            // no action required
            break;
        }
        default:
        {
            ASSERT(NVRAM_OK == InitType);

            // read out current values
            memcpy(&_data, InitLocation, sizeof(_data_t));
            break;
        }
    }
  return OK;
}
#endif

/*----------------------------------------------------------------------------*/
void fax_phbook_commit(void)
{
  /* If data has been updated in any way we set the data and request a flush */
  _access(1);
  if (_dirty)
  {
    nvram_set_var(_nvm_hdl, &_data);
    nvram_flush();
    _dirty = 0;
  }
  _access(0);
}

/*----------------------------------------------------------------------------*/
void fax_phbook_init(void)
{
  posix_mutex_init(&_mutex);
  _access(1);
  nvram_variable_register(&_nvm_hdl, "PHBK", 2, sizeof(_data), _init, NULL);
  _dirty = 0;
  _access(0);
  {
    extern void fax_phbook_cmd_init(void);
                fax_phbook_cmd_init();
  }
}

/*----------------------------------------------------------------------------*/
/* This macro performs parameter checking and allows 'single line' style coding
 * instead of a 4 to 1 source code expansion */
#define _CHECK(cond, result) do                  \
                             {                   \
                               if ((cond))       \
                               {                 \
                                 return (result);\
                               }                 \
                             } while (0)

/*----------------------------------------------------------------------------*/
int fax_phbook_set_name(unsigned int phbook_id, const char* name)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES,    -1);
  _CHECK(!name,                                  -2);
  _CHECK(strlen(name) > FAX_PHBOOK_NAME_MAX_LEN, -3);
  _access(1);
  strcpy(_data.entries[phbook_id].name, name);
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_get_name(unsigned int phbook_id, char* name)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _CHECK(!name,                               -2);
  _access(1);
  strcpy(name, _data.entries[phbook_id].name);
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_set_number(unsigned int phbook_id, const char* number)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES,        -1);
  _CHECK(!number,                                    -2);
  _CHECK(strlen(number) > FAX_PHBOOK_NUMBER_MAX_LEN, -3);
  _access(1);
  strcpy(_data.entries[phbook_id].number, number);
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_get_number(unsigned int phbook_id, char* number)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _CHECK(!number,                             -2);
  _access(1);
  strcpy(number, _data.entries[phbook_id].number);
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_set_blacklist(unsigned int phbook_id, int blacklist)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _access(1);
  _data.entries[phbook_id].blacklist = (blacklist?1:0);
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_get_blacklist(unsigned int phbook_id, int *blacklist)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _CHECK(!blacklist,                          -2);
  /* No need to lock access here */
  *blacklist = _data.entries[phbook_id].blacklist;
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_set(unsigned int phbook_id,
                   const char* name, const char* number, int  blacklist)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES,        -1);
  _CHECK(!name,                                      -2);
  _CHECK(!number,                                    -2);
  _CHECK(strlen(name)   > FAX_PHBOOK_NAME_MAX_LEN,   -3);
  _CHECK(strlen(number) > FAX_PHBOOK_NUMBER_MAX_LEN, -3);
  _access(1);
  strcpy(_data.entries[phbook_id].name,        name);
  strcpy(_data.entries[phbook_id].number,      number);
         _data.entries[phbook_id].blacklist = (blacklist?1:0);
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_get(unsigned int phbook_id,
                   char* name, char* number, int* blacklist)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _CHECK(!name,                               -2);
  _CHECK(!number,                             -2);
  _access(1);
  strcpy(name,   _data.entries[phbook_id].name);
  strcpy(number, _data.entries[phbook_id].number);
        *blacklist = _data.entries[phbook_id].blacklist;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_set_name(unsigned int group_id, const char* name)
{
  _CHECK(group_id >= FAX_PHBOOK_NUM_GROUPS,      -1);
  _CHECK(!name,                                  -2);
  _CHECK(strlen(name) > FAX_PHBOOK_NAME_MAX_LEN, -3);
  _access(1);
  strcpy(_data.groups[group_id].name, name);
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_get_name(unsigned int group_id, char* name)
{
  _CHECK(group_id >= FAX_PHBOOK_NUM_GROUPS, -1);
  _CHECK(!name,                             -2);
  _access(1);
  strcpy(name, _data.groups[group_id].name);
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_add_entry(unsigned int group_id,
                               unsigned int phbook_id)
{
  _CHECK(group_id  >= FAX_PHBOOK_NUM_GROUPS,  -1);
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  unsigned char mask = _pow2[(phbook_id & 0x7)];
  unsigned int  idx  = (phbook_id >> 3);
  _access(1);
  int preset = _data.groups[group_id].members[idx] & mask;
  if (!preset)
  {
    _data.groups[group_id].members[idx] |= mask;
    _dirty = 1;
  }
  _access(0);
  return preset?FAX_PHBOOK_RESULT_ALREADY:0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_remove_entry(unsigned int group_id,
                                  unsigned int phbook_id)
{
  _CHECK(group_id  >= FAX_PHBOOK_NUM_GROUPS,  -1);
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  unsigned char mask = _pow2[(phbook_id & 0x7)];
  unsigned int  idx  = (phbook_id >> 3);
  _access(1);
  int preset = _data.groups[group_id].members[idx] & mask;
  if (preset)
  {
    _data.groups[group_id].members[idx] &= ~mask;
    _dirty = 1;
  }
  _access(0);
  return preset?0:FAX_PHBOOK_RESULT_ALREADY;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_erase(unsigned int group_id)
{
  _CHECK(group_id >= FAX_PHBOOK_NUM_GROUPS,  -1);
  _access(1);
  memset(_data.groups[group_id].members, 0, sizeof(_data.groups[0].members));
  _data.groups[group_id].name[0] = 0;
  _dirty = 1;
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
static void _advance_mask_and_idx(unsigned char* mask, unsigned int* idx)
{
  if (*mask == 0x80)
  {
    *mask = 0x1;
    *idx += 1;
  }
  else
  {
    *mask <<= 1;
  }
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_get_entry_count(unsigned int  group_id,
                                     unsigned int *count)
{
  _CHECK(group_id >= FAX_PHBOOK_NUM_GROUPS, -1);

  unsigned int id, idx;
  unsigned char mask, *members = _data.groups[group_id].members;

  *count = 0;
  _access(1);
  for (id=0, idx=0, mask=0x01; id<FAX_PHBOOK_NUM_ENTRIES; id++)
  {
    if (members[idx] & mask)
      (*count)++;
    _advance_mask_and_idx(&mask, &idx);
  }
  _access(0);
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_group_get_phbook_idx(unsigned int  group_id,
                                    unsigned int  which,
                                    unsigned int *phbook_id)
{
  _CHECK(group_id >= FAX_PHBOOK_NUM_GROUPS,  -1);
  _CHECK(which    >= FAX_PHBOOK_NUM_ENTRIES, -1);
  _CHECK(!phbook_id,                         -2);

  unsigned int id, idx;
  unsigned char mask, *members = _data.groups[group_id].members;

  _access(1);
  for (id=0, idx=0, mask=0x01; id<FAX_PHBOOK_NUM_ENTRIES; id++)
  {
    if (members[idx] & mask)
      if (!which--)
        break;
    _advance_mask_and_idx(&mask, &idx);
  }
  _access(0);

  _CHECK(which+1, -3); /* Did not find enough group members */
  *phbook_id = id;
  return 0;
}

/*----------------------------------------------------------------------------*/
int fax_phbook_erase(unsigned int phbook_id)
{
  _CHECK(phbook_id >= FAX_PHBOOK_NUM_ENTRIES, -1);
  unsigned int  group_id;
  unsigned char clrmask = ~(_pow2[(phbook_id & 0x7)]);
  unsigned int  idx  = (phbook_id >> 3);
  _access(1);
  _data.entries[phbook_id].name[0] = 0;
  _data.entries[phbook_id].number[0] = 0;
  _data.entries[phbook_id].blacklist = 0;
  for (group_id=0; group_id<FAX_PHBOOK_NUM_GROUPS; group_id++)
    _data.groups[group_id].members[idx] &= clrmask;
  _dirty = 1;
  _access(0);
  return 0;
}
