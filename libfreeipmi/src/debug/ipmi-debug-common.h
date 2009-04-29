/*
  Copyright (C) 2003-2009 FreeIPMI Core Team

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

#ifndef _IPMI_DEBUG_COMMON_H
#define _IPMI_DEBUG_COMMON_H

#include <stdint.h>

#include "libcommon/ipmi-trace.h"

#define IPMI_DEBUG_MAX_PREFIX_LEN 32
#define IPMI_DEBUG_MAX_HDR_LEN 1024
#define IPMI_DEBUG_MAX_PKT_LEN 65536

/* Portable version of the extremely unportable Linux dprintf() */
int ipmi_debug_dprintf (int fd, char *fmt, ...);

int ipmi_debug_set_prefix (char *buf, unsigned int buflen, const char *prefix);

int ipmi_debug_output_str (int fd, const char *prefix, const char *str);

int ipmi_debug_output_byte_array (int fd, const char *prefix, uint8_t *buf, uint32_t buf_len);

#endif /* ipmi-debug-common.h */


