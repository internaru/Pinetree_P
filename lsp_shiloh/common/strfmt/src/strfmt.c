/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "stdarg.h"
#include "strfmt.h"
#include <stdint.h>
#include <math.h>

#define _INCLUDE_FLOAT (1)

/* This defines an unsigned integer which is wide enough to hold any pointer. */
#define INT_FOR_HOLDING_PTR uintptr_t

typedef struct
{
  void    (*outch)(char, void*);
  void     *data;
  int     field_width;
  int     precision;
  int     len;
  enum {   /* expected type based on length-modifier ('h','l','L') */
    NT_SZD, /* Default - most likely 4 */
    NT_SZ1, /* 'hh' */
    NT_SZ2, /* 'h' */
    NT_SZ4, /* 'l' or 'L' */
    NT_SZ8, /* 'll' or 'L' */
  } num_type;
  unsigned int left_adjust    :1; /* '-': left-justify within field width */
  unsigned int always_sign    :1; /* '+': always print number with a sign */
  unsigned int space_for_sign :1; /* ' ': if 1st char not sign, use space */
  unsigned int pad_with_zeroes:1; /* '0': pad number with leading zeroes */
  unsigned int use_alt_form   :1; /* '#': use alt form (prefix with 0 or 0x) */
} context_t;


static int _isdigit(unsigned char c)
{
  return ((c >= '0') && (c <= '9'));
}

/* outputs a character. All output goes through here. */
static void _put(char ch, context_t* context)
{
  (context->len)++;
  if (context->outch) context->outch(ch, context->data);
}

/* _pad outputs pad characters to fill out the field_width. */
static void _pad(int totlen, char padchar, context_t* context)
{
  int i;
  for (i=(context->field_width - totlen); i>0; i--) _put(padchar, context);
}

/* _prefix outputs prefix followed by any necessary padding.
 *
 * Inputs:  prefbuf = ptr to buffer containing prefix (usually a sign char)
 *	    preflen = number of characters in the prefix
 *	    totlen  = # chars in item about to be output, including prefix
 */
static void _prefix(char* prefbuf, int preflen,
                    int totlen, context_t* context)
{
  int i;

  if      (context->left_adjust)
  {
    for (i=preflen; i>0; i--) _put(*prefbuf++, context);
  }
  else if (context->pad_with_zeroes)
  {
    for (i=preflen; i>0; i--) _put(*prefbuf++, context);
    _pad(totlen,'0', context);
  }
  else
  {
    _pad(totlen, ' ', context);
    for (i=preflen; i>0; i--) _put(*prefbuf++, context);
  }
}

/* _suffix pads right end of field_width with spaces as necessary. */
static void _suffix(int totlen, context_t* context)
{
  if (context->left_adjust) _pad(totlen, ' ', context);
}

/* _str outputs a string, affected by the flags and width specifications */
static void _str(char* s, context_t* context, int is_NULL_c)
{
  int source_width, i;
  char* cp;

  if (is_NULL_c) source_width = 1, cp = 0; /* Needed to prevent compiler err */
  else
    for (source_width=0,cp=s; *cp++; source_width++)
      ;

  if (context->precision>=0 && source_width>context->precision)
    source_width = context->precision;

  _prefix(cp, 0, source_width, context);
  for (i=source_width; i>0; i--) _put(*s++, context);
  _suffix(source_width, context);
}

/* _char outputs a character, affected by flags and field_width */
static void _char(char c, context_t* context)
{
  char buf[2];

  buf[0] = c;  buf[1] = 0;
  context->precision = -1;
  _str(buf, context, c==0);
}

/* _int outputs an integer, affected by the flags and width spec's */
static void _int(long long     num,       /* the number to be output */
                 unsigned      ubase,     /* the number base (8, 10 or 16) */
                 unsigned char its_signed,/* is the number signed? */
                 const char    *digits,   /* 16-entry char array "0-9A-F" */
                 context_t     *context)
{
  unsigned long long unum;
  unsigned char negative;
  char          numbuf[24];  /* worst case is a 64-bit integer in octal */
  char          prefbuf[4];  /* worst case is "-0X\0" */
  char          *numptr, *prefptr;
  int           numlen, preflen, ileadz, totlen;
  int           i;

  negative = 0;

  if      (!its_signed)          unum =  num;
  else if ((negative = (num<0))) unum = -num;
  else                           unum =  num;

  if (context->precision < 0) context->precision = 1;
  else                        context->pad_with_zeroes = 0;

  /* build number backwards in numbuf */
  numptr = numbuf;
  do
  {
    *numptr++ = digits[(int)(unum % ubase)];
  } while ((unum /= ubase) > 0);
  numlen = numptr - numbuf;

  if (context->precision==0 && num==0)
  {
    numlen = 0;
    numptr = numbuf;
  }

  /* build prefix (sign and "alternate" stuff) in prefbuf */
  prefptr = prefbuf;
  if (its_signed)
  {
    if      (negative)                *prefptr = '-';
    else if (context->always_sign)    *prefptr = '+';
    else if (context->space_for_sign) *prefptr = ' ';
    else                              *prefptr = 0;
  }
  else                                *prefptr = 0;

  if (*prefptr != 0) *++prefptr = 0;

  if (context->use_alt_form   /* && *(numptr-1)!='0' */  )
  {
    if (ubase != 10) *prefptr++ = '0';
    if (ubase == 16)
    {
      if (digits[15] == 'f') *prefptr++ = 'x';
      else                   *prefptr++ = 'X';
    }
  }
  preflen = prefptr - prefbuf;

  /* compute ileadz -- the number of leading zeroes to output */
  ileadz = context->precision - numlen;
  if (ubase==8 && context->use_alt_form)
    ileadz--;   /* the leading 0 for octal counts as a pad-zero too */
  if (ileadz < 0) ileadz = 0;

  /* output the number */
  totlen = preflen + ileadz + numlen;
  _prefix(prefbuf, preflen, totlen, context);
  for (i=ileadz; i>0; i--) _put ('0', context);
  while (numptr > numbuf) _put (*(--numptr), context);
  _suffix(totlen, context);
}


#if _INCLUDE_FLOAT
static int _decimalify(unsigned long long n, char* digits, int maxdigits)
{
  int num = 0;
  while (maxdigits-- && n)
  {
    *digits++ = '0' + n%10;
    n /= 10;
    num++;
  }
  return num;
} 

static void _float(double n, char f, context_t* c)
{
  /* No support yet for field width, 0 pad, left justify, or alternate form */

  int u, neg, e, i, j;
  char digits[21]; /* 64 bits of data will fit in 20 decimal digits */

  if ('A' > 'a') { if ((u = (f >= 'A'))) f = f - 'A' + 'a'; }
  else           { if ((u = (f <  'a'))) f = f - 'A' + 'a'; }

  /* Handle special case of NaN */
  if (isnan(n)) 
  {
    if (u) {_put('N', c); _put('A', c); _put('N', c);}
    else   {_put('n', c); _put('a', c); _put('n', c);}
    return;
  }

  neg = (n < 0);
  if (neg) n = -n;

  /* Handle sign processing */
  if      (neg)               _put('-', c);
  else if (c->always_sign)    _put('+', c);
  else if (c->space_for_sign) _put(' ', c);

  /* Handle special case for infinity NaN */
  if (isinf(n))
  {
    if (u) {_put('I', c); _put('N', c); _put('F', c);}
    else   {_put('i', c); _put('n', c); _put('f', c);}
    return;
  }

  e = (int)((n==0.0)?0.0:((n<1.0)?(log10(n)-1):log10(n)));

  /* All floating point conversions use a default of 6 for the precision */
  if (c->precision == -1) c->precision = 6;

  /* 'g' and 'G' do not allow precision to be 0 */
  if ((f == 'g') && (!c->precision)) c->precision = 1;

  if (f == 'g')
  {
    if ((e < -4) || (e >= c->precision)) f = 'e';
    else                                 f = 'f';
  }

  if (f == 'e')
  {
    /* We need to print n as w.fffE+/-ee where number of f's is given
       by precision and both e's are given as zero if need be. */
    n = n * pow(10, c->precision - e);
    i = _decimalify((unsigned long long)n, digits, sizeof(digits));
    digits[i] = 0;
    if (i) _put(digits[i-1], c);
    else   _put('0',c);
    for (j=i-2;j>=0;j--) if (digits[j] != '0') break;
    if (j >= 0)
    {
      _put('.', c);
      for (j=i-2;j>=0;j--) _put(digits[j], c);
    }
    _put(u?'E':'e',c);
    _put((e<0)?'-':'+',c);
    if (e<0) e = -e;
    e = _decimalify(e, digits, sizeof(digits));
    if (e < 1) _put('0', c);
    if (e < 2) _put('0', c);
    for (j=0;j<e;j++) _put(digits[e-j-1], c);
  }
  else if (f == 'f')
  {
    /* We need to print n as wwww.fff where number of f's is given
       by precision. www must be present even if it is 0. */
    n = n * pow(10, c->precision);
    i = _decimalify((unsigned long long)n, digits, sizeof(digits));
    digits[i] = 0;
    if (i <= c->precision)
      _put('0', c);
    for (j=i-1;j>=c->precision;j--)
      _put(digits[j], c);
    if (c->precision)
      _put('.', c);
    for (j=c->precision-1;j>=i;j--)
      _put('0', c);
    for (     ;j>=0;j--)
      _put(digits[j], c);
  }
}
#endif

/* _getnum gets a decimal number from the format string */
static int _getnum(const char** linep)
{
  int n = 0;
  const char* cp = *linep;

  while (_isdigit(*cp))
    n = n*10 + ((*cp++) - '0');
  *linep = cp;
  return n;
}

int vfstrfmt(void (*outch)(char, void*), void* data,
             const char* fmt, va_list ap)
{
  context_t context_space;
  context_t* context;
  enum {CHARS, FLAGS, CONVERT} state;
  char ch;

  static const char lc_digits[] = "0123456789abcdef";
  static const char uc_digits[] = "0123456789ABCDEF";

  context = &context_space;

  context->outch = outch;
  context->data  = data;
  context->len = 0;

  state = CHARS;

  while ((ch=*fmt))  /* main loop to scan thru control string */
  {
    switch (state)
    {
      case CHARS:   /* output normal characters */
      {
        if (ch != '%') _put(ch, context);
        else
        {
          context->left_adjust     = 0;
          context->always_sign     = 0;
          context->space_for_sign  = 0;
          context->pad_with_zeroes = 0;
          context->use_alt_form    = 0;
          context->num_type        = NT_SZD;
          context->field_width     = 0;
          context->precision       = -1;
          state           = FLAGS;
        }
        fmt++;
        break;
      }

      case FLAGS:   /* parsing flags in a specification */
      {
        if ((_isdigit(ch) && ch !='0') || ch=='*' || ch=='.')
        {
          if (ch != '*')
          {
            if (ch != '.') context->field_width = _getnum(&fmt);
          }
          else
          {
            context->field_width = va_arg(ap, int);
            if (context->field_width < 0) context->field_width = 0;
            fmt++;
          }

          if (!(ch=*fmt))
          {
           _put('?', context); _put('?', context);
           return -1;
          }

          if (ch == '.')
          {
            ch = *(++fmt);
            if (ch != '*')
            {
              if (_isdigit(ch)) context->precision = _getnum(&fmt);
              else              context->precision = 0;
            }
            else
            {
              context->precision = va_arg(ap, int);
              if (context->precision < 0) context->precision = 0;
              fmt++;
            }
          }
          state = CONVERT;
        }
        else
        {
          switch (ch)
          {
            case '-':  context->left_adjust     = 1;  fmt++;  break;
            case '+':  context->always_sign     = 1;  fmt++;  break;
            case ' ':  context->space_for_sign  = 1;  fmt++;  break;
            case '0':  context->pad_with_zeroes = 1;  fmt++;  break;
            case '#':  context->use_alt_form    = 1;  fmt++;  break;
            default:   state = CONVERT;
          } /* end of switch (flag char) */
        }
        break;
      }

      case CONVERT:   /* ready to get the conversion character */
      {
        state = CHARS; /* ASSUME we'll go here next */
        if (context->left_adjust) context->pad_with_zeroes = 0;
        switch (ch)
        {
          case 'h': if (context->num_type == NT_SZ2) context->num_type = NT_SZ1;
                    else                             context->num_type = NT_SZ2;
                    state = CONVERT;
                    break;
          case 'L':
          case 'l': if (context->num_type == NT_SZ4) context->num_type = NT_SZ8;
                    else                             context->num_type = NT_SZ4;
                    state = CONVERT;
                    break;
          case 'd':
          case 'i': {
                      long long n;
                      switch (context->num_type)
                      {
#define _CASE(c, t, t2) c: n = (t)va_arg(ap, t2); break
                      _CASE(case NT_SZ1, signed char,     int);
                      _CASE(case NT_SZ2, signed short,    int);
                      _CASE(default,     signed int,      signed int);
                      _CASE(case NT_SZ4, signed long int, signed long int);
                      _CASE(case NT_SZ8, signed long long,signed long long);
#undef _CASE
                      }
                      _int(n,10,1,lc_digits,context);
                      break;
                    }
          case 'x': 
          case 'X':
          case 'o':
          case 'u': {
                      unsigned long long u;
                      switch (context->num_type)
                      {
#define _CASE(c, t, t2) c: u = (t)va_arg(ap, t2); break
                      _CASE(case NT_SZ1, unsigned char,     unsigned int);
                      _CASE(case NT_SZ2, unsigned short,    unsigned int);
                      _CASE(default,     unsigned int,      unsigned int);
                      _CASE(case NT_SZ4, unsigned long int, unsigned long int);
                      _CASE(case NT_SZ8, unsigned long long,unsigned long long);
#undef _CASE
                      }
                      switch(ch)
                      {
                        case 'x': _int(u,16,0,lc_digits,context); break;
                        case 'X': _int(u,16,0,uc_digits,context); break;
                        case 'o': _int(u, 8,0,lc_digits,context); break;
                        case 'u': _int(u,10,0,lc_digits,context); break;
                      }
                      break;
                    }
#if _INCLUDE_FLOAT
          case 'e':
          case 'f':
          case 'g':
          case 'E': 
          case 'F': 
          case 'G': {
                      double num;
                      if (context->num_type == NT_SZ8)
                        num = (double)va_arg(ap, long double); 
                      else
                        num = (double)va_arg(ap,      double); 
                      _float(num,ch,context);
                      break;
                    }
#endif

          case 's': _str(va_arg(ap,char*), context, 0);  break;
          case 'c': _char(va_arg(ap,int), context);      break;
          case '%': _char('%', context);                 break;
          case 'n': switch (context->num_type)
                    {
#define _CASE(c, t) c: *va_arg(ap,t*) = context->len; break
                      _CASE(case NT_SZ1, signed char);
                      _CASE(case NT_SZ2, signed short);
                      _CASE(default,     signed int);
                      _CASE(case NT_SZ4, signed long int);
                      _CASE(case NT_SZ8, signed long long);
#undef _CASE
                    }
          case 'p': 
              context->precision = 2 * sizeof(INT_FOR_HOLDING_PTR);
              _int((INT_FOR_HOLDING_PTR)va_arg(ap,void*), 16, 0, lc_digits, context);
                    break;

          default:  _put('?', context);
                    _put('?', context);
                    return -1;
        } /* end of switch (conversion char) */

        fmt++;
        break;
      }
    } /* end of switch (state) */
  } /* end of main loop */

  if (state != CHARS)
  {
    _put('?', context);
    _put('?', context);
    return -1;
  }

  return context->len;
}

static void _sprintf(char c, void* data) { *(*((char**)data))++ = c; }
int strfmt(char* str, const char* fmt, ...)
{
  va_list ap;
  int r;

  if (!str || !fmt || !*fmt) return -1; 

  va_start(ap, fmt);
  r = vfstrfmt(_sprintf, &str, fmt, ap);
  va_end(ap);
  if (r >= 0)
  {
    *str++ = 0;
    return r;
  }
  return -1;
}

#if 0 /* The following are some test cases */

#include "dprintf.h"

void strfmt_test(void)
{
  static char buffer[1024];
  const char* fmt;
  fmt = "%g"; strfmt(buffer, fmt, 345.65/0.0); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, -345.65/0.0); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, pow(-345.65,5.2)); dbg_printf("%s\n", buffer);
  fmt = "%G"; strfmt(buffer, fmt, pow(-345.65,5.2)); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, 0.0); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, .102345); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, .00102345); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, .00000000102345); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, 234565757.102345); dbg_printf("%s\n", buffer);
  fmt = "%.2e"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.2e"; strfmt(buffer, fmt, 10.0345); dbg_printf("%s\n", buffer);
  fmt = "%.4e"; strfmt(buffer, fmt, 10.0045); dbg_printf("%s\n", buffer);
  fmt = "%.0e"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.1e"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.9e"; strfmt(buffer, fmt, 1023.45); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, -0.0); dbg_printf("%s\n", buffer);
  fmt = "%e"; strfmt(buffer, fmt, -234565757.102345); dbg_printf("%s\n", buffer);
  fmt = "%.16e"; strfmt(buffer, fmt, -1234567890.123456789); dbg_printf("%s\n", buffer);
  fmt = "%.18e"; strfmt(buffer, fmt, -1234567890.123456789); dbg_printf("%s\n", buffer);
  fmt = "%.19e"; strfmt(buffer, fmt, -1234567890.123456789); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, 0.0); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, .102345); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, .00102345); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, .00000000102345); dbg_printf("%s\n", buffer);
  fmt = "%f"; strfmt(buffer, fmt, 234565757.102345); dbg_printf("%s\n", buffer);
  fmt = "%.2f"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.2f"; strfmt(buffer, fmt, 10.0345); dbg_printf("%s\n", buffer);
  fmt = "%.2f"; strfmt(buffer, fmt, 10.0045); dbg_printf("%s\n", buffer);
  fmt = "%.0F"; strfmt(buffer, fmt, 10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.9F"; strfmt(buffer, fmt, 1023.45); dbg_printf("%s\n", buffer);
  fmt = "%F"; strfmt(buffer, fmt, -0.0); dbg_printf("%s\n", buffer);
  fmt = "%F"; strfmt(buffer, fmt, -10.2345); dbg_printf("%s\n", buffer);
  fmt = "%.10f"; strfmt(buffer, fmt, -1234567890.123456789); dbg_printf("%s\n", buffer);
  fmt = "%.16f"; strfmt(buffer, fmt, -1.234567890123456789); dbg_printf("%s\n", buffer);
  fmt = "%.19f"; strfmt(buffer, fmt, -1.234567890123456789); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, 0.0); dbg_printf("%s\n", buffer);
  fmt = "%.2g"; strfmt(buffer, fmt, 0.0); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, 0.00012); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, 0.000012); dbg_printf("%s\n", buffer);
  fmt = "%g"; strfmt(buffer, fmt, 0.0000012); dbg_printf("%s\n", buffer);
  fmt = "%.3g"; strfmt(buffer, fmt, 12.0); dbg_printf("%s\n", buffer);
  fmt = "%.3g"; strfmt(buffer, fmt, 123.0); dbg_printf("%s\n", buffer);
  fmt = "%.3g"; strfmt(buffer, fmt, 1234.0); dbg_printf("%s\n", buffer);
}

#endif
