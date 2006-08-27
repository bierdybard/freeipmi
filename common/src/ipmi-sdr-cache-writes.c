/*
ipmi-sdr-cache-writes.c: SDR cache creation and management apis.
Copyright (C) 2006 FreeIPMI Core Team

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif	/* HAVE_UNISTD_H */
#include <error.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
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
#endif /* !TIME_WITH_SYS_TIME */
#include <sys/stat.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <errno.h>

#include "freeipmi/fiid.h"
#include "freeipmi/udm/ipmi-sdr-repository-cmds-udm.h"
#include "freeipmi/udm/ipmi-sensor-cmds-udm.h"

#include "bit-ops.h"
#include "err-wrappers.h"
#include "fiid-wrappers.h"
#include "freeipmi-portability.h"
#include "ipmi-common.h"

#include "ipmi-sdr-api.h"

int 
write_sdr_repository_info (FILE *fp, 
			   sdr_repository_info_t *sdr_info)
{
  ERR_EINVAL (fp && sdr_info);
  
  fprintf (fp, "sdr_version_major=%d\n", sdr_info->sdr_version_major);
  fprintf (fp, "sdr_version_minor=%d\n", sdr_info->sdr_version_minor);
  fprintf (fp, "record_count=%d\n", sdr_info->record_count);
  fprintf (fp, "free_space=%d\n", sdr_info->free_space);
  fprintf (fp, "most_recent_addition_timestamp=%d\n", 
	   sdr_info->most_recent_addition_timestamp);
  fprintf (fp, "most_recent_erase_timestamp=%d\n", 
	   sdr_info->most_recent_erase_timestamp);
  fprintf (fp, "get_sdr_repository_allocation_info_command_supported=%d\n", 
	   sdr_info->get_sdr_repository_allocation_info_command_supported);
  fprintf (fp, "reserve_sdr_repository_command_supported=%d\n", 
	   sdr_info->reserve_sdr_repository_command_supported);
  fprintf (fp, "partial_add_sdr_command_supported=%d\n", 
	   sdr_info->partial_add_sdr_command_supported);
  fprintf (fp, "delete_sdr_command_supported=%d\n", 
	   sdr_info->delete_sdr_command_supported);
  fprintf (fp, "modal_non_modal_sdr_repository_update_operation_supported=%d\n", 
	   sdr_info->modal_non_modal_sdr_repository_update_operation_supported);
  fprintf (fp, "overflow_flag=%d\n", sdr_info->overflow_flag);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_full_record (FILE *fp, 
			sdr_full_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "b=%d\n", record->b);
  fprintf (fp, "m=%d\n", record->m);
  fprintf (fp, "r_exponent=%d\n", (int) record->r_exponent);
  fprintf (fp, "b_exponent=%d\n", (int) record->b_exponent);
  fprintf (fp, "linear=%d\n", record->linear);
  fprintf (fp, "analog_data_format=%d\n", record->analog_data_format);
  fprintf (fp, "sensor_owner_id=%d\n", record->sensor_owner_id);
  fprintf (fp, "sensor_number=%d\n", record->sensor_number);
  fprintf (fp, "sensor_type=%d\n", record->sensor_type);
  fprintf (fp, "event_reading_type_code=%d\n", record->event_reading_type_code);
  fprintf (fp, "sensor_unit=%d\n", record->sensor_unit);
  fprintf (fp, "nominal_reading=%f\n", record->nominal_reading);
  fprintf (fp, "normal_minimum=%f\n", record->normal_minimum);
  fprintf (fp, "normal_maximum=%f\n", record->normal_maximum);
  fprintf (fp, "sensor_minimum_reading=%f\n", record->sensor_minimum_reading);
  fprintf (fp, "sensor_maximum_reading=%f\n", record->sensor_maximum_reading);
  fprintf (fp, "negative_going_threshold_hysteresis=%d\n", 
	   record->negative_going_threshold_hysteresis);
  fprintf (fp, "positive_going_threshold_hysteresis=%d\n", 
	   record->positive_going_threshold_hysteresis);
  fprintf (fp, "lower_non_recoverable_threshold=%f\n", 
	   record->lower_non_recoverable_threshold);
  fprintf (fp, "upper_non_recoverable_threshold=%f\n", 
	   record->upper_non_recoverable_threshold);
  fprintf (fp, "lower_critical_threshold=%f\n", 
	   record->lower_critical_threshold);
  fprintf (fp, "upper_critical_threshold=%f\n", 
	   record->upper_critical_threshold);
  fprintf (fp, "lower_non_critical_threshold=%f\n", 
	   record->lower_non_critical_threshold);
  fprintf (fp, "upper_non_critical_threshold=%f\n", 
	   record->upper_non_critical_threshold);
  fprintf (fp, "sensor_name=%s\n", record->sensor_name);
  fprintf (fp, "readable_threshold_lower_critical_threshold=%d\n", 
	   record->readable_threshold_lower_critical_threshold);
  fprintf (fp, "readable_threshold_upper_critical_threshold=%d\n", 
	   record->readable_threshold_upper_critical_threshold);
  fprintf (fp, "readable_threshold_lower_non_critical_threshold=%d\n", 
	   record->readable_threshold_lower_non_critical_threshold);
  fprintf (fp, "readable_threshold_upper_non_critical_threshold=%d\n", 
	   record->readable_threshold_upper_non_critical_threshold);
  fprintf (fp, "readable_threshold_lower_non_recoverable_threshold=%d\n", 
	   record->readable_threshold_lower_non_recoverable_threshold);
  fprintf (fp, "readable_threshold_upper_non_recoverable_threshold=%d\n", 
	   record->readable_threshold_upper_non_recoverable_threshold);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_compact_record (FILE *fp, 
			   sdr_compact_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "sensor_owner_id=%d\n", record->sensor_owner_id);
  fprintf (fp, "sensor_number=%d\n", record->sensor_number);
  fprintf (fp, "sensor_type=%d\n", record->sensor_type);
  fprintf (fp, "event_reading_type_code=%d\n", record->event_reading_type_code);
  fprintf (fp, "sensor_unit=%d\n", record->sensor_unit);
  fprintf (fp, "negative_going_threshold_hysteresis=%d\n", 
	   record->negative_going_threshold_hysteresis);
  fprintf (fp, "positive_going_threshold_hysteresis=%d\n", 
	   record->positive_going_threshold_hysteresis);
  fprintf (fp, "sensor_name=%s\n", record->sensor_name);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_event_only_record (FILE *fp, 
			      sdr_event_only_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "sensor_owner_id=%d\n", record->sensor_owner_id);
  fprintf (fp, "sensor_number=%d\n", record->sensor_number);
  fprintf (fp, "sensor_type=%d\n", record->sensor_type);
  fprintf (fp, "event_reading_type_code=%d\n", record->event_reading_type_code);
  fprintf (fp, "sensor_name=%s\n", record->sensor_name);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_entity_association_record (FILE *fp, 
				      sdr_entity_association_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "container_entity_id=%d\n", record->container_entity_id);
  fprintf (fp, "container_entity_instance=%d\n", record->container_entity_instance);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_generic_device_locator_record (FILE *fp, 
					  sdr_generic_device_locator_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "direct_access_address=%d\n", record->direct_access_address);
  fprintf (fp, "channel_number=%d\n", record->channel_number);
  fprintf (fp, "device_slave_address=%d\n", record->device_slave_address);
  fprintf (fp, "private_bus_id=%d\n", record->private_bus_id);
  fprintf (fp, "lun_for_master_write_read_command=%d\n", 
	   record->lun_for_master_write_read_command);
  fprintf (fp, "address_span=%d\n", record->address_span);
  fprintf (fp, "device_type=%d\n", record->device_type);
  fprintf (fp, "device_type_modifier=%d\n", record->device_type_modifier);
  fprintf (fp, "entity_id=%d\n", record->entity_id);
  fprintf (fp, "entity_instance=%d\n", record->entity_instance);
  fprintf (fp, "device_name=%s\n", record->device_name);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_logical_fru_device_locator_record (FILE *fp, 
					      sdr_logical_fru_device_locator_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "device_type=%d\n", record->device_type);
  fprintf (fp, "device_type_modifier=%d\n", record->device_type_modifier);
  fprintf (fp, "fru_entity_id=%d\n", record->fru_entity_id);
  fprintf (fp, "fru_entity_instance=%d\n", record->fru_entity_instance);
  fprintf (fp, "device_name=%s\n", record->device_name);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_management_controller_device_locator_record (FILE *fp, 
							sdr_management_controller_device_locator_record_t *record)
{
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "entity_id=%d\n", record->entity_id);
  fprintf (fp, "entity_instance=%d\n", record->entity_instance);
  fprintf (fp, "device_name=%s\n", record->device_name);
  fprintf (fp, "\n");
  
  return 0;
}

static int 
_write_sdr_oem_record (FILE *fp, 
		       sdr_oem_record_t *record)
{
  int i;
  
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "manufacturer_id=%d\n", record->manufacturer_id);
  fprintf (fp, "oem_data=");
  for (i = 0; i < record->oem_data_length; i++)
    {
      fprintf (fp, "%d ", record->oem_data[i]);
    }
  fprintf (fp, "\n");
  fprintf (fp, "\n");
  
  return 0;
}

int 
write_sdr_record (FILE *fp, sdr_record_t *record)
{
  
  ERR_EINVAL (fp && record);
  
  fprintf (fp, "record_id=%d\n", record->record_id);
  fprintf (fp, "record_type=%d\n", record->record_type);
  
  switch (record->record_type)
    {
    case IPMI_SDR_FORMAT_FULL_RECORD:
      return _write_sdr_full_record (fp, 
				     &(record->record.sdr_full_record));
    case IPMI_SDR_FORMAT_COMPACT_RECORD:
      return _write_sdr_compact_record (fp, 
					&(record->record.sdr_compact_record));
    case IPMI_SDR_FORMAT_EVENT_ONLY_RECORD:
      return _write_sdr_event_only_record (fp, 
					   &(record->record.sdr_event_only_record));
    case IPMI_SDR_FORMAT_ENTITY_ASSOCIATION_RECORD:
      return _write_sdr_entity_association_record (fp, 
						   &(record->record.sdr_entity_association_record));
    case IPMI_SDR_FORMAT_GENERIC_DEVICE_LOCATOR_RECORD:
      return _write_sdr_generic_device_locator_record (fp, 
						       &(record->record.sdr_generic_device_locator_record));
    case IPMI_SDR_FORMAT_FRU_DEVICE_LOCATOR_RECORD:
      return _write_sdr_logical_fru_device_locator_record (fp, 
							   &(record->record.sdr_logical_fru_device_locator_record));
    case IPMI_SDR_FORMAT_MANAGEMENT_CONTROLLER_DEVICE_LOCATOR_RECORD:
      return _write_sdr_management_controller_device_locator_record (fp, 
								     &(record->record.sdr_management_controller_device_locator_record));
    case IPMI_SDR_FORMAT_OEM_RECORD:
      return _write_sdr_oem_record (fp, 
				    &(record->record.sdr_oem_record));
    }
  
  return (-1);
}

