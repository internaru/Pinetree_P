/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>

#include "memAPI.h"
#include "ipp.h"
#include "ipp_const.h"
#include "ipp_attribute.h"
#include "ipp_dbg.h"

/* #define _fndbg ipp_dbg */
#define _dbg0  ipp_dbg
/* #define _dbg1  ipp_dbg */
/* #define _dbg2  ipp_dbg */

#define _fndbg(fmt, ...)
/* #define _dbg0(fmt, ...) */
#define _dbg1(fmt, ...)
#define _dbg2(fmt, ...)

/*----------------------------------------------------------------------------*/
static int _read_data(ipp_reader_t readfn, void* data,
                      ipp_u8bit_t* *buf, int* len, int bytes)
{
  ipp_u8bit_t* d;
  if (!(d = IPP_REALLOC(*buf, *len+bytes))) 
  {
     ASSERT(0);
     return -1;
  }
  *buf = d;
  while (bytes)
  {
    int r = readfn(data, d+*len, bytes);
    if (!r) return -2; /* End of data stream */
    if (r<0) return -1; /* Read error */
    *len += r;
    bytes -= r;
  }
  _dbg2("X. BUF = %p, Len = %d", *buf, *len);
  return 0;
}


/*----------------------------------------------------------------------------*/
int ipp_read_attributes(ipp_reader_t readfn, void* data,
                        ipp_u8bit_t* *buf, int* len)
{
  int l;

  if (!readfn) return -1;

  while (1)
  {
    if (_read_data(readfn, data, buf, len, 1)) return -2;
    l = (*buf)[*len-1];
    _dbg1("TAG = %d", l);
    if (l == IPP_TAG_END_OF_ATTRIBUTES) return 0;
    if (l <= IPP_TAG_UNSUPPORTED) continue; /* Delimiter */

    /* Read the name length */
    if (_read_data(readfn, data, buf, len, 2)) return -2;
    l = (*buf)[*len-2]; l <<= 8; l += (*buf)[*len-1];
    _dbg1("LENGTH = %d", l);

    /* Read the name */
    if (l && _read_data(readfn, data, buf, len, l)) return -2;

    /* Read the value length */
    if (_read_data(readfn, data, buf, len, 2)) return -2;
    l = (*buf)[*len-2]; l <<= 8; l += (*buf)[*len-1];
    _dbg1("LENGTH = %d", l);

    /* Read the value */
    if (l && _read_data(readfn, data, buf, len, l)) return -2;
  }
  return 0;
}

/*----------------------------------------------------------------------------*/
static unsigned int _get_unum(void** data, int* len, int bytes)
{
  unsigned char* d = (unsigned char*)(*data);
  if (*len < bytes) return (unsigned int)-1;
  unsigned int x = 0;
  *len -= bytes;
  *data = d + bytes;
  while (bytes--)
  {
    x *= 256;
    x += *d++;
  }
  return x;
}

/*----------------------------------------------------------------------------*/
static void _unget(void** data, int* len, int bytes)
{
  unsigned char* d = (unsigned char*)(*data);
  *len += bytes;
  *data = d - bytes;
}

/*----------------------------------------------------------------------------*/
static ipp_attr_t* _parse_attr(void** data, int* len, int single)
{
  ipp_attr_t     *attribute, *attr;
  ipp_attr_val_t *val;

  ipp_u8bit_t  tag;
  ipp_u16bit_t namelen, vallen;
  void        *tdata;

  _fndbg("ENTER(%d %d)", *len, single);

  if (*len < 5) return NULL;
  tag = _get_unum(data, len, 1);
  if (tag <= IPP_TAG_UNSUPPORTED)
  {
    _dbg1("Delimiter tag read - rewinding 1");
    _unget(data, len, 1);
    return NULL; /* Delimiter tag - we'll be reinvoked later */
  }
  namelen = _get_unum(data, len, 2);
  if (namelen && !single)
  {
    _dbg1("New attribute encountered - rewinding 3");
    _unget(data, len, 3);
    return NULL; /* This is new attribute - we'll be reinvoked with single=1 */
  }
  if ((namelen + 2) > *len)
  {
    _dbg0("ERROR: rewinding 3");
    _unget(data, len, 3);
    return NULL; /* This is a protocol error */
  }
  tdata = *data; /* Save this for now */
  *data += namelen;
  *len -= namelen;
  vallen = _get_unum(data, len, 2);
  if (vallen > *len)
  {
    _dbg0("ERROR: rewinding %d", 5+namelen);
    _unget(data, len, 5+namelen);
    return NULL; /* This is a protocol error */
  }
  attribute = ipp_create_attr(tdata, namelen);
  if (!attribute)
  {
    _dbg0("ERROR: failed to create attribute. rewinding %d", 5+namelen);
    _unget(data, len, 5+namelen);
    return NULL;
  }
  val = ipp_create_attr_val(tag, *data, vallen);
  if (!val)
  {
    _dbg0("ERROR: failed to create val. rewinding %d", 5+namelen);
    _unget(data, len, 5+namelen);
    ipp_free_attr(attribute, 1);
    return NULL;
  }
  if (ipp_add_attr_val(attribute, val))
  {
    _dbg0("ERROR: failed to add val. rewinding %d", 5+namelen);
    _unget(data, len, 5+namelen);
    ipp_free_attr_val(val, 1);
    ipp_free_attr(attribute, 1);
    return NULL;
  }
  ipp_free_attr_val(val, 1);
  *data += vallen;
  *len -= vallen;
  if (single)
  {
    _dbg1("  Looking for additional values");
    while ((attr = _parse_attr(data, len, 0)))
    {
      _dbg1("  Got an additional value - adding");
      if (ipp_add_attr_val(attribute, attr->val))
      {
        _dbg0("ERROR: failed to add val. cannot rewind");
        ipp_free_attr(attr, 1);
        ipp_free_attr(attribute, 1);
        return NULL;
      }
      ipp_free_attr(attr, 1);
    }
    _dbg1("  Additional value search is complete");
  }
  _fndbg("EXIT(%d %d %p)", *len, single, attribute);
  return attribute;
}

/*----------------------------------------------------------------------------*/
static ipp_attr_grp_t* _parse_grp(void** data, int* len)
{
  ipp_attr_grp_t *grp;
  ipp_attr_t     *attr;
  ipp_u8bit_t tag;

  _fndbg("ENTER");

  if (*len < 1) return NULL;
  if ((tag = _get_unum(data, len, 1)) == IPP_TAG_END_OF_ATTRIBUTES) return NULL;
  if (!(grp = ipp_create_attr_group(tag))) return NULL;
  while ((attr = _parse_attr(data, len, 1)))
  {
    if (ipp_add_attr(grp, attr))
    {
      ipp_free_attr_group(grp, 1);
      return NULL;
    }
    ipp_free_attr(attr, 1);
  }
  _fndbg("EXIT(%p)", grp);
  return grp;
}

/*----------------------------------------------------------------------------*/
ipp_attr_groups_t* ipp_parse_attr_groups(void** data, int* len)
{
  _fndbg("ENTER");

  ipp_attr_groups_t *grps = ipp_create_attr_groups();
  ipp_attr_grp_t    *grp;
  if (!grps) return NULL;
  while ((grp = _parse_grp(data, len)))
  {
    if (ipp_add_attr_group(grps, grp))
    {
      ipp_free_attr_groups(grps, 1);
      return NULL;
    }
    ipp_free_attr_group(grp, 1);
  }
  return grps;
}

/*----------------------------------------------------------------------------*/
static void _emit_bytes(unsigned char** dst, int* size, int* used,
                        unsigned char*  src, int num)
{
  if (!dst || !*dst)
    *used += num;
  else
  {
    while (num && (*size > 0))
    {
      *((*dst)++) = *src++;
      *size -= 1;
      *used += 1;
      num -= 1;
    }
    if (num) *size = -1;
  }
}

/*----------------------------------------------------------------------------*/
static void _emit_short(unsigned char** dst, int* size, int* used,
                        ipp_u16bit_t v)
{
  ipp_u8bit_t b[2] = { v/256, v%256 };
  _emit_bytes(dst, size, used, b, 2);
}

/*----------------------------------------------------------------------------*/
int ipp_encode_attr_groups(ipp_attr_groups_t* groups, void* dst, int len)
{
  int g, a, v, c;
  ipp_u8bit_t eoa = IPP_TAG_END_OF_ATTRIBUTES;
  unsigned char* d = (unsigned char*)dst;
  if (!groups) return -1;
  c = 0;
  for (g=0;g<groups->num_grp;g++)
  {
    ipp_attr_grp_t* grp = groups->grp + g;
    _emit_bytes(&d, &len, &c, &grp->tag, 1);
    for (a=0;a<grp->num_attr;a++)
    {
      ipp_attr_t* attr = grp->attr + a;
      ipp_u16bit_t namelen = strlen(attr->name);
      for (v=0;v<attr->num_val;v++)
      {
        ipp_attr_val_t* val = attr->val + v;
        _emit_bytes(&d, &len, &c, &val->syntax, 1);
        _emit_short(&d, &len, &c, namelen);
        _emit_bytes(&d, &len, &c, (unsigned char*)(attr->name), namelen);
        namelen = 0;
        _emit_short(&d, &len, &c, val->len);
        _emit_bytes(&d, &len, &c, val->data, val->len);
      }
    }
  }
  _emit_bytes(&d, &len, &c, &eoa, 1);
  if (len < 0) return -2;
  return c;
}

/*----------------------------------------------------------------------------*/
int ipp_encoded_attr_groups_len(ipp_attr_groups_t* groups)
{
  return ipp_encode_attr_groups(groups, NULL, 0);
}

/*----------------------------------------------------------------------------*/
ipp_attr_groups_t* ipp_create_attr_groups(void)
{
  _fndbg("ENTER");
  ipp_attr_groups_t* grps = IPP_MALLOC(sizeof(ipp_attr_groups_t));
  ASSERT(grps);
  if (grps) memset(grps, 0, sizeof(*grps));
  _fndbg("EXIT(%p)", grps);
  return grps;
}

/*----------------------------------------------------------------------------*/
void ipp_free_attr_groups(ipp_attr_groups_t *grps, int all)
{
  _fndbg("ENTER(%p)", grps);
  if (grps)
  {
    int g;
    
    for (g=0;g<grps->num_grp;g++)
        ipp_free_attr_group(grps->grp + g, 0);
    if (grps->grp) IPP_FREE(grps->grp);
    if (all) IPP_FREE(grps);
  }
  _fndbg("EXIT(%p)", grps);
}

/*----------------------------------------------------------------------------*/
void ipp_dump_attr_groups(ipp_attr_groups_t *grps)
{
  if (grps)
  {
    int g;
    _dbg0("GROUPS: ");
    for (g=0;g<grps->num_grp;g++)
      ipp_dump_attr_group(grps->grp + g);
  }
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_group(ipp_attr_groups_t *grps,
                       ipp_attr_grp_t* grp)
{
  _fndbg("ENTER");
  int a;
  ipp_attr_grp_t *g;
  if (!grps) return -1; /* Parameter error */
  if (!grp)  return -1; /* Parameter error */
  g = IPP_REALLOC(grps->grp, sizeof(*g) * (grps->num_grp+1));
  if (!g)
  {
     ASSERT(0);
     return -2; /* Memory allocation error */
  }
  grps->grp = g;
  g = ipp_create_attr_group(grp->tag);
  if (!g)    return -2; /* Memory allocation error */
  for (a=0;a<grp->num_attr;a++)
  {
    if (ipp_add_attr(g, grp->attr + a))
    {
      ipp_free_attr_group(g, 1);
      return -3;
    }
  }
  memcpy(grps->grp + grps->num_grp, g, sizeof(*g));
  grps->num_grp += 1;
  IPP_FREE(g);
  _fndbg("EXIT(%d)", grps->num_grp);
  return 0;
}

/*----------------------------------------------------------------------------*/
ipp_attr_grp_t* ipp_create_attr_group(ipp_u8bit_t tag)
{
  _fndbg("ENTER");
  ipp_attr_grp_t* grp = IPP_MALLOC(sizeof(*grp));
  if (!grp) 
  {
     ASSERT(0);
     return NULL;
  }
  memset(grp, 0, sizeof(*grp));
  grp->tag = tag;
  _fndbg("EXIT(%p)", grp);
  return grp;
}

/*----------------------------------------------------------------------------*/
void ipp_free_attr_group(ipp_attr_grp_t *grp, int all)
{
  _fndbg("ENTER(%p)", grp);
  if (grp)
  {
    int a;
    
    for (a=0;a<grp->num_attr;a++)
      ipp_free_attr(grp->attr + a, 0);
    if (grp->attr) IPP_FREE(grp->attr);
    if (all) IPP_FREE(grp);
  }
  _fndbg("EXIT(%p)", grp);
}

/*----------------------------------------------------------------------------*/
void ipp_dump_attr_group(ipp_attr_grp_t *grp)
{
  if (grp)
  {
    int a;
    _dbg0("  GROUP: %s", ipp_const_tag_str(grp->tag));
    for (a=0;a<grp->num_attr;a++)
      ipp_dump_attr(grp->attr + a);
  }
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr(ipp_attr_grp_t* grp, ipp_attr_t* attr)
{
  _fndbg("ENTER(%p, %p)", grp, attr);
  int v;
  ipp_attr_t *a;
  if (!grp)  return -1; /* Parameter error */
  if (!attr) return -1; /* Parameter error */
  a = IPP_REALLOC(grp->attr, sizeof(*a) * (grp->num_attr+1));
  if (!a)
  {
     ASSERT(0);
     return -2; /* Memory allocation error */
  }
  grp->attr = a;
  a = ipp_create_attr(attr->name, 0);
  if (!a)    return -2; /* Memory allocation error */
  for (v=0;v<attr->num_val;v++)
  {
    if (ipp_add_attr_val(a, attr->val + v))
    {
      ipp_free_attr(a, 1);
      return -3;
    }
  }
  memcpy(grp->attr + grp->num_attr, a, sizeof(*a));
  grp->num_attr += 1;
  IPP_FREE(a);
  _fndbg("EXIT(%d)", grp->num_attr);
  return 0;
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_with_val(ipp_attr_grp_t* grp,
                          const char* attrname,
                          ipp_u8bit_t value_tag,
                          void* data, ipp_u16bit_t datalen)
{
  int r;
  if (!grp) return -1;
  ipp_attr_t* attr = ipp_create_attr_with_val(attrname, value_tag,
                                              data, datalen);
  if (!attr) return -2;
  r = ipp_add_attr(grp, attr);
  ipp_free_attr(attr, 1);
  return r?-3:0;
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_with_enum(ipp_attr_grp_t* grp,
                           const char* attrname,
                           ipp_s32bit_t val)
{
  int r;
  if (!grp) return -1;
  ipp_attr_t* attr = ipp_create_attr_with_enum(attrname, val);
  if (!attr) return -2;
  r = ipp_add_attr(grp, attr);
  ipp_free_attr(attr, 1);
  return r?-3:0;
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_with_int(ipp_attr_grp_t* grp,
                          const char* attrname,
                          ipp_s32bit_t val)
{
  int r;
  if (!grp) return -1;
  ipp_attr_t* attr = ipp_create_attr_with_int(attrname, val);
  if (!attr) return -2;
  r = ipp_add_attr(grp, attr);
  ipp_free_attr(attr, 1);
  return r?-3:0;
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_with_bool(ipp_attr_grp_t* grp,
                           const char* attrname,
                           ipp_u8bit_t val)
{
  return ipp_add_attr_with_val(grp, attrname, IPP_TAG_BOOLEAN, &val, 1);
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_with_name(ipp_attr_grp_t* grp,
                           const char* attrname, const char* val)
{
  return ipp_add_attr_with_val(grp, attrname, IPP_TAG_NAME_WITHOUT_LANGUAGE,
                               (void*)val, strlen(val));
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr(const char* name, int optional_len)
{
  _fndbg("ENTER(%p, %d)", name, optional_len);
  int len;
  if (!name) return NULL;
  ipp_attr_t* attr = IPP_MALLOC(sizeof(*attr));
  if (!attr) 
  {
     ASSERT(0);
     return NULL;
  }
  memset(attr, 0, sizeof(*attr));
  if (optional_len) len = optional_len;
  else              len = strlen(name);
  attr->name = IPP_MALLOC(len+1);
  if (!attr->name)
  {
    ASSERT(0);
    IPP_FREE(attr);
    return NULL;
  }
  if (len) memcpy(attr->name, name, len);
  attr->name[len] = 0;
  _fndbg("Exit(%p %s)", attr, attr->name);
  return attr;
}

/*----------------------------------------------------------------------------*/
void ipp_free_attr(ipp_attr_t *attr, int all)
{
  _fndbg("ENTER(%p)", attr);
  if (attr)
  {
    int v;
    
    for (v=0;v<attr->num_val;v++)
      ipp_free_attr_val(attr->val + v, 0);
    if (attr->val) IPP_FREE(attr->val);
    if (attr->name) IPP_FREE(attr->name);
    if (all) IPP_FREE(attr);
  }
  _fndbg("EXIT(%p)", attr);
}

/*----------------------------------------------------------------------------*/
void ipp_dump_attr(ipp_attr_t *attr)
{
  if (attr)
  {
    int v;
    _dbg0("    ATTRIBUTE: %s", attr->name);
    for (v=0;v<attr->num_val;v++)
      ipp_dump_attr_val(attr->val + v);
  }
}

/*----------------------------------------------------------------------------*/
int ipp_add_attr_val(ipp_attr_t* attr, ipp_attr_val_t* val)
{
  _fndbg("ENTER(%p %p)", attr, val);
  ipp_attr_val_t *v;
  if (!attr) return -1; /* Parameter error */
  if (!val)  return -1; /* Parameter error */
  v = IPP_REALLOC(attr->val, sizeof(*v) * (attr->num_val+1));
  if (!v)
  {
     ASSERT(0);
     return -2; /* Memory allocation error */
  }
  attr->val = v;
  v = ipp_create_attr_val(val->syntax, val->data, val->len);
  if (!v)    return -2; /* Memory allocation error */
  memcpy(attr->val + attr->num_val, v, sizeof(*v));
  attr->num_val += 1;
  IPP_FREE(v);
  _fndbg("EXIT(%d)", attr->num_val);
  return 0;
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr_with_val(const char* attrname,
                                     ipp_u8bit_t value_tag,
                                     void* data, ipp_u16bit_t datalen)
{
  ipp_attr_t     *attr;
  ipp_attr_val_t *val;
  if (!attrname) return NULL;
  if (datalen && !data) return NULL;
// rdj 2014-1-10: 0-length values are legal, e.g. syntax type 'text' is sequence of *zero* or 
//    more characters (rfc2911 4.1.1) 
//  if (!datalen)  return NULL;
  attr = ipp_create_attr(attrname, 0);
  if (!attr) return NULL;
  val = ipp_create_attr_val(value_tag, data, datalen);
  if (!val)
  {
    ipp_free_attr(attr, 1);
    return NULL;
  }
  if (ipp_add_attr_val(attr, val))
  {
    ipp_free_attr(attr, 1);
    ipp_free_attr_val(val, 1);
    return NULL;
  }
  ipp_free_attr_val(val, 1);
  return attr;
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr_with_enum(const char* attrname, ipp_s32bit_t eval)
{
  ipp_attr_t     *attr;
  ipp_attr_val_t *val;
  if (!attrname) return NULL;
  attr = ipp_create_attr(attrname, 0);
  if (!attr) return NULL;
  val = ipp_create_attr_val_enum(eval);
  if (!val)
  {
    ipp_free_attr(attr, 1);
    return NULL;
  }
  if (ipp_add_attr_val(attr, val))
  {
    ipp_free_attr(attr, 1);
    ipp_free_attr_val(val, 1);
    return NULL;
  }
  ipp_free_attr_val(val, 1);
  return attr;
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr_with_int(const char* attrname, ipp_s32bit_t ival)
{
  ipp_attr_t     *attr;
  ipp_attr_val_t *val;
  if (!attrname) return NULL;
  attr = ipp_create_attr(attrname, 0);
  if (!attr) return NULL;
  val = ipp_create_attr_val_int(ival);
  if (!val)
  {
    ipp_free_attr(attr, 1);
    return NULL;
  }
  if (ipp_add_attr_val(attr, val))
  {
    ipp_free_attr(attr, 1);
    ipp_free_attr_val(val, 1);
    return NULL;
  }
  ipp_free_attr_val(val, 1);
  return attr;
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr_with_bool(const char* attrname, ipp_u8bit_t val)
{
  return ipp_create_attr_with_val(attrname, IPP_TAG_BOOLEAN, &val, 1);
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_create_attr_with_name(const char* attrname, char* val)
{
  return ipp_create_attr_with_val(attrname, IPP_TAG_NAME_WITHOUT_LANGUAGE,
                                  val, strlen(val));
}

/*----------------------------------------------------------------------------*/
ipp_attr_val_t* ipp_create_attr_val(ipp_u8bit_t syntax,
                                    void* data, ipp_u16bit_t data_length)
{
  _fndbg("ENTER");
  ipp_attr_val_t* val = IPP_MALLOC(sizeof(*val));
  if (!val) 
  {
     ASSERT(0);
     return NULL;
  }
  if (!data_length)
    val->data = NULL;
  else
  {
    val->data = IPP_MALLOC(data_length);
    if (!val->data)
    {
      ASSERT(0);
      IPP_FREE(val);
      return NULL;
    }
    memcpy(val->data, data, data_length);
  }
  val->syntax = syntax;
  val->len = data_length;
  _fndbg("EXIT(%p)", val);
  return val;
}

/*----------------------------------------------------------------------------*/
static void* _create_signed_int(int val)
{
  union
  {
      signed int sval;
    unsigned int uval;
  } v;

  unsigned char* data = (unsigned char*)IPP_MALLOC(4);
  if (!data) 
  {
     ASSERT(0);
     return NULL;
  }
  v.sval = val;
  data[3] = v.uval&0xff; v.uval >>= 8;
  data[2] = v.uval&0xff; v.uval >>= 8;
  data[1] = v.uval&0xff; v.uval >>= 8;
  data[0] = v.uval&0xff;
  return data;
}

/*----------------------------------------------------------------------------*/
ipp_attr_val_t* ipp_create_attr_val_enum(ipp_s32bit_t eval)
{
  ipp_attr_val_t* val;
  void* data = _create_signed_int(eval);
  if (!data) return NULL;
  val = ipp_create_attr_val(IPP_TAG_ENUM, data, 4);
  IPP_FREE(data);
  return val;
}

/*----------------------------------------------------------------------------*/
ipp_attr_val_t* ipp_create_attr_val_int(ipp_s32bit_t ival)
{
  ipp_attr_val_t* val;
  void* data = _create_signed_int(ival);
  if (!data) return NULL;
  val = ipp_create_attr_val(IPP_TAG_INTEGER, data, 4);
  IPP_FREE(data);
  return val;
}

/*----------------------------------------------------------------------------*/
ipp_attr_val_t* ipp_create_attr_val_bool(ipp_u8bit_t val)
{
  return ipp_create_attr_val(IPP_TAG_BOOLEAN, &val, 1);
}

/*----------------------------------------------------------------------------*/
ipp_attr_val_t* ipp_create_attr_val_name(char* val)
{
  return ipp_create_attr_val(IPP_TAG_NAME_WITHOUT_LANGUAGE, val, strlen(val));
}

/*----------------------------------------------------------------------------*/
void ipp_free_attr_val(ipp_attr_val_t *val, int all)
{
  _fndbg("ENTER(%p)", val);
  if (val)
  {
    if (val->data) IPP_FREE(val->data);
    if (all) IPP_FREE(val);
  }
  _fndbg("EXIT(%p)", val);
}

/*----------------------------------------------------------------------------*/
static char* _makestr(char* data, int len)
{
  char* s = IPP_MALLOC(len+1);
  if (s)
  {
    memcpy(s, data, len);
    s[len] = 0;
  }
  else
  {
     ASSERT(0);
  }
  return s;
}

/*----------------------------------------------------------------------------*/
void ipp_dump_attr_val(ipp_attr_val_t *val)
{
  if (!val) return;
  unsigned char *d = (unsigned char*)val->data;

  const char* syntax = ipp_const_tag_str(val->syntax);
  if ((val->syntax == IPP_TAG_CHARSET) ||
      (val->syntax == IPP_TAG_NATURAL_LANGUAGE) ||
      (val->syntax == IPP_TAG_MIME_MEDIA_TYPE) ||
      (val->syntax == IPP_TAG_KEYWORD) ||
      (val->syntax == IPP_TAG_URI) ||
      (val->syntax == IPP_TAG_URI_SCHEME) ||
      (val->syntax == IPP_TAG_TEXT_WITHOUT_LANGUAGE) ||
      (val->syntax == IPP_TAG_NAME_WITHOUT_LANGUAGE))
  {
    char* s = _makestr(val->data, val->len);
    _dbg0("      VALUE: Syntax=%s, Data=%s", syntax, s);
    IPP_FREE(s);
  }
  else if (val->syntax == IPP_TAG_BOOLEAN)
  {
    if (val->len != 1)
      _dbg0("      VALUE: Syntax=%s, (invalid len = %d)",syntax,val->len);
    else if (d[0] > 1)
      _dbg0("      VALUE: Syntax=%s, (invalid value = %d)",syntax,d[0]);
    else
      _dbg0("      VALUE: Syntax=%s, %s",syntax,d[0]?"TRUE":"FALSE");
  }
  else if ((val->syntax == IPP_TAG_INTEGER) ||
           (val->syntax == IPP_TAG_ENUM))
  {
    if (val->len != 4)
      _dbg0("      VALUE: Syntax=%s, (invalid len = %d)",syntax,val->len);
    else
    {
      union
      {
          signed int sval;
        unsigned int uval;
      } v;
      v.uval  = d[0]; v.uval <<= 8;
      v.uval += d[1]; v.uval <<= 8;
      v.uval += d[2]; v.uval <<= 8;
      v.uval += d[3];
      _dbg0("      VALUE: Syntax=%s, %d",syntax,v.sval);
    }
  }
  else
  {
    _dbg0("      VALUE: Syntax=%s, Data_len=%d", syntax, val->len);
  }
}

/*----------------------------------------------------------------------------*/
ipp_attr_t* ipp_find_attr(ipp_attr_groups_t* grps, const char* attrname)
{
  if (grps)
  {
    int g;
    for (g=0;g<grps->num_grp;g++)
    {
      int a;
      ipp_attr_grp_t* grp = grps->grp + g;
      for (a=0;a<grp->num_attr;a++)
      {
        ipp_attr_t* attr = grp->attr + a;
        if (!strcmp(attr->name, attrname))
          return attr;
      }
    }
  }
  return NULL;
}
