/* 
   freeipmi-portability.h - portability includes & defines for libfreeipmi

   Copyright (C) 2003, 2004, 2005 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  
*/

#ifndef _FREEIPMI_PORTABILITY_H
#define	_FREEIPMI_PORTABILITY_H	1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>	/* For FILE definition */

#if  __WORDSIZE == 64
#define FI_64 "%l"
#else
#define FI_64 "%ll"
#endif

# if ENABLE_NLS
#  include <libintl.h>
#  define _(Text) gettext (Text)
# else
#  define textdomain(Domain)
#  define _(Text) Text
# endif
# define N_(Text) Text

#if !defined(O_SYNC) && defined(O_FSYNC)
#define O_SYNC	O_FSYNC
#endif

/* FreeBSD don't have log2(), exp10() and exp2() */
#ifndef HAVE_LOG2
#define log2(x)		(M_LOG2E * log((x)))
#endif
#ifndef HAVE_EXP10
#define exp10(x)	(pow(10.0, (x)))
#endif
#ifndef HAVE_EXP2
#define exp2(x)		(pow(2.0, (x)))
#endif

/* FreeBSD don't have strdupa */
#ifndef strdupa
/* Duplicate S, returning an identical alloca'd string.  */
# define strdupa(s)								\
	({											\
		const char *__old = (s);				\
		size_t __len = strlen (__old) + 1;		\
		char *__new = (char *) alloca (__len);	\
		(char *) memcpy (__new, __old, __len);	\
	})
#endif

#ifndef STDC_HEADERS
#ifndef HAVE_MEMCPY
static inline void*
freeipmi_memcpy (void *dest, const void *src, size_t n)
{
  while (0 <= --n) ((unsigned char*)dest) [n] = ((unsigned char*)src) [n];
  return dest;
}
#define memcpy freeipmi_memcpy
#endif /* HAVE_MEMCPY */
#  ifndef HAVE_MEMSET
static inline void*
freeipmi_memset (void *s, int c, size_t n)
{
  while (0 <= --n) ((unsigned char*)s) [n] = (unsigned char) c;
  return s;
}
#define memset freeipmi_memset
#endif /* HAVE_MEMSET */
#ifndef HAVE_STRCHR
static inline char*
freeipmi_strchr (const char* s, int c)
{
  while (*s != '\0')
    if (*s == (char)c) return s;
    else s++;
  return NULL;
}
# define strchr	freeipmi_strchr
#endif /* HAVE_STRCHR */
#endif /* STDC_HEADERS */

/* FreeBSD don't have strndup() */
#ifndef HAVE_STRNDUP
#define strndup	freeipmi_strndup
char *freeipmi_strndup(const char *, size_t);
#endif

/* FreeBSD don't have getline() */
#ifndef HAVE_GETLINE
#define getline	freeipmi_getline
ssize_t freeipmi_getline(char **buf, size_t *bufsize, FILE *fp);
#endif

#ifndef HAVE_FUNC_GETHOSTBYNAME_R_6
#define HAVE_FUNC_GETHOSTBYNAME_R_6	1
#define LOCAL_GETHOSTBYNAME_R
#ifdef gethostbyname_r
#undef gethostbyname_r
#endif
#define gethostbyname_r	freeipmi_gethostbyname_r
int freeipmi_gethostbyname_r(const char *name,
	struct hostent *ret,
	char *buf,
	size_t buflen,
	struct hostent **result,
	int *h_errnop);
#endif /* !HAVE_FUNC_GETHOSTBYNAME_R_6 */

#ifdef __cplusplus
}
#endif

#endif /* freeipmi-portability.h */
