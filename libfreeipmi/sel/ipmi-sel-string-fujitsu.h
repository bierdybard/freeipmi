/*
  Copyright (C) 2003-2013 FreeIPMI Core Team

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

#ifndef IPMI_SEL_STRING_FUJITSU_H
#define IPMI_SEL_STRING_FUJITSU_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdint.h>

#include "freeipmi/sel/ipmi-sel.h"

#include "ipmi-sel-defs.h"
#include "ipmi-sel-common.h"

int sel_string_output_fujitsu_event_data1_class_sensor_specific_discrete (ipmi_sel_ctx_t ctx,
									  struct ipmi_sel_entry *sel_entry,
									  uint8_t sel_record_type,
									  char *tmpbuf,
									  unsigned int tmpbuflen,
									  unsigned int flags,
									  unsigned int *wlen,
									  struct ipmi_sel_system_event_record_data *system_event_record_data);

int sel_string_output_fujitsu_event_data2_event_data3 (ipmi_sel_ctx_t ctx,
						       struct ipmi_sel_entry *sel_entry,
						       uint8_t sel_record_type,
						       char *buf,
						       unsigned int buflen,
						       unsigned int flags,
						       unsigned int *wlen,
						       struct ipmi_sel_system_event_record_data *system_event_record_data,
						       int *oem_rv);

int sel_string_output_fujitsu_oem_record_data (ipmi_sel_ctx_t ctx,
					       struct ipmi_sel_entry *sel_entry,
					       uint8_t sel_record_type,
					       char *buf,
					       unsigned int buflen,
					       unsigned int flags,
					       unsigned int *wlen,
					       int *oem_rv);

int sel_string_output_fujitsu_oem_string (ipmi_sel_ctx_t ctx,
					  struct ipmi_sel_entry *sel_entry,
					  uint8_t sel_record_type,
					  char *buf,
					  unsigned int buflen,
					  unsigned int flags,
					  unsigned int *wlen,
					  int *oem_rv);

#endif /* IPMI_SEL_STRING_FUJITSU_H */
