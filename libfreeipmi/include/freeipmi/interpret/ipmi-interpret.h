/*
  Copyright (C) 2003-2010 FreeIPMI Core Team

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

#ifndef _IPMI_INTERPRET_H
#define _IPMI_INTERPRET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define IPMI_INTERPRET_ERR_SUCCESS                            0
#define IPMI_INTERPRET_ERR_CONTEXT_NULL                       1
#define IPMI_INTERPRET_ERR_CONTEXT_INVALID                    2
#define IPMI_INTERPRET_ERR_PARAMETERS                         3
#define IPMI_INTERPRET_ERR_OUT_OF_MEMORY                      4
#define IPMI_INTERPRET_ERR_PERMISSION                         5
#define IPMI_INTERPRET_ERR_SENSOR_CONFIG_FILE_DOES_NOT_EXIST  6        
#define IPMI_INTERPRET_ERR_SENSOR_CONFIG_FILE_PARSE           7
#define IPMI_INTERPRET_ERR_SYSTEM_ERROR                       8
#define IPMI_INTERPRET_ERR_OVERFLOW                           9
#define IPMI_INTERPRET_ERR_INTERNAL_ERROR                    10
#define IPMI_INTERPRET_ERR_ERRNUMRANGE                       11

#define IPMI_INTERPRET_FLAGS_DEFAULT                     0x0000

#define IPMI_INTERPRET_SENSOR_STATE_NOMINAL                0x00
#define IPMI_INTERPRET_SENSOR_STATE_WARNING                0x01
#define IPMI_INTERPRET_SENSOR_STATE_CRITICAL               0x02
#define IPMI_INTERPRET_SENSOR_STATE_UNKNOWN                0x03

typedef struct ipmi_interpret_ctx *ipmi_interpret_ctx_t;

/* Interpret Context Functions */
ipmi_interpret_ctx_t ipmi_interpret_ctx_create (void);
void ipmi_interpret_ctx_destroy (ipmi_interpret_ctx_t ctx);
int ipmi_interpret_ctx_errnum (ipmi_interpret_ctx_t ctx);
char * ipmi_interpret_ctx_strerror (int errnum);
char * ipmi_interpret_ctx_errormsg (ipmi_interpret_ctx_t ctx);

/* interpret flag functions */
int ipmi_interpret_ctx_get_flags (ipmi_interpret_ctx_t ctx, unsigned int *flags);
int ipmi_interpret_ctx_set_flags (ipmi_interpret_ctx_t ctx, unsigned int flags);

/* interpret file config loading */
/* specify NULL for default config file */
/* if not called, library default will always be used */
int ipmi_interpret_load_sensor_config (ipmi_interpret_ctx_t ctx,
                                       const char *sensor_config_file);

/* interpret core functions */
int ipmi_interpret_sensor (ipmi_interpret_ctx_t ctx,
                           uint8_t event_reading_type_code,
                           uint8_t sensor_type,
                           uint16_t sensor_event_bitmask,
                           unsigned int *sensor_state);

#ifdef __cplusplus
}
#endif

#endif /* _IPMI_INTERPRET_H */
