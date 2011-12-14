/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef _IPMI_SEL_PARSE_STRING_INTEL_H
#define _IPMI_SEL_PARSE_STRING_INTEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdint.h>

#include "freeipmi/sel-parse/ipmi-sel-parse.h"

#include "ipmi-sel-parse-defs.h"
#include "ipmi-sel-parse-common.h"

int ipmi_sel_parse_output_intel_sensor_name (ipmi_sel_parse_ctx_t ctx,
					     struct ipmi_sel_parse_entry *sel_parse_entry,
					     uint8_t sel_record_type,
					     char *buf,
					     unsigned int buflen,
					     unsigned int flags,
					     unsigned int *wlen,
					     struct ipmi_sel_system_event_record_data *system_event_record_data,
					     int *oem_rv);

int ipmi_sel_parse_output_intel_event_data1_class_oem (ipmi_sel_parse_ctx_t ctx,
                                                       struct ipmi_sel_parse_entry *sel_parse_entry,
                                                       uint8_t sel_record_type,
                                                       char *tmpbuf,
                                                       unsigned int tmpbuflen,
                                                       unsigned int flags,
                                                       unsigned int *wlen,
                                                       struct ipmi_sel_system_event_record_data *system_event_record_data);

int ipmi_sel_parse_output_intel_event_data2_discrete_oem (ipmi_sel_parse_ctx_t ctx,
                                                          struct ipmi_sel_parse_entry *sel_parse_entry,
                                                          uint8_t sel_record_type,
                                                          char *tmpbuf,
                                                          unsigned int tmpbuflen,
                                                          unsigned int flags,
                                                          unsigned int *wlen,
                                                          struct ipmi_sel_system_event_record_data *system_event_record_data);

int ipmi_sel_parse_output_intel_event_data2_class_oem (ipmi_sel_parse_ctx_t ctx,
                                                       struct ipmi_sel_parse_entry *sel_parse_entry,
                                                       uint8_t sel_record_type,
                                                       char *tmpbuf,
                                                       unsigned int tmpbuflen,
                                                       unsigned int flags,
                                                       unsigned int *wlen,
                                                       struct ipmi_sel_system_event_record_data *system_event_record_data);

int ipmi_sel_parse_output_intel_event_data3_discrete_oem (ipmi_sel_parse_ctx_t ctx,
                                                          struct ipmi_sel_parse_entry *sel_parse_entry,
                                                          uint8_t sel_record_type,
                                                          char *tmpbuf,
                                                          unsigned int tmpbuflen,
                                                          unsigned int flags,
                                                          unsigned int *wlen,
                                                          struct ipmi_sel_system_event_record_data *system_event_record_data);

int ipmi_sel_parse_output_intel_event_data3_class_oem (ipmi_sel_parse_ctx_t ctx,
                                                       struct ipmi_sel_parse_entry *sel_parse_entry,
                                                       uint8_t sel_record_type,
                                                       char *tmpbuf,
                                                       unsigned int tmpbuflen,
                                                       unsigned int flags,
                                                       unsigned int *wlen,
                                                       struct ipmi_sel_system_event_record_data *system_event_record_data);

int ipmi_sel_parse_output_intel_event_data2_event_data3 (ipmi_sel_parse_ctx_t ctx,
							 struct ipmi_sel_parse_entry *sel_parse_entry,
							 uint8_t sel_record_type,
							 char *buf,
							 unsigned int buflen,
							 unsigned int flags,
							 unsigned int *wlen,
							 struct ipmi_sel_system_event_record_data *system_event_record_data,
							 int *oem_rv);

#endif /* _IPMI_SEL_PARSE_STRING_INTEL_H */