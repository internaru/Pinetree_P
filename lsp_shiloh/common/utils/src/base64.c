/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "base64_api.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

/*----------------------------------------------------------------------------*/
int base64_encode(base64_get_t get, void* getarg,
                  base64_put_t put, void* putarg)
{
  static const char _code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz"
                              "0123456789+/";
  unsigned char in[3], out[4];
  int l=0, c, p, i;

  while ((c = get(getarg))>=0)
  {
    p = 0;
    in[0] = c;
    for (i=1;i<3;i++)
    {
      if ((c = get(getarg))<0) c = 0, p++;
      in[i] = c;
    }

    out[0] =       _code[                    in[0]      >>2 ];
    out[1] =       _code[((in[0]&0x03)<<4)|((in[1]&0xf0)>>4)];
    out[2] = (p<2)?_code[((in[1]&0x0f)<<2)|((in[2]&0xc0)>>6)]:'=';
    out[3] = (p<1)?_code[  in[2]&0x3f                       ]:'=';
    for (i=0;i<4;i++) put(putarg, out[i]);
    l++;
  }
  return l*4;
}

/*----------------------------------------------------------------------------*/
int base64_decode(base64_get_t get, void* getarg,
                  base64_put_t put, void* putarg)
{
  unsigned char in[4], out[3];
  int p, c, i, l = 0;

  while (1)
  {
    p = 0;
    for (i=0;i<4;)
    {
      c = get(getarg);
      if      (c <    0) return l;
      else if (c >= 'a') in[i++] = c - 71;
      else if (c >= 'A') in[i++] = c - 65;
      else if (c == '=') in[i++] = 0, p += 1;
      else if (c >= '0') in[i++] = c + 4;
      else if (c == '/') in[i++] = 63;
      else if (c == '+') in[i++] = 62;
      /* else - what the heck - unexpected input char - just skip for now */
    }
    out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
    for( i = 0; i < (3-p); i++ ) put(putarg, out[i]);
    l += i;
  }
  return l;
}

/*----------------------------------------------------------------------------*/
typedef struct
{
  unsigned char *src,   *dst;
  int            srclen, dstlen;
} _cntxt_t;

/*----------------------------------------------------------------------------*/
static int _get(void* cntxt)
{
  _cntxt_t* c = (_cntxt_t*)cntxt;
  if (c->srclen) { c->srclen--; return *c->src++; }
  else                          return -1;
}

/*----------------------------------------------------------------------------*/
static int _put(void* cntxt, unsigned char data)
{
  _cntxt_t* c = (_cntxt_t*)cntxt;
  if      (c->dstlen < 0) {              *c->dst++ = data; return  1; }
  else if (c->dstlen)     { c->dstlen--; *c->dst++ = data; return  1; }
  else                                                     return -1;
}

/*----------------------------------------------------------------------------*/
int base64_dec(void* dst, const char* src)
{
  _cntxt_t cntxt;
  cntxt.src    = (unsigned char*)src;
  cntxt.srclen = strlen(src);
  cntxt.dst    = dst;
  cntxt.dstlen = -1;
  return base64_decode(_get, &cntxt, _put, &cntxt);
}

/*----------------------------------------------------------------------------*/
int base64_enc(void* dst, void* src, unsigned int len)
{
  _cntxt_t cntxt;
  cntxt.src    = src;
  cntxt.srclen = len;
  cntxt.dst    = dst;
  cntxt.dstlen = -1;
  return base64_encode(_get, &cntxt, _put, &cntxt);
}

#ifdef HAVE_CMD
/*----------------------------------------------------------------------------*/
static int _capture(void* cntxt, unsigned char data)
{
  cmd_append_result("%c", data);
  return 1;
}

/*----------------------------------------------------------------------------*/
static int _printf(void* cntxt, unsigned char data)
{
  cmd_printf("%c", data);
  return 1;
}

/*----------------------------------------------------------------------------*/
static const char* _cmd_enc_usage = "<src addr> <len>";
static const char* _cmd_enc_notes = "This command produces a base64 encoded "
  "block of data. It encodes the data located at the address <src addr> for "
  "<len> number of bytes.";
static int _cmd_enc(int argc, char **argv)
{
  _cntxt_t cntxt;
  if (argc != 3) return CMD_USAGE_ERROR;
  cntxt.src    = (unsigned char*)(uintptr_t)cmd_atoi(argv[1]);
  cntxt.srclen = cmd_atoi(argv[2]);
  base64_encode(_get, &cntxt, _capture, NULL);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char* _cmd_dec_usage = "<base64 string> <dst addr> [<maxlen>]";
static const char* _cmd_dec_notes = "This command decodes the base 64 string "
  "and stores the result in memory. It will write at most <maxlen> bytes to "
  "memory starting at the address given by <dst addr>. If <maxlen> is not "
  "specified then the amount of data written is governed solely by the length "
  "of the input string. This command's result is the number of bytes produced "
  "as output.";
static int _cmd_dec(int argc, char **argv)
{
  int n;
  _cntxt_t cntxt;
  if (argc < 3) return CMD_USAGE_ERROR;
  cntxt.src = (unsigned char*)argv[1];
  cntxt.srclen = strlen(argv[1]);
  cntxt.dst    = (unsigned char*)(uintptr_t)cmd_atoi(argv[2]);
  if (argc > 3) cntxt.dstlen = cmd_atoi(argv[3]);
  else          cntxt.dstlen = -1;
  if (!cntxt.dst)
  {
    cmd_append_result("Destination address may not be zero!\n");
    return CMD_ERROR;
  }
  n = base64_decode(_get, &cntxt, _put, &cntxt);
  cmd_append_result("%d", n);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char* _cmd_cat_usage = "<filename>";
static const char* _cmd_cat_notes = "This command produces a base64 "
  "encoded block of data. It encodes the data from the specified file.";
static int _read(void* arg)
{
  FILE* fhdl = (FILE*)arg;
  int c = fgetc(fhdl);
  if (c == EOF) return -1;
  return c;
}
static int _cmd_cat(int argc, char **argv)
{
  FILE* fhdl;

  if (argc != 2) return CMD_USAGE_ERROR;
  fhdl = fopen(argv[1], "r");
  if (!fhdl)
  {
    cmd_printf("File open error\n");
    return CMD_ERROR;
  }
  base64_encode(_read, fhdl, _printf, NULL);
  fclose(fhdl);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char* _cmd_store_usage = "<base64 string> <filename>";
static const char* _cmd_store_notes = "This command stores a decoded base64 "
  "string to a specified file (it appends).";
static int _cmd_store(int argc, char **argv)
{
  FILE* fhdl;
  const char* data = (const char*)argv[1];
  int n;

  if (argc != 3) return CMD_USAGE_ERROR;
  fhdl = fopen(argv[2], "a");
  if (!fhdl)
  {
    cmd_printf("File open error\n");
    return CMD_ERROR;
  }

  /* Safe because decoded is ALWAYS shorter than encoded */
  n = base64_dec((void*)data, data);
  if (1 != fwrite(data, n, 1, fhdl))
  {
    cmd_printf("Write failure\n");
    fclose(fhdl);
    return CMD_ERROR;
  }
  fclose(fhdl);
  return CMD_OK;
}
#endif

/*----------------------------------------------------------------------------*/
void base64_init(void)
{
#ifdef HAVE_CMD
  cmd_register("base64", "Commands for encoding or decoding base64 data",
               NULL, NULL, NULL);
  cmd_register("base64 encode", "Encode a block of data into base64",
               _cmd_enc_usage, _cmd_enc_notes, _cmd_enc);
  cmd_register("base64 decode", "Decode a block of base64 data",
               _cmd_dec_usage, _cmd_dec_notes, _cmd_dec);
  cmd_register("base64 cat", "Cat a file in base64 format",
               _cmd_cat_usage, _cmd_cat_notes, _cmd_cat);
  cmd_register("base64 store", "Append a decoded base64 string to a file",
               _cmd_store_usage, _cmd_store_notes, _cmd_store);
#endif
}
