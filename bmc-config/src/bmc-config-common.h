/* 
   $Id: bmc-config-common.h,v 1.7 2007-09-07 21:09:53 chu11 Exp $ 

   Copyright (C) 2005 FreeIPMI Core Team

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

#ifndef _BMC_CONFIG_COMMON_H
#define _BMC_CONFIG_COMMON_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <argp.h>
#include <pwd.h>
#include <termios.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else /* !TIME_WITH_SYS_TIME */
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else /* !HAVE_SYS_TIME_H */
#include <time.h>
#endif /* !HAVE_SYS_TIME_H */
#endif  /* !TIME_WITH_SYS_TIME */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <limits.h>

#include <freeipmi/freeipmi.h>
#include <freeipmi/udm/udm.h>

#include "cmdline-parse-common.h"
#include "format-text.h"
#include "freeipmi-portability.h"
#include "tool-common.h"
#include "ipmi-sensor-api.h"
#include "xmalloc.h"

#define COMMENT_PREFIX          '#'
#define COMMENT_COLUMN_WIDTH    80
#define COMMENT_BUFLEN          4096

#define SET_SELECTOR      0x0
#define BLOCK_SELECTOR    0x0

#define same(a,b) (strcasecmp(a,b) == 0)

#endif
