/* gppdemid.c
   Copyright 1993-1994 Eberhard Mattes
   Copyright 1996      Axel Zeuner

This file is part of the GPPDEMID library.
GPPDEMID is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

GPPDEMID is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */


/* Note: This is 32-bit code! */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <os2.h>
#include "demangle.h"
#include "tools.h"



// __export _DLL_InitTerm
APIRET _System _DLL_InitTerm(ULONG hModule, ULONG ulFlag)
{
   ULONG ulAction;
switch (ulFlag) {
   case 0:  // Init
      return (1);
   case 1:  // Exit
      SysDestroyHeap();
      return (1);
}
return (0);
}

/* These functions are imported by cplus-de.c.  Don't do error
   checking.  setjmp() and longjmp() could be used to make DEMANGLEID
   return 0 if out of memory.  Note: allocate one more byte and change
   the offset to 1 to work around a bug in string_prependn. */

char *xmalloc (size_t n)
{
   char* x= SysAlloc(n+1);
if (x) return x+1;
return x;
}


char *xrealloc (char *p, size_t n)
{
   unsigned long* h= (unsigned long*)(p-1);
   unsigned char* w;
w= SysAlloc(n+1);
if (w) {
   if (*(h-1)>n+1) {
      memcpy(w,p-1,n+1);
   } else {
      memcpy(w,p-1,*(h-1));
   } /* endif */
   SysFree((char*)h);
}
return w+1;
}


void free( void* p)
{
SysFree(((char*)p)-1);
}


/* cplus-de.c uses only %d. */

int sprintf (char *dst, const char *fmt, ...)
{
  int i;
  char *d;
  va_list arg_ptr;

  d = dst;
  va_start (arg_ptr, fmt);
  while (*fmt != 0)
    if (*fmt != '%')
      *d++ = *fmt++;
    else if (fmt[1] == '%')
      *d++ = '%', fmt += 2;
    else
      {
        ++fmt;
        if (*fmt == 0)
          break;
        switch (*fmt)
          {
          case 'd':
            i = va_arg (arg_ptr, int);
            d += strlen (_itoa (i, d, 10));
            break;
          default:
            *d++= '%';
            *d++= *fmt;
            break;
          }
        ++fmt;
      }
  *d = 0;
  va_end (arg_ptr);
  return (d - dst);
}

void abort()
{
DosExit(EXIT_PROCESS,17);
}

/* Initialization. */

unsigned long InitDemangleID32 (const char *init)
{
  return 1;                     /* Success */
}

/* Demangler. */

unsigned long DemangleID32(const char *src, char *dst,
                                  unsigned long dst_size)
{
  char *result;
  char mangled[256];
  size_t len;
  len = ((const unsigned char *)src)[0];

  memcpy (mangled, src+1, len);
  mangled[len] = 0;

  result = cplus_demangle (mangled, DMGL_PARAMS | DMGL_ANSI);
  if (result == NULL)
    return 0;                   /* Error */
  len = strlen (result);
  if ((unsigned long)len >= dst_size)
    return 0;                   /* Error */
  memcpy (dst, result, len + 1);
  free (result);
  return 1;                     /* Success */
}
