/* 
   ipmi-sensor-api.c - IPMI sensor commands API

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
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#if defined (IPMI_SYSLOG)
#include <syslog.h>
#endif /* IPMI_SYSLOG */

#include "ipmi-sensor-api.h"

#include "freeipmi/fiid.h"
#include "freeipmi/ipmi-sdr-record-types.h"
#include "freeipmi/ipmi-sensor-cmds.h"
#include "freeipmi/ipmi-sensor-and-event-code-tables.h"
#include "freeipmi/ipmi-sensor-types-spec.h"
#include "freeipmi/ipmi-sensor-utils.h"
#include "freeipmi/udm/ipmi-sensor-cmds-udm.h"

#include "err-wrappers.h"
#include "fiid-wrappers.h"
#include "freeipmi-portability.h"

enum system_software_type
  {
    IPMI_BIOS,
    IPMI_SMI_HANDLER,
    IPMI_SYSTEM_MANAGEMENT_SOFTWARE,
    IPMI_OEM,
    IPMI_REMOTE_CONSOLE_SOFTWARE,
    IPMI_TERMINAL_MODE_REMOTE_CONSOLE_SOFTWARE,
    IPMI_SYS_SOFT_ID_RESERVED
  };

const char *system_software_type_desc[] =
  {
    "BIOS",
    "SMI Handler",
    "System Management Software",
    "OEM",
    "Remote Console software",
    "Terminal Mode Remote Console software",
    "Reserved",
    NULL
  };

int 
ipmi_sensor_classify (uint8_t event_reading_type_code)
{
  if (event_reading_type_code == 0x01)
    return IPMI_SENSOR_CLASS_THRESHOLD;
  
  if (event_reading_type_code >= 0x02 && event_reading_type_code <= 0x0C)
    return IPMI_SENSOR_CLASS_GENERIC_DISCRETE;
  
  if (event_reading_type_code == 0x6F)
    return IPMI_SENSOR_CLASS_SENSOR_SPECIFIC_DISCRETE;
  
  if (event_reading_type_code >= 0x70 && event_reading_type_code <= 0x7F)
    return IPMI_SENSOR_CLASS_OEM;
  
  return IPMI_SENSOR_CLASS_NOT_AVAILABLE;
}

const char *
ipmi_get_sensor_group (int sensor_type)
{
  if (IPMI_SENSOR_TYPE_VALID(sensor_type))
    return (ipmi_sensor_types[sensor_type]);
  
  if (IPMI_SENSOR_TYPE_IS_OEM (sensor_type))
    return ipmi_oem_sensor_type;

  return NULL;
}

static int
_get_system_software_type (uint8_t system_software_id)
{
  /* To avoid "warning: comparison is always true due to limited range of data type" */
  if ((system_software_id + 1) >= 1 && system_software_id <= 0x0F)
    return IPMI_BIOS;
  if (system_software_id >= 0x10 && system_software_id <= 0x1F)
    return IPMI_SMI_HANDLER;
  if (system_software_id >= 0x20 && system_software_id <= 0x2F)
    return IPMI_SYSTEM_MANAGEMENT_SOFTWARE;
  if (system_software_id >= 0x30 && system_software_id <= 0x3F)
    return IPMI_OEM;
  if (system_software_id >= 0x40 && system_software_id <= 0x46)
    return IPMI_REMOTE_CONSOLE_SOFTWARE;
  if (system_software_id == 0x47)
    return IPMI_TERMINAL_MODE_REMOTE_CONSOLE_SOFTWARE;

  return IPMI_SYS_SOFT_ID_RESERVED;
}

static char **
_get_threshold_message_list (uint8_t sensor_state)
{
  char **event_message_list = NULL;
  char *message_list[16];
  char buf[1024];
  int indx = 0;
  int16_t offset;
  uint16_t bit; 
  int i;
  
  /* achu: multiple threshold flags can be crossed but we only want to
   * output one message at the max.  Luckily for us (and due to smarts
   * by the IPMI specification authors) if we go from high bits to low
   * bits, we will read the flags in the correct order for output.
   */

  for (offset = 5; offset >= 0; offset--)
    {
      bit = pow (2, offset);
      if (sensor_state & bit)
	{
	  if (ipmi_get_threshold_message (offset,
                                          buf,
                                          1024) < 0)
            continue;
	  
	  message_list[indx] = strdup(buf);
	  if (!message_list[indx])
	    goto cleanup;
	  else
	    {
	      indx++;
	      break;
	    }
	}
    }
  
  if (indx)
    {
      event_message_list = (char **) malloc (sizeof (char *) * (indx + 1));
      for (offset = 0; offset < indx; offset++)
	event_message_list[offset] = message_list[offset];
      event_message_list[indx] = NULL;
    }
  
  return event_message_list;

 cleanup:
  for (i = 0; i < indx; i++)
    free(message_list[indx]);
  return NULL;
}

static char **
_get_generic_event_message_list (uint8_t event_reading_type_code, uint16_t sensor_state)
{
  char **event_message_list = NULL;
  char *message_list[16];
  char buf[1024];
  int indx = 0;
  uint16_t offset;
  uint16_t bit; 
  int i;
  
  for (offset = 0; offset < 16; offset++)
    {
      bit = pow (2, offset);
      if (sensor_state & bit)
	{
	  if (ipmi_get_generic_event_message (event_reading_type_code,
					      offset,
					      buf,
					      1024) < 0)
            continue;

	  message_list[indx] = strdup(buf);
	  if (!message_list[indx])
	    goto cleanup;
	  else
	    indx++;
	  
	}
    }
  
  if (indx)
    {
      event_message_list = (char **) malloc (sizeof (char *) * (indx + 1));
      for (offset = 0; offset < indx; offset++)
	event_message_list[offset] = message_list[offset];
      event_message_list[indx] = NULL;
    }
  
  return event_message_list;

 cleanup:
  for (i = 0; i < indx; i++)
    free(message_list[indx]);
  return NULL;
}

static char **
_get_event_message_list (int sensor_type_code, uint16_t sensor_state)
{
  char **event_message_list = NULL;
  char *message_list[16];
  char buf[1024];
  int indx = 0;
  uint16_t offset;
  uint16_t bit; 
  int i;
  
  for (offset = 0; offset < 16; offset++)
    {
      bit = pow (2, offset);
      if (sensor_state & bit)
	{
	  if (ipmi_get_sensor_type_code_message (sensor_type_code,
						 offset,
						 buf,
						 1024) < 0)
            continue;

	  message_list[indx] = strdup(buf);
	  if (!message_list[indx])
	    goto cleanup;
	  else
	    indx++;
	}
    }
  
  if (indx)
    {
      event_message_list = (char **) malloc (sizeof (char *) * (indx + 1));
      for (offset = 0; offset < indx; offset++)
	event_message_list[offset] = message_list[offset];
      event_message_list[indx] = NULL;
    }
  
  return event_message_list;

 cleanup:
  for (i = 0; i < indx; i++)
    free(message_list[indx]);
  return NULL;
}

int8_t 
get_sensor_reading (ipmi_device_t dev, 
		    sdr_record_t *sdr_record, 
		    sensor_reading_t *sensor_reading)
{
  fiid_template_t l_tmpl_cmd_get_sensor_reading_threshold_rs =
    {
      {8, "cmd", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {8, "comp_code", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {8, "sensor_reading", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {5, "reserved1", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "reading_state", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "sensor_scanning", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "all_event_messages", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {6, "sensor_state", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {2, "reserved2", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      /* optional byte */
      {8, "ignore", FIID_FIELD_OPTIONAL | FIID_FIELD_LENGTH_FIXED}, 
      
      {0,  "", 0}
    };
  
  fiid_template_t l_tmpl_cmd_get_sensor_reading_discrete_rs =
    {
      {8, "cmd", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {8, "comp_code", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {8, "sensor_reading", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {5, "reserved1", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "reading_state", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "sensor_scanning", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "all_event_messages", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {15, "sensor_state", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      {1, "reserved2", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED}, 
      
      {0,  "", 0}
    };
  
  uint8_t slave_sys_soft_id;
  uint8_t event_reading_type_code;
  uint8_t sensor_number;
  uint8_t sensor_type;
  short b = 0;
  short m = 0;
  char r_exponent = 0;
  char b_exponent = 0;
  uint8_t linear = 0;
  uint8_t analog_data_format = 0;
  int rv = -1;
  fiid_obj_t obj_cmd_rs = NULL;  
  fiid_obj_t l_obj_cmd_rs = NULL;
  uint8_t buf[1024];
  int32_t len;
  uint64_t val;
  
  ERR_EINVAL (dev
	      && sdr_record
	      && sensor_reading);

  switch (sdr_record->record_type)
    {
    case IPMI_SDR_FORMAT_FULL_RECORD:
      slave_sys_soft_id = sdr_record->record.sdr_full_record.sensor_owner_id;
      if (_get_system_software_type (slave_sys_soft_id) == IPMI_SYS_SOFT_ID_RESERVED)
	return -1;
      
      event_reading_type_code = sdr_record->record.sdr_full_record.event_reading_type_code;
      sensor_number = sdr_record->record.sdr_full_record.sensor_number;
      sensor_type = sdr_record->record.sdr_full_record.sensor_type;
      b = sdr_record->record.sdr_full_record.b;
      m = sdr_record->record.sdr_full_record.m;
      r_exponent = sdr_record->record.sdr_full_record.r_exponent;
      b_exponent = sdr_record->record.sdr_full_record.b_exponent;
      linear = sdr_record->record.sdr_full_record.linear;
      analog_data_format = sdr_record->record.sdr_full_record.analog_data_format;
      break;
    case IPMI_SDR_FORMAT_COMPACT_RECORD:
      slave_sys_soft_id = sdr_record->record.sdr_compact_record.sensor_owner_id;
      if (_get_system_software_type (slave_sys_soft_id) == IPMI_SYS_SOFT_ID_RESERVED)
	return -1;
      
      event_reading_type_code = sdr_record->record.sdr_compact_record.event_reading_type_code;
      sensor_number = sdr_record->record.sdr_compact_record.sensor_number;
      sensor_type = sdr_record->record.sdr_compact_record.sensor_type;
      break;
    default:
      ERR(0);
    }
  
  switch (ipmi_sensor_classify (event_reading_type_code))
    {
    case IPMI_SENSOR_CLASS_THRESHOLD:
      FIID_OBJ_CREATE_CLEANUP(obj_cmd_rs, tmpl_cmd_get_sensor_reading_threshold_rs);
      FIID_OBJ_CREATE_CLEANUP(l_obj_cmd_rs, l_tmpl_cmd_get_sensor_reading_threshold_rs);

      if (ipmi_cmd_get_sensor_reading_threshold (dev, 
						 sensor_number, 
						 obj_cmd_rs) != 0)
        goto cleanup;

      FIID_OBJ_GET_ALL_LEN_CLEANUP(len,
				   obj_cmd_rs,
				   buf,
				   1024);

      FIID_OBJ_SET_ALL_CLEANUP (l_obj_cmd_rs, buf, len);
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, "sensor_reading", &val);

      if (sdr_record->record_type == IPMI_SDR_FORMAT_FULL_RECORD
	  && sdr_record->record_type != IPMI_SDR_ANALOG_DATA_FORMAT_NOT_ANALOG)
	{
	  ERR_CLEANUP (!(ipmi_sensor_decode_value (r_exponent, 
						   b_exponent, 
						   m, 
						   b, 
						   linear, 
						   analog_data_format, 
						   (uint8_t) val,
						   &(sensor_reading->current_reading)) < 0));
	}
      else 
	{
	  sensor_reading->current_reading = val;
	}
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "reading_state", 
			    &val);
      sensor_reading->reading_state = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_scanning", 
			    &val);
      sensor_reading->sensor_scanning = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "all_event_messages", 
			    &val);
      sensor_reading->event_messages_flag = val;
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_state", 
			    &val);
      sensor_reading->event_message_list = 
	_get_threshold_message_list (val);
      
      rv = 0;
      break;
    case IPMI_SENSOR_CLASS_GENERIC_DISCRETE:
      FIID_OBJ_CREATE_CLEANUP(obj_cmd_rs, tmpl_cmd_get_sensor_reading_discrete_rs);

      FIID_OBJ_CREATE_CLEANUP(l_obj_cmd_rs, l_tmpl_cmd_get_sensor_reading_discrete_rs);

      if (ipmi_cmd_get_sensor_reading_discrete (dev, 
						sensor_number, 
						obj_cmd_rs) != 0)
        goto cleanup;
      
      FIID_OBJ_GET_ALL_LEN_CLEANUP(len,
				   obj_cmd_rs,
				   buf,
				   1024);

      FIID_OBJ_SET_ALL_CLEANUP (l_obj_cmd_rs, buf, len);

      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, "sensor_reading", &val);

      if (sdr_record->record_type == IPMI_SDR_FORMAT_FULL_RECORD
	  && sdr_record->record_type != IPMI_SDR_ANALOG_DATA_FORMAT_NOT_ANALOG)
	{
	  ERR_CLEANUP (!(ipmi_sensor_decode_value (r_exponent, 
						   b_exponent, 
						   m, 
						   b, 
						   linear, 
						   analog_data_format, 
						   (uint8_t) val,
						   &(sensor_reading->current_reading)) < 0));
	}
      else 
	{
	  sensor_reading->current_reading = val;
	}
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "reading_state", 
			    &val);
      sensor_reading->reading_state = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_scanning", 
			    &val);
      sensor_reading->sensor_scanning = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "all_event_messages", 
			    &val);
      sensor_reading->event_messages_flag = val;
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_state", 
			    &val);
      sensor_reading->event_message_list = 
	_get_generic_event_message_list (event_reading_type_code, val);
      
      rv = 0;
      break;
    case IPMI_SENSOR_CLASS_SENSOR_SPECIFIC_DISCRETE:
      FIID_OBJ_CREATE_CLEANUP(obj_cmd_rs, tmpl_cmd_get_sensor_reading_discrete_rs);

      FIID_OBJ_CREATE_CLEANUP(l_obj_cmd_rs, l_tmpl_cmd_get_sensor_reading_discrete_rs);

      if (ipmi_cmd_get_sensor_reading_discrete (dev, 
						sensor_number, 
						obj_cmd_rs) != 0)
        goto cleanup;
      
      FIID_OBJ_GET_ALL_LEN_CLEANUP(len,
				   obj_cmd_rs,
				   buf,
				   1024);

      FIID_OBJ_SET_ALL_CLEANUP (l_obj_cmd_rs, buf, len);

      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, "sensor_reading", &val);

      if (sdr_record->record_type == IPMI_SDR_FORMAT_FULL_RECORD
	  && sdr_record->record_type != IPMI_SDR_ANALOG_DATA_FORMAT_NOT_ANALOG)
	{
	  ERR_CLEANUP (!(ipmi_sensor_decode_value (r_exponent, 
						   b_exponent, 
						   m, 
						   b, 
						   linear, 
						   analog_data_format, 
						   (uint8_t) val,
						   &(sensor_reading->current_reading)) < 0));
	}
      else 
	{
	  sensor_reading->current_reading = val;
	}
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "reading_state", 
			    &val);
      sensor_reading->reading_state = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_scanning", 
			    &val);
      sensor_reading->sensor_scanning = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "all_event_messages", 
			    &val);
      sensor_reading->event_messages_flag = val;
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_state", 
			    &val);
      sensor_reading->event_message_list = 
	_get_event_message_list (sensor_type, val);
      
      rv = 0;
      break;
    case IPMI_SENSOR_CLASS_OEM:
      FIID_OBJ_CREATE_CLEANUP(obj_cmd_rs, tmpl_cmd_get_sensor_reading_discrete_rs);

      FIID_OBJ_CREATE_CLEANUP(l_obj_cmd_rs, l_tmpl_cmd_get_sensor_reading_discrete_rs);

      if (ipmi_cmd_get_sensor_reading_discrete (dev, 
						sensor_number, 
						obj_cmd_rs) != 0)
        goto cleanup;
      
      FIID_OBJ_GET_ALL_LEN_CLEANUP(len,
				   obj_cmd_rs,
				   buf,
				   1024);

      FIID_OBJ_SET_ALL_CLEANUP (l_obj_cmd_rs, buf, len);

      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_reading",
			    &val);
      sensor_reading->current_reading = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "reading_state", 
			    &val);
      sensor_reading->reading_state = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_scanning", 
			    &val);
      sensor_reading->sensor_scanning = val;
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "all_event_messages", 
			    &val);
      sensor_reading->event_messages_flag = val;
      
      FIID_OBJ_GET_CLEANUP (l_obj_cmd_rs, 
			    "sensor_state", 
			    &val);

      {
	char *event_message = NULL;
	asprintf (&event_message, 
		  "OEM State = %04Xh", 
		  (uint16_t) val);
	sensor_reading->event_message_list = (char **) malloc (sizeof (char *) * 2);
	sensor_reading->event_message_list[0] = event_message;
	sensor_reading->event_message_list[1] = NULL;
      }
      
      rv = 0;
      break;
    }
  
 cleanup:
  FIID_OBJ_DESTROY_NO_RETURN(obj_cmd_rs);
  FIID_OBJ_DESTROY_NO_RETURN(l_obj_cmd_rs);
  return (rv);
}

