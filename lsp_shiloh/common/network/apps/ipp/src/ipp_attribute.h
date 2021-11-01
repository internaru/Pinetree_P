/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_ATTRIBUTE_H__
#define __INC_IPP_ATTRIBUTE_H__

#include "ipp_types.h"
#include "ipp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  ipp_u8bit_t  syntax;
  ipp_u16bit_t len;
  void*        data;
} ipp_attr_val_t;

typedef struct
{
  char*           name;    /* Null terminated */
  ipp_u32bit_t    num_val;
  ipp_attr_val_t* val;
} ipp_attr_t;

typedef struct
{
  ipp_u8bit_t  tag;
  ipp_u32bit_t num_attr;
  ipp_attr_t*  attr;
} ipp_attr_grp_t;

typedef struct
{
  ipp_u32bit_t    num_grp;
  ipp_attr_grp_t* grp;
} ipp_attr_groups_t;

typedef struct
{
   ipp_u32bit_t num_req_attr;
   char         *req_attr_str;

} ipp_requested_attrs_t;

ipp_attr_t* ipp_find_attr(ipp_attr_groups_t* groups, const char* attrname);

/* This reads the attributes from an IO access function. It grows buf and len
   as it reads (via ipp_realloc) and returns 0 upon success. Len and buf may
   be pre-allocated (and, in fact, this is often the case) */
int ipp_read_attributes(ipp_reader_t readfn, void* data,
                        ipp_u8bit_t* *buf, int* len);

/* This will parse all attribute groups from the data stream. Data and len
   will be updated as the parsing progresses. In the case of error NULL is
   returned. In all other cases the memory should eventually be free by a call
   to ipp_free_attribute_groups(). */
ipp_attr_groups_t* ipp_parse_attr_groups(void** data, int* len);

/* Binary encodes a set of attribute groups. If dst is too short (as indicated
   by len) to hold the data or, in the case of any other error, the return code
   will be <0. Upon success the return code indicates how many bytes of len
   were consumed. */
int ipp_encode_attr_groups(ipp_attr_groups_t* groups, void* dst, int len);

/* Use this to determine how long the encoded groups data will consume. This
   can be used, for example, to create a buffer to encode into. Any source of
   error (such as incorrect groups data) will generate a response code <0 */
int ipp_encoded_attr_groups_len(ipp_attr_groups_t* groups);

ipp_attr_groups_t* ipp_create_attr_groups(void);
void               ipp_free_attr_groups(ipp_attr_groups_t *groups,int all);
void               ipp_dump_attr_groups(ipp_attr_groups_t *groups);
int                ipp_add_attr_group(ipp_attr_groups_t *groups,
                                      ipp_attr_grp_t* group);

ipp_attr_grp_t* ipp_create_attr_group(ipp_u8bit_t tag);
void            ipp_free_attr_group(ipp_attr_grp_t *group, int all);
void            ipp_dump_attr_group(ipp_attr_grp_t *group);
int             ipp_add_attr(ipp_attr_grp_t* group, ipp_attr_t* attr);
int             ipp_add_attr_with_val(ipp_attr_grp_t* grp,
                                      const char* attrname,
                                      ipp_u8bit_t value_tag,
                                      void* data, ipp_u16bit_t datalen);
int             ipp_add_attr_with_enum(ipp_attr_grp_t* grp,
                                       const char* attrname,
                                       ipp_s32bit_t val);
int             ipp_add_attr_with_int(ipp_attr_grp_t* grp,
                                      const char* attrname,
                                      ipp_s32bit_t val);
int             ipp_add_attr_with_bool(ipp_attr_grp_t* grp,
                                       const char* attrname,
                                       ipp_u8bit_t val);
int             ipp_add_attr_with_name(ipp_attr_grp_t* grp,
                                       const char* attrname,
                                       const char* val);
               
/* len is required to be non-zero if name is not NULL terminated */
ipp_attr_t* ipp_create_attr(const char* name, int len);
void        ipp_free_attr(ipp_attr_t *attr, int all);
void        ipp_dump_attr(ipp_attr_t *attr);
int         ipp_add_attr_val(ipp_attr_t* attr, ipp_attr_val_t* val);
ipp_attr_t* ipp_create_attr_with_val(const char* attrname,
                                     ipp_u8bit_t value_tag,
                                     void* data, ipp_u16bit_t datalen);
ipp_attr_t* ipp_create_attr_with_enum(const char* attrname, ipp_s32bit_t val);
ipp_attr_t* ipp_create_attr_with_int(const char* attrname, ipp_s32bit_t val);
ipp_attr_t* ipp_create_attr_with_bool(const char* attrname, ipp_u8bit_t val);
ipp_attr_t* ipp_create_attr_with_name(const char* attrname, char* val);

ipp_attr_val_t* ipp_create_attr_val(ipp_u8bit_t syntax,
                                    void* data, ipp_u16bit_t data_length);
ipp_attr_val_t* ipp_create_attr_val_enum(ipp_s32bit_t val);
ipp_attr_val_t* ipp_create_attr_val_int(ipp_s32bit_t val);
ipp_attr_val_t* ipp_create_attr_val_bool(ipp_u8bit_t val);
ipp_attr_val_t* ipp_create_attr_val_name(char* val);
void            ipp_free_attr_val(ipp_attr_val_t *val, int all);
void            ipp_dump_attr_val(ipp_attr_val_t *val);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_ATTRIBUTE_H__ */
