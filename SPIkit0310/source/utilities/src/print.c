/******************************************************************************/
/*                                                                            */
/*    $Workfile::   print.c                                            $    */
/*                                                                            */
/*    $Revision:: 4805                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-11-28 14:16:45 +0800 (周四, 28 十一月 2013)  $    */
/*                                                                            */
/*      $Author:: ricky.lu@philips.com                                   $    */
/*                                                                            */
/*        Owner::   Pieter-Jelle Gerbrandts                                             */
/*                                                                            */
/*  Description::   This module is responsible for                            */
/*                                                                            */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include "platform.h"
#include "stdtypes.h"
#include "uartdrv.h"
#include "print.h"

/*lint -save */
/*lint -w1 Only give Error messages, since it is used as an external library */

#define CR     "\r"

static void printchar(char **str, char c)
{
  
  if (str)
  {
    **str = c;
    ++(*str);
  }
  else
  {
    if (c == '\n')
    {
       (void)UARTDRV_teWrite(CR,1);
    }
    (void)UARTDRV_teWrite((void *)&c,1);
  }
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
  register int pc = 0;
  register char padchar = ' ';
  if (width > 0)
  {
    register int len = 0;
    register const char *ptr;
      
    for (ptr = string; *ptr; ++ptr)
      ++len;
    if (len >= width)
      width = 0;
    else
      width -= len;
    if (pad & PAD_ZERO)
      padchar = '0';
  }
  if (!(pad & PAD_RIGHT))
  {
    for ( ; width > 0; --width)
    {
      printchar (out, padchar);
      ++pc;
    }
  }
  for ( ; *string ; ++string)
  {
    printchar (out, *string);
    ++pc;
  }
  for ( ; width > 0; --width)
  {
    printchar (out, padchar);
    ++pc;
  }
  return pc;
}
  
/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12
static int printi(char **out, int i, unsigned int b, int sg, int width, int pad, char letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char *s;
  register char t;
  register unsigned int neg = 0;
  register int pc = 0;
  register unsigned int u = (unsigned int)i;
    
  if (i == 0)
  {
    print_buf[0] = '0';
    print_buf[1] = '\0';
    return prints (out, print_buf, width, pad);
  }
  if (sg && b == 10 && i < 0)
  {
    neg = 1;
    u = (unsigned int)(-i);
  }
  s = print_buf + PRINT_BUF_LEN-1;
  *s = '\0';
  
  while (u)
  {
    t = (char)(u % b);
    if( t >= 10 )
      t += (letbase - '0') - 10;
    *--s = t + '0';
    u /= b;
  }
  if (neg)
  {
    if( width && (pad & PAD_ZERO) )
    {
      printchar (out, '-');
      ++pc;
      --width;
    }
    else
    {
      *--s = '-';
    }
  }
  return pc + prints (out, s, width, pad);
}
  
static int printx(char **out, int *varg)
{
  register int width, pad;
  register int pc = 0; 
  register char *format = (char *)(*varg++);
  char scr[2];
  for (; *format != 0; ++format)
  {
    if (*format == '%')
    {
      ++format;
      width = pad = 0;
      if (*format == '\0')
        break;
      if (*format == '%')
        goto out;
      if (*format == '-')
      {
        ++format;
        pad = PAD_RIGHT;
      }
      while (*format == '0')
      {
        ++format;
        pad |= PAD_ZERO;
      }
      for ( ; *format >= '0' && *format <= '9'; ++format)
      {
        width *= 10;
        width += *format - '0';
      }
      if( *format == 's' )
      {
        register char *s = *((char **)varg++);
        pc += prints (out, s?s:"(null)", width, pad);
        continue;
      }
      if( *format == 'd' )
      {
        pc += printi (out, *varg++, 10, 1, width, pad, 'a');
        continue;
      }
      if( *format == 'x' )
      {
        pc += printi (out, *varg++, 16, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'X' )
      {
        pc += printi (out, *varg++, 16, 0, width, pad, 'A');
        continue;
      }
      if( *format == 'u' )
      {
        pc += printi (out, *varg++, 10, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'c' )
      {
        /* char are converted to int then pushed on the stack */
        scr[0] = (char)*varg++;
        scr[1] = '\0';
        pc += prints (out, scr, width, pad);
        continue;
      }
    }
    else
    {
out:    printchar (out, *format);
      ++pc;
    }
  }
  if (out)
    **out = '\0';
  return pc;
}

/* assuming sizeof(void *) == sizeof(int) */
int printf_embedded(const char *format, ...)
{
  register int *varg = (int *)(&format);
  return printx(0, varg);
}

int sprintf_embedded(char *out, const char *format, ...)
{
  register int *varg = (int *)(&format);
  return printx(&out, varg);
}

/*lint -restore */

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
