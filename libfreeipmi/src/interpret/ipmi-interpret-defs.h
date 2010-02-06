/*
 * Copyright (C) 2003-2010 FreeIPMI Core Team
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

#ifndef _IPMI_INTERPRET_DEFS_H
#define _IPMI_INTERPRET_DEFS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdint.h>

#include "freeipmi/interpret/ipmi-interpret.h"

#include "ipmi-interpret-sensor-config.h"

#include "hash.h"

#define IPMI_INTERPRET_CTX_MAGIC 0xACFF3289

#define IPMI_INTERPRET_FLAGS_MASK \
  (IPMI_INTERPRET_FLAGS_INTERPRET_OEM_DATA)

#define IPMI_INTERPRET_MAX_BITMASKS 32

#define IPMI_INTERPRET_HASH_SIZE 32

#define IPMI_OEM_STATE_TYPE_BITMASK 0
#define IPMI_OEM_STATE_TYPE_VALUE   1

#define IPMI_OEM_HASH_KEY_BUFLEN    128

struct ipmi_interpret_sensor_config {
  char *option_str;
  int sensor_state;
};

struct ipmi_interpret_oem_state {
  uint16_t sensor_event_bitmask;
  unsigned int sensor_state;
  int oem_state_type;
};

struct ipmi_interpret_oem_sensor_config {
  char key[IPMI_OEM_HASH_KEY_BUFLEN + 1];
  uint32_t manufacturer_id;
  uint16_t product_id;
  uint8_t event_reading_type_code;
  uint8_t sensor_type;
  struct ipmi_interpret_oem_state oem_state[IPMI_INTERPRET_MAX_BITMASKS];
  unsigned int oem_state_count;
};

struct ipmi_interpret_sensors {
  struct ipmi_interpret_sensor_config **ipmi_interpret_threshold_sensor_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_voltage_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_voltage_performance_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_fan_device_present_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_fan_transition_availability_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_fan_redundancy_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_physical_security_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_platform_security_violation_attempt_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_processor_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_processor_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_supply_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_supply_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_supply_redundancy_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_unit_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_unit_device_present_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_power_unit_redundancy_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_memory_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_drive_slot_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_drive_slot_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_drive_slot_predictive_failure_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_drive_slot_device_present_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_system_firmware_progress_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_event_logging_disabled_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_system_event_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_critical_interrupt_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_button_switch_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_button_switch_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_module_board_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_module_board_device_present_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_cable_interconnect_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_boot_error_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_slot_connector_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_system_acpi_power_state_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_watchdog2_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_entity_presence_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_entity_presence_device_present_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_management_subsystem_health_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_battery_config;
  struct ipmi_interpret_sensor_config **ipmi_interpret_fru_state_config;

  hash_t oem_config;
};

struct ipmi_interpret_ctx {
  uint32_t magic;
  int errnum;
  unsigned int flags;
  uint32_t manufacturer_id;
  uint16_t product_id;

  struct ipmi_interpret_sensors interpret_sensors;
};

#endif /* _IPMI_INTERPRET_DEFS_H */
