/*****************************************************************************\
 *  $Id: ipmi_monitoring.h,v 1.2 2007-02-16 20:23:31 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2006 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-222073
 *
 *  This file is part of Ipmimonitoring, an IPMI sensor monitoring
 *  library.  For details, see http://www.llnl.gov/linux/.
 *
 *  Ipmimonitoring is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  Ipmimonitoring is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmimonitoring; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _IPMI_MONITORING_H
#define _IPMI_MONITORING_H

#ifdef __cplusplus
extern "C" {
#endif

enum ipmi_monitoring_error_codes 
  {
    IPMI_MONITORING_ERR_SUCCESS                   = 0,
    IPMI_MONITORING_ERR_CONTEXT_NULL              = 1,
    IPMI_MONITORING_ERR_CONTEXT_INVALID           = 2,
    IPMI_MONITORING_ERR_PARAMETERS                = 3,
    IPMI_MONITORING_ERR_PERMISSION                = 4,
    IPMI_MONITORING_ERR_LIBRARY_UNINITIALIZED     = 5,
    IPMI_MONITORING_ERR_CONFIG_FILE_PARSE         = 6,
    IPMI_MONITORING_ERR_SENSOR_CONFIG_FILE_PARSE  = 7,
    IPMI_MONITORING_ERR_SDR_CACHE_PERMISSION      = 8,
    IPMI_MONITORING_ERR_SDR_CACHE_FILESYSTEM      = 9, 
    IPMI_MONITORING_ERR_HOSTNAME_INVALID          = 10,
    IPMI_MONITORING_ERR_SENSOR_NOT_FOUND          = 11,
    IPMI_MONITORING_ERR_NO_SENSOR_READINGS        = 12,
    IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END  = 13,
    IPMI_MONITORING_ERR_SESSION_TIMEOUT           = 14,
    IPMI_MONITORING_ERR_USERNAME                  = 15,
    IPMI_MONITORING_ERR_PASSWORD                  = 16,
    IPMI_MONITORING_ERR_PRIVILEGE_LEVEL           = 17,
    IPMI_MONITORING_ERR_AUTHENTICATION_TYPE       = 18,
    IPMI_MONITORING_ERR_BMC_BUSY                  = 19,
    IPMI_MONITORING_ERR_IPMI                      = 20,
    IPMI_MONITORING_ERR_OUT_OF_MEMORY             = 21,
    IPMI_MONITORING_ERR_SYSTEM_ERROR              = 22,
    IPMI_MONITORING_ERR_INTERNAL                  = 23,
    IPMI_MONITORING_ERR_ERRNUMRANGE               = 24,
  };

enum ipmi_monitoring_sensor_group
  {
    IPMI_MONITORING_SENSOR_GROUP_TEMPERATURE                         = 0x00,
    IPMI_MONITORING_SENSOR_GROUP_VOLTAGE                             = 0x01,
    IPMI_MONITORING_SENSOR_GROUP_CURRENT                             = 0x02,
    IPMI_MONITORING_SENSOR_GROUP_FAN                                 = 0x03,
    IPMI_MONITORING_SENSOR_GROUP_PHYSICAL_SECURITY                   = 0x04,
    IPMI_MONITORING_SENSOR_GROUP_PLATFORM_SECURITY_VIOLATION_ATTEMPT = 0x05,
    IPMI_MONITORING_SENSOR_GROUP_PROCESSOR                           = 0x06,
    IPMI_MONITORING_SENSOR_GROUP_POWER_SUPPLY                        = 0x07,
    IPMI_MONITORING_SENSOR_GROUP_POWER_UNIT                          = 0x08,
    IPMI_MONITORING_SENSOR_GROUP_MEMORY                              = 0x09,
    IPMI_MONITORING_SENSOR_GROUP_DRIVE_SLOT                          = 0x0A,
    IPMI_MONITORING_SENSOR_GROUP_SYSTEM_FIRMWARE_PROGRESS            = 0x0B,
    IPMI_MONITORING_SENSOR_GROUP_EVENT_LOGGING_DISABLED              = 0x0C,
    IPMI_MONITORING_SENSOR_GROUP_SYSTEM_EVENT                        = 0x0D, 
    IPMI_MONITORING_SENSOR_GROUP_CRITICAL_INTERRUPT                  = 0x0E,
    IPMI_MONITORING_SENSOR_GROUP_MODULE_BOARD                        = 0x0F, 
    IPMI_MONITORING_SENSOR_GROUP_SLOT_CONNECTOR                      = 0x10,
    IPMI_MONITORING_SENSOR_GROUP_WATCHDOG2                           = 0x11,
    IPMI_MONITORING_SENSOR_GROUP_UNKNOWN                             = 0x12,
  } ipmi_monitoring_sensor_group_t;

enum ipmi_monitoring_sensor_state
  {
    IPMI_MONITORING_SENSOR_STATE_NOMINAL  = 0x00,
    IPMI_MONITORING_SENSOR_STATE_WARNING  = 0x01,
    IPMI_MONITORING_SENSOR_STATE_CRITICAL = 0x02,
    IPMI_MONITORING_SENSOR_STATE_UNKNOWN  = 0x03,
  };

enum ipmi_monitoring_sensor_units
  {
    IPMI_MONITORING_SENSOR_UNITS_NONE       = 0x00,
    IPMI_MONITORING_SENSOR_UNITS_CELSIUS    = 0x01,
    IPMI_MONITORING_SENSOR_UNITS_FAHRENHEIT = 0x02,
    IPMI_MONITORING_SENSOR_UNITS_VOLTS      = 0x03,
    IPMI_MONITORING_SENSOR_UNITS_AMPS       = 0x04,
    IPMI_MONITORING_SENSOR_UNITS_RPM        = 0x05,
    IPMI_MONITORING_SENSOR_UNITS_UNKNOWN    = 0x06,
  };

enum ipmi_monitoring_sensor_reading_type
  {
    IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER8_BOOL     = 0x00,
    IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER32         = 0x01,
    IPMI_MONITORING_SENSOR_READING_TYPE_DOUBLE                     = 0x02,
    IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER16_BITMASK  = 0x03,
    IPMI_MONITORING_SENSOR_READING_TYPE_UNKNOWN                    = 0x04,
  };

enum ipmi_monitoring_sensor_bitmask_type
  {
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION                          = 0x00,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_STATE                               = 0x01,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_PREDICTIVE_FAILURE                  = 0x02,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_LIMIT                               = 0x03,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_PERFORMANCE                         = 0x04,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION_SEVERITY                 = 0x05,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_DEVICE_INSTALL                      = 0x06,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_DEVICE_STATE                        = 0x07,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION_DEVICE                   = 0x08,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_REDUNDANCY                          = 0x09,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_STATE                         = 0x0A,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_PHYSICAL_SECURITY                   = 0x0B,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_PLATFORM_SECURITY_VIOLATION_ATTEMPT = 0x0C,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_PROCESSOR                           = 0x0D, 
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_SUPPLY                        = 0x0E,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_UNIT                          = 0x0F,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_MEMORY                              = 0x10,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_DRIVE_SLOT                          = 0x11,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_SYSTEM_FIRMWARE_PROGRESS            = 0x12,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_EVENT_LOGGING_DISABLED              = 0x13,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_SYSTEM_EVENT                        = 0x14, 
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_CRITICAL_INTERRUPT                  = 0x15,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_MODULE_BOARD                        = 0x16, 
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_SLOT_CONNECTOR                      = 0x17,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_WATCHDOG2                           = 0x18,
    IPMI_MONITORING_SENSOR_BITMASK_TYPE_UNKNOWN                             = 0x19,
  };

enum ipmi_monitoring_privilege_type
  {
    IPMI_MONITORING_PRIVILEGE_USER     = 0x00,
    IPMI_MONITORING_PRIVILEGE_OPERATOR = 0x01,
    IPMI_MONITORING_PRIVILEGE_ADMIN    = 0x02,
  };

enum ipmi_monitoring_authentication_type
  {
    IPMI_MONITORING_AUTHENTICATION_TYPE_NONE                  = 0x00,
    IPMI_MONITORING_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY = 0x01,
    IPMI_MONITORING_AUTHENTICATION_TYPE_MD2                   = 0x02,
    IPMI_MONITORING_AUTHENTICATION_TYPE_MD5                   = 0x03,
  };

enum ipmi_monitoring_flags
  {
    IPMI_MONITORING_FLAGS_NONE               = 0x00,
    IPMI_MONITORING_FLAGS_DEBUG_STDOUT       = 0x01,
    IPMI_MONITORING_FLAGS_DEBUG_STDERR       = 0x02,
    IPMI_MONITORING_FLAGS_DEBUG_SYSLOG       = 0x04,
    IPMI_MONITORING_FLAGS_DEBUG_IPMI_PACKETS = 0x08,
    IPMI_MONITORING_FLAGS_DO_NOT_LOCK_MEMORY = 0x10,
  };

enum ipmi_monitoring_workaround_flags
  {
    IPMI_MONITORING_WORKAROUND_FLAGS_SESSION_ID_ZERO = 0x00000001,
  };

enum ipmi_monitoring_sensor_reading_flags
  {
    IPMI_MONITORING_SENSOR_READING_FLAGS_REREAD_SDR_CACHE          = 0x00000001,
    IPMI_MONITORING_SENSOR_READING_FLAGS_IGNORE_UNREADABLE_SENSORS = 0x00000002,
  };

/* 
 * IPMI Bitmasks
 *
 * The following are the bitmask masks that can be returned by
 * those sensors that return bitmasks.  Use the returned bitmask type
 * to determine which set of bitmasks to use.
 */
enum ipmi_monitoring_sensor_bitmask_transition 
  {
    IPMI_MONITORING_BITMASK_TRANSITION_TO_IDLE = 0x01,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_ACTIVE = 0x02,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_BUSY = 0x03,
  };

enum ipmi_monitoring_sensor_bitmask_state
  {
    IPMI_MONITORING_BITMASK_STATE_DEASSERTED = 0x01,
    IPMI_MONITORING_BITMASK_STATE_ASSERTED = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_predictive_failure
  {
    IPMI_MONITORING_BITMASK_PREDICTIVE_FAILURE_DEASSERTED = 0x01,
    IPMI_MONITORING_BITMASK_PREDICTIVE_FAILURE_ASSERTED = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_limit
  {
    IPMI_MONITORING_BITMASK_LIMIT_NOT_EXCEEDED = 0x01,
    IPMI_MONITORING_BITMASK_LIMIT_EXCEEDED = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_performance
  {
    IPMI_MONITORING_BITMASK_PERFORMANCE_MET = 0x01,
    IPMI_MONITORING_BITMASK_PERFORMANCE_LAGS = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_transition_severity
  {
    IPMI_MONITORING_BITMASK_TRANSITION_TO_OK = 0x01,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_NON_CRITICAL_FROM_OK = 0x02,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_CRITICAL_FROM_LESS_SEVERE = 0x03,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_NON_RECOVERABLE_FROM_LESS_SEVERE = 0x04,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_NON_CRITICAL_FROM_MORE_SEVERE = 0x05,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_CRITICAL_FROM_NON_RECOVERABLE = 0x06,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_NON_RECOVERABLE = 0x07,
    IPMI_MONITORING_BITMASK_TRANSITION_MONITOR = 0x08,
    IPMI_MONITORING_BITMASK_TRANSITION_INFORMATIONAL = 0x09,
  };

enum ipmi_monitoring_sensor_bitmask_device_install
  {
    IPMI_MONITORING_BITMASK_DEVICE_REMOVED_DEVICE_ABSENT = 0x01,
    IPMI_MONITORING_BITMASK_DEVICE_INSERTED_DEVICE_PRESENT = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_device_state
  {
    IPMI_MONITORING_BITMASK_DEVICE_DISABLED = 0x01,
    IPMI_MONITORING_BITMASK_DEVICE_ENABLED = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_transition_device
  {
    IPMI_MONITORING_BITMASK_TRANSITION_TO_RUNNING = 0x01,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_IN_TEST = 0x02,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_POWER_OFF = 0x03,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_ON_LINE = 0x04,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_OFF_LINE = 0x05,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_OFF_DUTY = 0x06,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_DEGRADED = 0x07,
    IPMI_MONITORING_BITMASK_TRANSITION_TO_POWER_SAVE = 0x08,
    IPMI_MONITORING_BITMASK_TRANSITION_INSTALL_ERROR = 0x09,
  };

enum ipmi_monitoring_sensor_bitmask_redundancy
  {
    IPMI_MONITORING_BITMASK_FULLY_REDUNDANT = 0x01,
    IPMI_MONITORING_BITMASK_REDUNDANCY_LOST = 0x02,
    IPMI_MONITORING_BITMASK_REDUNDANCY_DEGRADED = 0x03,
    IPMI_MONITORING_BITMASK_ENTERED_FROM_REDUNDANCY_DEGRADED_OR_FULLY_REDUNDANT = 0x04,
    IPMI_MONITORING_BITMASK_ENTERED_FROM_NON_REDUNDANT_INSUFFICIENT_RESOURCES = 0x05,
    IPMI_MONITORING_BITMASK_NON_REDUNDANT_INSUFFICIENT_RESOURCES = 0x06,
    IPMI_MONITORING_BITMASK_REDUNDANCY_DEGRADED_FROM_FULLY_REDUNDANT = 0x07,
    IPMI_MONITORING_BITMASK_REDUNDANCY_DEGRADED_FROM_NON_REDUNDANT = 0x08,
  };

enum ipmi_monitoring_sensor_bitmask_power_state
  {
    IPMI_MONITORING_BITMASK_D0_POWER_STATE = 0x01,
    IPMI_MONITORING_BITMASK_D1_POWER_STATE = 0x02,
    IPMI_MONITORING_BITMASK_D2_POWER_STATE = 0x03,
    IPMI_MONITORING_BITMASK_D3_POWER_STATE = 0x04,
  };

enum ipmi_monitoring_sensor_bitmask_physical_security
  {
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_GENERAL_CHASSIS_INTRUSION = 0x01,
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_DRIVE_BAY_INTRUSION = 0x02,
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_IO_CARD_INTRUSION = 0x03,
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_PROCESSOR_AREA_INTRUSION = 0x04, 
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_LAN_LEASH_LOST = 0x05,
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_UNAUTHORIZED_DOCK_UNDOCK = 0x06,
    IPMI_MONITORING_BITMASK_PHYSICAL_SECURITY_FAN_AREA_INTRUSION = 0x07,
  };

enum ipmi_monitoring_sensor_bitmask_platform_security_violation_attempt
  {
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_SECURE_MODE_VIOLATION_ATTEMPT = 0x01,
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_PRE_BOOT_PASSWORD_VIOLATION_USER_PASSWORD = 0x02,
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_PRE_BOOT_PASSWORD_VIOLATION_SETUP_PASSWORD = 0x03,
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_PRE_BOOT_PASSWORD_VIOLATION_NETWORK_BOOT_PASSWORD = 0x04,
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_OTHER_PRE_BOOT_PASSWORD_VIOLATION = 0x05,
    IPMI_MONITORING_BITMASK_PLATFORM_SECURITY_VIOLATION_ATTEMPT_OUT_OF_BAND_ACCESS_PASSWORD_VIOLATION = 0x06,
  };

enum ipmi_monitoring_sensor_bitmask_processor
  {
    IPMI_MONITORING_BITMASK_PROCESSOR_IERR = 0x01,
    IPMI_MONITORING_BITMASK_PROCESSOR_THERMAL_TRIP = 0x02,
    IPMI_MONITORING_BITMASK_PROCESSOR_FRB1_BIST_FAILURE = 0x03,
    IPMI_MONITORING_BITMASK_PROCESSOR_FRB2_HANG_IN_POST_FAILURE = 0x04,
    IPMI_MONITORING_BITMASK_PROCESSOR_FRB3_PROCESSOR_STARTUP_INITIALIZATION_FAILURE = 0x05,
    IPMI_MONITORING_BITMASK_PROCESSOR_CONFIGURATION_ERROR = 0x06,
    IPMI_MONITORING_BITMASK_PROCESSOR_SMBIOS_UNCORRECTABLE_CPU_COMPLEX_ERROR = 0x07,
    IPMI_MONITORING_BITMASK_PROCESSOR_PROCESSOR_PRESENCE_DETECTED = 0x08,
    IPMI_MONITORING_BITMASK_PROCESSOR_PROCESSOR_DISABLED = 0x09,
    IPMI_MONITORING_BITMASK_PROCESSOR_TERMINATOR_PRESENCE_DETECTED = 0x0A,
    IPMI_MONITORING_BITMASK_PROCESSOR_PROCESSOR_AUTOMATICALLY_THROTTLED = 0x0B,
  };

enum ipmi_monitoring_sensor_bitmask_power_supply
  {
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_PRESENCE_DETECTED = 0x01,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_POWER_SUPPLY_FAILURE_DETECTED = 0x02,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_PREDICTIVE_FAILURE = 0x03,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_POWER_SUPPLY_INPUT_LOST_AC_DC = 0x04,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_POWER_SUPPLY_INPUT_LOST_OR_OUT_OF_RANGE = 0x05,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_POWER_SUPPLY_INPUT_OUT_OF_RANGE_BUT_PRESENT = 0x06,
    IPMI_MONITORING_BITMASK_POWER_SUPPLY_CONFIGURATION_ERROR = 0x07,
  };

enum ipmi_monitoring_sensor_bitmask_power_unit
  {
    IPMI_MONITORING_BITMASK_POWER_UNIT_POWER_OFF_POWER_DOWN = 0x01,
    IPMI_MONITORING_BITMASK_POWER_UNIT_POWER_CYCLE = 0x02,
    IPMI_MONITORING_BITMASK_POWER_UNIT_240VA_POWER_DOWN = 0x03,
    IPMI_MONITORING_BITMASK_POWER_UNIT_INTERLOCK_POWER_DOWN = 0x04,
    IPMI_MONITORING_BITMASK_POWER_UNIT_AC_LOST = 0x05,
    IPMI_MONITORING_BITMASK_POWER_UNIT_SOFT_POWER_CONTROL_FAILURE = 0x06,
    IPMI_MONITORING_BITMASK_POWER_UNIT_POWER_UNIT_FAILURE_DETECTED = 0x07,
    IPMI_MONITORING_BITMASK_POWER_UNIT_PREDICTIVE_FAILURE = 0x08,
  };

enum ipmi_monitoring_sensor_bitmask_memory
  {
    IPMI_MONITORING_BITMASK_MEMORY_CORRECTABLE_ECC_MEMORY_ERROR = 0x01,
    IPMI_MONITORING_BITMASK_MEMORY_UNCORRECTABLE_ECC_MEMORY_ERROR = 0x02,
    IPMI_MONITORING_BITMASK_MEMORY_PARITY = 0x03,
    IPMI_MONITORING_BITMASK_MEMORY_MEMORY_SCRUB_FAILED = 0x04,
    IPMI_MONITORING_BITMASK_MEMORY_MEMORY_DEVICE_DISABLED = 0x05,
    IPMI_MONITORING_BITMASK_MEMORY_CORRECTABLE_ECC_MEMORY_ERROR_LOGGING_LIMIT_REACHED = 0x06,
    IPMI_MONITORING_BITMASK_MEMORY_PRESENCE_DETECTED = 0x07,
    IPMI_MONITORING_BITMASK_MEMORY_CONFIGURATION_ERROR = 0x08,
    IPMI_MONITORING_BITMASK_MEMORY_SPARE = 0x09,
    IPMI_MONITORING_BITMASK_MEMORY_MEMORY_AUTOMATICALLY_THROTTLED = 0x0A,
  };

enum ipmi_monitoring_sensor_bitmask_drive_slot
  {
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_DRIVE_PRESENCE = 0x01,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_DRIVE_FAULT = 0x02,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_PREDICTIVE_FAILURE = 0x03,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_HOT_SPARE = 0x04,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_CONSISTENCY_CHECK_PARITY_CHECK_IN_PROGRESS = 0x05,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_IN__ARRAY = 0x06,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_IN_FAILED_ARRAY = 0x07,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_REBUILD_REMAP_IN_PROGRESS = 0x08,
    IPMI_MONITORING_BITMASK_DRIVE_SLOT_REBUILD_REMAP_ABORTED = 0x09,
  };

enum ipmi_monitoring_sensor_bitmask_system_firmware_progress
  {
    IPMI_MONITORING_BITMASK_SYSTEM_FIRMWARE_PROGRESS_SYSTEM_FIRMWARE_ERROR = 0x01,
    IPMI_MONITORING_BITMASK_SYSTEM_FIRMWARE_PROGRESS_SYSTEM_FIRMWARE_HANG = 0x02,
    IPMI_MONITORING_BITMASK_SYSTEM_FIRMWARE_PROGRESS_SYSTEM_FIRMWARE_PROGRESS = 0x03,
  };

enum ipmi_monitoring_sensor_bitmask_event_logging_disabled
  {
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_CORRECTABLE_MEMORY_LOGGING_DISABLED = 0x01,
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_EVENT_TYPE_LOGGING_DISABLED = 0x02,
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_LOG_AREA_RESET_CLEARED = 0x03,
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_ALL_EVENT_LOGGING_DISABLED = 0x04,
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_SEL_FULL = 0x05,
    IPMI_MONITORING_BITMASK_EVENT_LOGGING_DISABLED_SEL_ALMOST_FULL = 0x06,
  };

enum ipmi_monitoring_sensor_bitmask_system_event
  {
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_SYSTEM_RECONFIGURED = 0x01,
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_OEM_SYSTEM_BOOT_EVENT = 0x02,
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_UNDETERMINED_SYSTEM_HARDWARE_FAILURE = 0x03,
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_ENTRY_ADDED_TO_AUXILIARY_LOG = 0x04, 
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_PEF_ACTION = 0x05,
    IPMI_MONITORING_BITMASK_SYSTEM_EVENT_TIMESTAMP_CLOCK_SYNC = 0x06,
  };

enum ipmi_monitoring_sensor_bitmask_critical_interrupt
  {
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_FRONT_PANEL_NMI_DIAGNOSTIC_INTERRUPT = 0x01,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_BUS_TIMEOUT = 0x02,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_I_O_CHANNEL_CHECK_NMI = 0x03,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_SOFTWARE_NMI = 0x04,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_PCI_PERR = 0x05,             
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_PCI_SERR = 0x06,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_EISA_FAIL_SAFE_TIMEOUT = 0x07,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_BUS_CORRECTABLE_ERROR = 0x08,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_BUS_UNCORRECTABLE_ERROR = 0x09,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_FATAL_NMI = 0x0A,
    IPMI_MONITORING_BITMASK_CRITICAL_INTERRUPT_BUS_FATAL_ERROR = 0x0B,
  };

enum ipmi_monitoring_sensor_bitmask_module_board
  {
    IPMI_MONITORING_BITMASK_CABLE_INTERCONNECT_IS_CONNECTED = 0x01,
    IPMI_MONITORING_BITMASK_CABLE_INTERCONNECT_INCORRECT_CABLE_CONNECTED_INCORRECT_INTERCONNECTION = 0x02,
  };

enum ipmi_monitoring_sensor_bitmask_slot_connector
  {
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_FAULT_STATUS_ASSERTED = 0x01,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_IDENTIFY_STATUS_ASSERTED = 0x02,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_CONNECTOR_DEVICE_INSTALLED_ATTACHED = 0x03,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_CONNECTOR_READY_FOR_DEVICE_INSTALLATION = 0x04,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_CONNECTOR_READY_FOR_DEVICE_REMOVAL = 0x05,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_POWER_IS_OFF = 0x06,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_CONNECTOR_DEVICE_REMOVAL_REQUEST = 0x07,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_INTERLOCK_ASSERTED = 0x08,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_IS_DISABLED = 0x09,
    IPMI_MONITORING_BITMASK_SLOT_CONNECTOR_SLOT_HOLDS_SPARE_DEVICE = 0x0A,
  };

enum ipmi_monitoring_sensor_bitmask_watchdog2
  {
    IPMI_MONITORING_BITMASK_WATCHDOG2_TIMER_EXPIRED = 0x01,
    IPMI_MONITORING_BITMASK_WATCHDOG2_HARD_RESET = 0x02,
    IPMI_MONITORING_BITMASK_WATCHDOG2_POWER_DOWN = 0x03,
    IPMI_MONITORING_BITMASK_WATCHDOG2_POWER_CYCLE = 0x04,
    IPMI_MONITORING_BITMASK_WATCHDOG2_RESERVED1 = 0x05,
    IPMI_MONITORING_BITMASK_WATCHDOG2_RESERVED2 = 0x06,
    IPMI_MONITORING_BITMASK_WATCHDOG2_RESERVED3 = 0x07,
    IPMI_MONITORING_BITMASK_WATCHDOG2_RESERVED4 = 0x08,
    IPMI_MONITORING_BITMASK_WATCHDOG2_TIMER_INTERRUPT = 0x09,
  };

/* 
 * ipmi_monitoring_ipmi_config
 *
 * Configuration information for IPMI Out-of-Band monitoring
 *
 * username
 *
 *   BMC username. Pass NULL ptr for NULL username.  Maximum length of
 *   16 bytes.
 *
 * password
 *
 *   BMC password. Pass NULL ptr for NULL password.  Maximum length of
 *   16 bytes.
 *
 * privilege_level
 *
 *   privilege level to authenticate with.
 *
 *   Supported privilege levels:
 *
 *   IPMI_MONITORING_PRIVILEGE_USER
 *   IPMI_MONITORING_PRIVILEGE_OPERATOR
 *   IPMI_MONITORING_PRIVILEGE_ADMIN
 *
 *   Pass < 0 for default of IPMI_MONITORING_PRIVILEGE_USER.
 *
 * authentication_type
 * 
 *   authentication type to use
 *
 *   IPMI_MONITORING_AUTHENTICATION_TYPE_NONE
 *   IPMI_MONITORING_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY
 *   IPMI_MONITORING_AUTHENTICATION_TYPE_MD2
 *   IPMI_MONITORING_AUTHENTICATION_TYPE_MD5
 * 
 *   Pass < 0 for default of IPMI_MONITORING_AUTHENTICATION_TYPE_MD5.
 *
 * session_timeout_len
 *
 *   Specifies the session timeout length in milliseconds.  Pass <= 0
 *   to default to 60000 (60 seconds).
 *
 * retransmission_timeout_len
 *
 *   Specifies the packet retransmission timeout length in
 *   milliseconds.  Pass <= 0 to default to 500 (0.5 seconds).
 *
 * retransmission_backoff_count
 *
 *   Specifies the packet retransmission count until retransmission
 *   timeout lengths will be backed off.  Pass <= 0 to default to 2.
 *
 * workaround_flags
 *
 *   Bitwise OR of flags indicating any behavior which should be
 *   changed from the default to handle IPMI non-compliance problems.
 *   Some BMCs which are non-compliant may require a workaround flag
 *   for correct operation. Pass 0 for default of no modifications to
 *   behavior.
 */
struct ipmi_monitoring_ipmi_config
{
  char *username;
  char *password;
  int privilege_level;
  int authentication_type;
  int session_timeout_len;
  int retransmission_timeout_len;
  int retransmission_backoff_count;
  unsigned int workaround_flags;
};

typedef struct ipmi_monitoring_ctx *ipmi_monitoring_ctx_t;

/* 
 * ipmi_monitoring_init
 *
 * Initialize the ipmi monitoring library.  Needs to be called only
 * once before any ipmi monitoring functions are called.  Threaded
 * applications are responsible for calling this function before any
 * thread may call another function in the library.
 *
 * Returns 0 on success, -1 on error
 */
int ipmi_monitoring_init(unsigned int flags, int *errnum);

/* 
 * ipmi_monitoring_ctx_create
 *
 * Create an ipmi monitoring context
 *
 * Returns context on success, NULL on error
 */
ipmi_monitoring_ctx_t ipmi_monitoring_ctx_create(void);

/* 
 * ipmi_monitoring_ctx_destroy
 *
 * Destroy an ipmi monitoring context
 */
void ipmi_monitoring_ctx_destroy(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_ctx_errnum
 *
 * Returns the error code of the most recently caused error
 */
int ipmi_monitoring_ctx_errnum(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_ctx_strerror
 *
 * Returns a pointer to statically allocated string describing the
 * error code in errnum.
 */
char *ipmi_monitoring_ctx_strerror(int errnum);

/* 
 * ipmi_monitoring_sensor_readings_by_record_id
 *
 * Retrieve sensor readings by sensor numbers and store them in the monitoring context.
 *
 * If 'hostname' is NULL, sensors for the current node will be retrieved in-band.
 * If 'record_ids' is NULL, default sensors will be retrieved
 * (default == all sensors unless configured otherwise).
 *
 * Returns number of sensors values retrieved on success, -1 on error
 */
int ipmi_monitoring_sensor_readings_by_record_id(ipmi_monitoring_ctx_t c,
                                                 char *hostname,
                                                 struct ipmi_monitoring_ipmi_config *config,
                                                 unsigned int sensor_reading_flags,
                                                 unsigned int *record_ids,
                                                 unsigned int record_ids_len);

/* 
 * ipmi_monitoring_sensor_readings_by_sensor_group
 *
 * Retrieve sensor readings by sensor group and store them in the monitoring context.
 *
 * If 'hostname' is NULL, sensors for the current node will be retrieved in-band.
 * If 'sensor_groups' is NULL, default sensors will be retrieved.
 * (default == all sensors unless configured otherwise).
 *
 * Returns number of sensors values retrieved on success, -1 on error
 */
int ipmi_monitoring_sensor_readings_by_sensor_group(ipmi_monitoring_ctx_t c,
                                                    char *hostname,
                                                    struct ipmi_monitoring_ipmi_config *config,
                                                    unsigned int sensor_reading_flags,
                                                    unsigned int *sensor_groups,
                                                    unsigned int sensor_groups_len);

/* 
 * ipmi_monitoring_iterator_first
 *
 * Reset iterator to the first sensor reading
 * 
 * Returns 0 on success, -1 on error
 */
int ipmi_monitoring_iterator_first(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_next
 *
 * Advance iterator to the next set of sensors information
 *
 * Returns 0 on success, -1 on error
 */
int ipmi_monitoring_iterator_next(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_record_id
 *
 * Returns the sensor number
 */
int ipmi_monitoring_iterator_record_id(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_group
 *
 * Returns the sensor group
 */
int ipmi_monitoring_iterator_sensor_group(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_name
 *
 * Returns a pointer to the sensor name
 */
char *ipmi_monitoring_iterator_sensor_name(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_state
 *
 * Returns the current sensor state
 */
int ipmi_monitoring_iterator_sensor_state(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_units
 *
 * Returns the sensor units type
 */
int ipmi_monitoring_iterator_sensor_units(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_reading_type
 *
 * Returns the sensor reading type
 */
int ipmi_monitoring_iterator_sensor_reading_type(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_bitmask_type
 *
 * Returns the bitmask type that should be used if the reading type is
 * a bitmask.
 */
int ipmi_monitoring_iterator_sensor_bitmask_type(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_sensor_reading
 *
 * Returns a pointer to the sensor reading.  It is the responsibility
 * of the user to cast it to the correct type based on the reading
 * type.  Returns NULL if no reading available. 
 */
void *ipmi_monitoring_iterator_sensor_reading(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_iterator_destroy
 *
 * Destroy all internally stored sensors readings.
 */
void ipmi_monitoring_iterator_destroy(ipmi_monitoring_ctx_t c);

/* 
 * ipmi_monitoring_bitmask_string
 *
 * Retrieve a string for the given bitmask.
 *
 * Returns 0 on success, -1 on error
 */
int ipmi_monitoring_bitmask_string(ipmi_monitoring_ctx_t c,
                                   int bitmask_type,
                                   unsigned int bitmask,
                                   char *buffer,
                                   unsigned int buflen);
#ifdef __cplusplus
}
#endif

#endif /* _IPMI_MONITORING_H */
