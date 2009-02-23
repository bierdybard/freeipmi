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

#ifndef _IPMI_FILL_UTIL_H
#define	_IPMI_FILL_UTIL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "freeipmi/fiid/fiid.h"

#include "libcommon/ipmi-trace.h"

#define FILL_FIID_OBJ_CLEAR(__obj)                                          \
do {                                                                        \
    if (fiid_obj_clear ((__obj)) < 0)                                       \
      {                                                                     \
         FIID_OBJECT_ERROR_TO_ERRNO((__obj));                               \
         return (-1);                                                       \
      }                                                                     \
} while (0)

#define FILL_FIID_OBJ_SET(__obj, __field, __val)                            \
do {                                                                        \
    if (fiid_obj_set ((__obj), (__field), (__val)) < 0)                     \
      {                                                                     \
         FIID_OBJECT_ERROR_TO_ERRNO((__obj));                               \
         return (-1);                                                       \
      }                                                                     \
} while (0)

#define FILL_FIID_OBJ_SET_DATA(__obj, __field, __data, __data_len)          \
do {                                                                        \
    if (fiid_obj_set_data ((__obj), (__field), (__data), (__data_len)) < 0) \
      {                                                                     \
         FIID_OBJECT_ERROR_TO_ERRNO((__obj));                               \
         return (-1);                                                       \
      }                                                                     \
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* ipmi-fill-util.h */
